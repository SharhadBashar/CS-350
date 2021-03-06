#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <kern/wait.h>
#include <lib.h>
#include <syscall.h>
#include <current.h>
#include <proc.h>
#include <thread.h>
#include <addrspace.h>
#include <copyinout.h>
#include <array.h>
#include <synch.h>
#include <spinlock.h>
#include <machine/trapframe.h>
#include <kern/fcntl.h>
#include <vfs.h>
#include "opt-A2.h"
#include "opt-A3.h"
/* this implementation of sys__exit does not do anything with the exit code */
/* this needs to be fixed to get exit() and waitpid() working properly */

void sys__exit(int exitcode)
{
  struct addrspace *as;
  struct proc *p = curproc;

#if OPT_A2
  lock_acquire(exitLock);
  p->dead = true;
  for (unsigned int i = 0; i < array_num(p->child); i++)
  {
    struct proc *childProc = (struct proc *)array_get(p->child, i);
    childProc->parent = NULL;
    if (childProc->dead)
    {
      proc_destroy(childProc);
    }
  }
  p->exitStatus = _MKWAIT_EXIT(exitcode);
  p->exitCode = exitcode;
#endif
#if OPT_A3
  if (exitcode < 0) {
    p->exitStatus = _MKWAIT_SIG(exitcode*(-1)); 
    p->exitCode = exitcode*(-1);
  }
#endif
  /* for now, just include this to keep the compiler from complaining about
     an unused variable */
  (void)exitcode;

  DEBUG(DB_SYSCALL, "Syscall: _exit(%d)\n", exitcode);

  KASSERT(curproc->p_addrspace != NULL);
  as_deactivate();
  /*
   * clear p_addrspace before calling as_destroy. Otherwise if
   * as_destroy sleeps (which is quite possible) when we
   * come back we'll be calling as_activate on a
   * half-destroyed address space. This tends to be
   * messily fatal.
   */
  as = curproc_setas(NULL);
  as_destroy(as);

  /* detach this thread from its process */
  /* note: curproc cannot be used after this call */
  proc_remthread(curthread);

#if OPT_A2
  if (p->parent != NULL)
  {
    lock_acquire(p->parent->lockChild);
    if (!p->parent->dead)
    {
      cv_signal(p->parent->cvChild, p->parent->lockChild);
    }
    lock_release(p->parent->lockChild);
  }
  else
    proc_destroy(p);
  lock_release(exitLock);
#else

  /* if this is the last user process in the system, proc_destroy()
     will wake up the kernel menu thread */
  proc_destroy(p);
#endif

  thread_exit();
  /* thread_exit() does not return, so we should never get here */
  panic("return from thread_exit in sys_exit\n");
}

/* stub handler for getpid() system call                */
int sys_getpid(pid_t *retval)
{
#if OPT_A2
  *retval = curproc->pid;
#else
  /* for now, this is just a stub that always returns a PID of 1 */
  /* you need to fix this to make it work properly */
  *retval = 1;
#endif
  return (0);
}

/* stub handler for waitpid() system call                */

int sys_waitpid(pid_t pid,
                userptr_t status,
                int options,
                pid_t *retval)
{
#if OPT_A2
  if (options != 0)
  {
    return EINVAL;
  }
  lock_acquire(curproc->lockProc);
  struct proc *childProc = NULL;
  for (unsigned int i = 0; i < array_num(curproc->child); i++)
  {
    childProc = (struct proc *)array_get(curproc->child, i);
    if (childProc->pid == pid)
    {
      array_remove(curproc->child, i);
      break;
    }
  }
  lock_release(curproc->lockProc);
  if (childProc == NULL)
  {
    return ECHILD;
  }
  lock_acquire(curproc->lockChild);
  while (!childProc->dead)
  {
    cv_wait(curproc->cvChild, curproc->lockChild);
  }
  lock_release(curproc->lockChild);
  int result = copyout((void *)&childProc->exitStatus, status, sizeof(int));
  lock_acquire(exitLock);
  proc_destroy(childProc);
  lock_release(exitLock);
#else
  int exitstatus;
  int result;

  /* this is just a stub implementation that always reports an
     exit status of 0, regardless of the actual exit status of
     the specified process.   
     In fact, this will return 0 even if the specified process
     is still running, and even if it never existed in the first place.

     Fix this!
  */

  if (options != 0)
  {
    return (EINVAL);
  }
  /* for now, just pretend the exitstatus is 0 */
  exitstatus = 0;
  result = copyout((void *)&exitstatus, status, sizeof(int));
#endif
  if (result)
  {
    return (result);
  }
  *retval = pid;
  return (0);
}

