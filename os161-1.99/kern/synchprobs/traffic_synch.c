#include <types.h>
#include <lib.h>
#include <synchprobs.h>
#include <synch.h>
#include <opt-A1.h>
#include <array.h>

/* 
 * This simple default synchronization mechanism allows only vehicle at a time
 * into the intersection.   The intersectionSem is used as a a lock.
 * We use a semaphore rather than a lock so that this code will work even
 * before locks are implemented.
 */

/* 
 * Replace this default synchronization mechanism with your own (better) mechanism
 * needed for your solution.   Your mechanism may use any of the available synchronzation
 * primitives, e.g., semaphores, locks, condition variables.   You are also free to 
 * declare other global variables if your solution requires them.
 */
static struct lock *lock;

static struct cv *cvN;
static struct cv *cvE;
static struct cv *cvS;
static struct cv *cvW;

static struct array *cars;
typedef struct car {
  int id;
  Direction origin;
}car;

//Volatile
static volatile int vId;
static volatile int count;
static volatile int intersectionCount;
static volatile bool n;
static volatile bool e;
static volatile bool s;
static volatile bool w;


/* 
 * The simulation driver will call this function once before starting
 * the simulation
 *
 * You can use it to initialize synchronization and other variables.
 * 
 */
void
intersection_sync_init(void)
{
  lock = lock_create("Lock");
  cvN = cv_create("North");
  cvE = cv_create("East");
  cvS = cv_create("South");
  cvW = cv_create("West"); 
  cars = array_create();
  array_init(cars);

  vId = 0;
  count = 0;
  intersectionCount = 0;
  n = true; e = true; s = true; w = true;

  if (lock == NULL) {
    panic("Could not create Intersection Lock");
  }
  if (cvN == NULL) {
    panic ("Could not create North Conditional Variable");
  }
  if (cvE == NULL) {
    panic ("Could not create East Conditional Variable");
  }
  if (cvS == NULL) {
    panic ("Could not create South Conditional Variable");
  }
  if (cvW == NULL) {
    panic ("Could not create West Conditional Variable");
  }
  if (cars == NULL) {
    panic ("Could not create the array for cars");
  }
  return;
}

/* 
 * The simulation driver will call this function once after
 * the simulation has finished
 *
 * You can use it to clean up any synchronization and other variables.
 *
 */
void
intersection_sync_cleanup(void)
{
  KASSERT(lock != NULL);
  KASSERT(cvN != NULL);
  KASSERT(cvE != NULL);
  KASSERT(cvS != NULL);
  KASSERT(cvW != NULL);
  KASSERT(cars != NULL);

  lock_destroy(lock);
  cv_destroy(cvN);
  cv_destroy(cvE);
  cv_destroy(cvS);
  cv_destroy(cvW);
  array_destroy(cars);
}


/*
 * The simulation driver will call this function each time a vehicle
 * tries to enter the intersection, before it enters.
 * This function should cause the calling simulation thread 
 * to block until it is OK for the vehicle to enter the intersection.
 *
 * parameters:
 *    * origin: the Direction from which the vehicle is arriving
 *    * destination: the Direction in which the vehicle is trying to go
 *
 * return value: none
 */

void
intersection_before_entry(Direction origin, Direction destination) 
{
  (void)destination; /* avoid compiler complaint about unused parameter */

  KASSERT(lock != NULL);
  KASSERT(cvN != NULL);
  KASSERT(cvE != NULL);
  KASSERT(cvS != NULL);
  KASSERT(cvW != NULL);
  KASSERT(cars != NULL);

  lock_acquire(lock);

    car c = {
      .id = vId++,
      .origin = origin
    };
    array_add(cars, &c, NULL);

    if (origin == 0) {
      while (!n) cv_wait(cvN, lock);
      e = false; s = false; w = false;
    }
    
    else if (origin == 1) {
      while (!e) cv_wait(cvE, lock);
      n = false; s = false; w = false;
    }

    else if (origin == 2) {
      while (!s) cv_wait(cvS, lock);
      n = false; e = false; w = false;
    }

    else if (origin == 3) {
      while(!w) cv_wait(cvW, lock);
      n = false; e = false; s = false;
    }
    intersectionCount++;

    for(unsigned int i = 0; i < array_num(cars); i++) {
      int id = ((car*)array_get(cars, i))->id;
      if (c.id == id) {
        array_remove(cars, i);
        break;
      }
    }

    if (++count > 10) {
      n = false; e = false; s = false; w = false;
      count = 0;
    }
  lock_release(lock);
}