#if OPT_A2
int sys_fork(struct trapframe *tf, int *retval)
{
  struct proc *childProc = proc_create_runprogram("childProc");
  if (childProc == NULL)
  {
    return ENOMEM;
  }
  spinlock_acquire(&childProc->p_lock);
  int status = as_copy(curproc_getas(), &childProc->p_addrspace);
  if (status)
  {
    as_destroy(childProc->p_addrspace);
    proc_destroy(childProc);
    return status;
  }
  spinlock_release(&childProc->p_lock);
  lock_acquire(globalPidLock);
  childProc->pid = globalPid;
  lock_release(globalPidLock);
  lock_acquire(curproc->lockProc);
  childProc->parent = curproc;
  for (unsigned int i = 0; i < array_num(kproc->child); i++)
  {
    if (array_get(kproc->child, i) == childProc)
    {
      array_remove(kproc->child, i);
      break;
    }
  }
  array_add(curproc->child, childProc, NULL);
  lock_release(curproc->lockProc);
  struct trapframe *tfTemp = kmalloc(sizeof(struct trapframe));
  if (tfTemp == NULL)
  {
    lock_acquire(curproc->lockProc);
    for (unsigned int i = 0; i < array_num(curproc->child); i++)
    {
      if (array_get(curproc->child, i) == childProc)
      {
        array_remove(curproc->child, i);
        break;
      }
    }
    lock_release(curproc->lockProc);
    as_destroy(childProc->p_addrspace);
    proc_destroy(childProc);
    return ENOMEM;
  }
  memcpy(tfTemp, tf, sizeof(struct trapframe));
  status = thread_fork("thread", childProc, (void *)&enter_forked_process, tfTemp, 0);
  if (status)
  {
    lock_acquire(curproc->lockProc);
    for (unsigned int i = 0; i < array_num(curproc->child); i++)
    {
      if (array_get(curproc->child, i) == childProc)
      {
        array_remove(curproc->child, i);
        break;
      }
    }
    lock_release(curproc->lockProc);
    as_destroy(childProc->p_addrspace);
    proc_destroy(childProc);
    kfree(tfTemp);
    return status;
  }
  *retval = childProc->pid;
  return 0;
}

int sys_execv(const_userptr_t program, userptr_t *args)
{
  int result;
  if (program == NULL)
  {
    return ENOENT;
  }
  int progname_len = strlen((char *)program) + 1;
  char *progname = kmalloc(progname_len * sizeof(char));
  result = copyin(program, (void *)progname, progname_len);
  if (result)
  {
    return result;
  }
  int argc = 0;
  while (args[argc] != NULL)
  {
    argc++;
  }
  argc++;
  char **argv = kmalloc(argc * sizeof(char *));
  argv[0] = progname;
  for (int i = 0; i < argc - 1; i++)
  {
    int arg_size = strlen((char *)args[i]) + 1;
    argv[i + 1] = kmalloc(arg_size * sizeof(char));
    result = copyinstr(args[i], argv[i + 1], arg_size, NULL);
    if (result)
    {
      return result;
    }
  }
  struct addrspace *as;
  struct vnode *v;
  vaddr_t entrypoint, stackptr;
  result = vfs_open(progname, O_RDONLY, 0, &v);
  if (result)
  {
    for (int i = 0; i < argc; i++)
    {
      kfree(argv[i]);
    }
    kfree(argv);
    return result;
  }
  as = as_create();
  if (as == NULL)
  {
    vfs_close(v);
    for (int i = 0; i < argc; i++)
    {
      kfree(argv[i]);
    }
    kfree(argv);
    return ENOMEM;
  }
  curproc_setas(NULL);
  KASSERT(curproc_getas() == NULL);
  curproc_setas(as);
  as_activate();
  result = load_elf(v, &entrypoint);
  if (result)
  {
    vfs_close(v);
    for (int i = 0; i < argc; i++)
    {
      kfree(argv[i]);
    }
    kfree(argv);
    return result;
  }
  vfs_close(v);
  result = as_define_stack(as, &stackptr);
  if (result)
  {
    for (int i = 0; i < argc; i++)
    {
      kfree(argv[i]);
    }
    kfree(argv);
    return result;
  }
  vaddr_t argv_ptr[argc + 1];
  for (int i = argc - 1; i >= 0; i--)
  {
    stackptr -= strlen(argv[i]) + 1;
    result = copyoutstr(argv[i], (userptr_t)stackptr, strlen(argv[i]) + 1, NULL);
    if (result)
    {
      for (int i = 0; i < argc; i++)
      {
        kfree(argv[i]);
      }
      kfree(argv);
      return result;
    }
    argv_ptr[i] = stackptr;
  }
  argv_ptr[argc] = 0;
  stackptr = ROUNDUP(stackptr - 8, 8);
  stackptr -= ROUNDUP((argc + 1) * sizeof(char *), 8);
  vaddr_t argvptr = stackptr;
  for (int i = 0; i <= argc; i++)
  {
    result = copyout(&argv_ptr[i], (userptr_t)argvptr, sizeof(vaddr_t));
    if (result)
    {
      for (int i = 0; i < argc; i++)
      {
        kfree(argv[i]);
      }
      kfree(argv);
      return result;
    }
    argvptr += sizeof(char *);
  }
  for (int i = 0; i < argc; i++)
  {
    kfree(argv[i]);
  }
  kfree(argv);
  enter_new_process(argc, (userptr_t)stackptr, stackptr, entrypoint);
  panic("Enter new process returned");
  return EINVAL;
}
#endif