/*
 * The simulation driver will call this function each time a vehicle
 * leaves the intersection.
 *
 * parameters:
 *    * origin: the Direction from which the vehicle arrived
 *    * destination: the Direction in which the vehicle is going
 *
 * return value: none
 */

void
intersection_after_exit(Direction origin, Direction destination) 
{
  (void)destination; /* avoid compiler complaint about unused parameter */

  KASSERT(lock != NULL);
  KASSERT(cvN != NULL);
  KASSERT(cvE != NULL);
  KASSERT(cvS != NULL);
  KASSERT(cvW != NULL);
  KASSERT(cars != NULL);

  lock_acquire(lock);
    intersectionCount--;
    if (intersectionCount == 0) {
      if (origin == 0) {
        n = false;
        if (array_num(cars) > 0) {
          Direction origin = ((car*)array_get(cars, 0))->origin;
          switch(origin) {
            case 0:
              n = true;
              cv_broadcast(cvN, lock);
              break;
            case 1:
              e = true;
              cv_broadcast(cvE, lock);
              break;
            case 2:
              s = true;
              cv_broadcast(cvS, lock);
              break;
            case 3:
              w = true;
              cv_broadcast(cvW, lock);
              break;
            default:
              panic("Origin = 0 error");
          }
        }
        else {
          n = true; e = true; s = true; w = true;
        }
      }

      else if (origin == 1) {
        e = false;
        if (array_num(cars) > 0) {
          Direction origin = ((car*)array_get(cars, 0))->origin;
          switch(origin) {
            case 0:
              n = true;
              cv_broadcast(cvN, lock);
              break;
            case 1:
              e = true;
              cv_broadcast(cvE, lock);
              break;
            case 2:
              s = true;
              cv_broadcast(cvS, lock);
              break;
            case 3:
              w = true;
              cv_broadcast(cvW, lock);
              break;
            default:
              panic("Origin = 1 error");
          }
        }
        else {
          n = true; e = true; s = true; w = true;
        }
      }

      else if (origin == 2) {
        s = false;
        if (array_num(cars) > 0) {
          Direction origin = ((car*)array_get(cars, 0))->origin;
          switch(origin) {
            case 0:
              n = true;
              cv_broadcast(cvN, lock);
              break;
            case 1:
              e = true;
              cv_broadcast(cvE, lock);
              break;
            case 2:
              s = true;
              cv_broadcast(cvS, lock);
              break;
            case 3:
              w = true;
              cv_broadcast(cvW, lock);
              break;
            default:
              panic("Origin = 2 error");
          }
        }
        else {
          n = true; e = true; s = true; w = true;
        }
      }

      else if (origin == 3) {
        w = false;
        if (array_num(cars) > 0) {
          Direction origin = ((car*)array_get(cars, 0))->origin;
          switch(origin) {
            case 0:
              n = true;
              cv_broadcast(cvN, lock);
              break;
            case 1:
              e = true;
              cv_broadcast(cvE, lock);
              break;
            case 2:
              s = true;
              cv_broadcast(cvS, lock);
              break;
            case 3:
              w = true;
              cv_broadcast(cvW, lock);
              break;
            default:
              panic("Origin = 3 error");
          }
        }
        else {
          n = true; e = true; s = true; w = true;
        }
      }
    }
  lock_release(lock);
}
