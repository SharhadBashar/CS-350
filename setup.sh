#!/bin/bash

############################################
##                WARNING                 ##
############################################

# To use this file, $ source setup.sh

SUBMIT="2a" # Modify this for different assignments

export PATH; PATH=`/bin/showpath /u/cs350/sys161/bin /u/cs350/bin standard`

# Modify this if you installed os161 somewhere else
export CS350=$PWD

export ROOT=$CS350/root
export PROJ=$CS350/os161-1.99

ASSIGNMENT="ASST${SUBMIT:0:1}" # 2a -> ASST2, 3 -> ASST3

echo "Setting up CS350 for Assignment $SUBMIT"


############################################
##              Directories               ##
############################################

export B=$PROJ/build

export K=$PROJ/kern
export KA=$K/arch
export KAM=$KA/mips
export KAMI=$KAM/include
export KAML=$KAM/locore
export KAMT=$KAM/thread
export KAMS=$KAM/syscall
export KAMV=$KAM/vm
export KC=$K/conf
export KD=$K/dev
export KF=$K/fs
export KI=$K/include
export KIK=$KI/kern
export KL=$K/lib
export KP=$K/proc
export KSu=$K/startup
export KSp=$K/synchprobs
export KSc=$K/syscall
export KT=$K/test
export KTh=$K/thread
export KVf=$K/vfs
export KVm=$K/vm

export M=$PROJ/man

export U=$PROJ/user
export UB=$U/bin
export UI=$U/include
export UL=$U/lib
export UM=$U/my-testbin
export UT=$U/testbin
export UW=$U/uw-testbin

export C="$K/compile/${ASSIGNMENT}"

## Configuration files
export ConfK=$KC/conf.kern
export ConfS=$ROOT/sys161.conf

pwd_abbr() {
	case $PWD in
		"$B/"*) echo "${PWD/#"$B/"/"\$B/"}";;
		"$B")   echo "\$B";;
		# $C is subdirectory of $K
		"$C/"*) echo "${PWD/#"$C/"/"\$C/"}";;
		"$C")   echo "\$C";;

		"$M/"*) echo "${PWD/#"$M/"/"\$M/"}";;
		"$M")   echo "\$M";;




		### Subdirectories of $K
		"$KAMI/"*) echo "${PWD/#"$KAMI/"/"\$KAMI/"}";;
		"$KAMI")   echo "\$KAMI";;
		"$KAML/"*) echo "${PWD/#"$KAML/"/"\$KAML/"}";;
		"$KAML")   echo "\$KAML";;
		"$KAMT/"*) echo "${PWD/#"$KAMT/"/"\$KAMT/"}";;
		"$KAMT")   echo "\$KAMT";;
		"$KAMS/"*) echo "${PWD/#"$KAMS/"/"\$KAMS/"}";;
		"$KAMS")   echo "\$KAMS";;
		"$KAMV/"*) echo "${PWD/#"$KAMV/"/"\$KAMV/"}";;
		"$KAMV")   echo "\$KAMV";;
		"$KAM/"*) echo "${PWD/#"$KAM/"/"\$KAM/"}";;
		"$KAM")   echo "\$KAM";;
		"$KA/"*) echo "${PWD/#"$KA/"/"\$KA/"}";;
		"$KA")   echo "\$KA";;

		"$KC/"*) echo "${PWD/#"$KC/"/"\$KC/"}";;
		"$KC")   echo "\$KC";;
		"$KC/"*) echo "${PWD/#"$KC/"/"\$KC/"}";;
		"$KC")   echo "\$KC";;
		"$KD/"*) echo "${PWD/#"$KD/"/"\$KD/"}";;
		"$KD")   echo "\$KD";;
		"$KF/"*) echo "${PWD/#"$KF/"/"\$KF/"}";;
		"$KF")   echo "\$KF";;

		"$KIK/"*) echo "${PWD/#"$KIK/"/"\$KIK/"}";;
		"$KIK")   echo "\$KIK";;
		"$KI/"*) echo "${PWD/#"$KI/"/"\$KI/"}";;
		"$KI")   echo "\$KI";;
		"$KL/"*) echo "${PWD/#"$KL/"/"\$KL/"}";;
		"$KL")   echo "\$KL";;
		"$KP/"*) echo "${PWD/#"$KP/"/"\$KP/"}";;
		"$KP")   echo "\$KP";;
		"$KSu/"*) echo "${PWD/#"$KSu/"/"\$KSu/"}";;
		"$KSu")   echo "\$KSu";;
		"$KSp/"*) echo "${PWD/#"$KSp/"/"\$KSp/"}";;
		"$KSp")   echo "\$KSp";;
		"$KSc/"*) echo "${PWD/#"$KSc/"/"\$KSc/"}";;
		"$KSc")   echo "\$KSc";;
		"$KT/"*) echo "${PWD/#"$KT/"/"\$KT/"}";;
		"$KT")   echo "\$KT";;
		"$KTh/"*) echo "${PWD/#"$KTh/"/"\$KTh/"}";;
		"$KTh")   echo "\$KTh";;
		"$KVf/"*) echo "${PWD/#"$KVf/"/"\$KVf/"}";;
		"$KVf")   echo "\$KVf";;
		"$KVm/"*) echo "${PWD/#"$KVm/"/"\$KVm/"}";;
		"$KVm")   echo "\$KVm";;
		"$K/"*) echo "${PWD/#"$K/"/"\$K/"}";;
		"$K")   echo "\$K";;




		### Subdirectories of $U
		"$UB/"*) echo "${PWD/#"$UB/"/"\$UB/"}";;
		"$UB")   echo "\$UB";;
		"$UI/"*) echo "${PWD/#"$UI/"/"\$UI/"}";;
		"$UI")   echo "\$UI";;
		"$UL/"*) echo "${PWD/#"$UL/"/"\$UL/"}";;
		"$UL")   echo "\$UL";;
		"$UM/"*) echo "${PWD/#"$UM/"/"\$UM/"}";;
		"$UM")   echo "\$UM";;
		"$UT/"*) echo "${PWD/#"$UT/"/"\$UT/"}";;
		"$UT")   echo "\$UT";;
		"$UW/"*) echo "${PWD/#"$UW/"/"\$UW/"}";;
		"$UW")   echo "\$UW";;
		"$U/"*) echo "${PWD/#"$U/"/"\$U/"}";;
		"$U")   echo "\$U";;



		### Other
		"$PROJ/"*) echo "${PWD/#"$PROJ/"/"\$PROJ/"}";;
		"$PROJ")   echo "\$PROJ";;

		"$ROOT/"*) echo "${PWD/#"$ROOT/"/"\$ROOT/"}";;
		"$ROOT")   echo "\$ROOT";;

		# Home is a special case
		"$HOME/"*) echo "${PWD/#"$HOME/"/"~/"}";;
		"$HOME")   echo "~";;

		# All other directories are printed in full
		*)         echo "$PWD";;
	esac
}

directories() {
	echo "\$K:kern"
	echo "	\$KA:arch"
	echo "		\$KAM:mips"
	echo "			\$KAMI:include"
	echo "	\$KC:conf"
	echo "	\$KD:dev"
	echo "	\$KF:fs"
	echo "	\$KI:include"
	echo "	\$KL:lib"
	echo "	\$KP:proc"
	echo "	\$KSu:startup"
	echo "	\$KSp:synchprobs"
	echo "	\$KSc:syscall"
	echo "	\$KT:test"
	echo "	\$KTh:thread"
	echo "	\$KVf:vfs"
	echo "	\$KVm:vm"
	echo "	\$C:compile/ASST$ASSIGNMENT"
}


#
# Contract directory names.
#
# e.g. ~/cs350-os161/os161-1.99/kern/arch/mips/include
#         becomes
#      $KAMI
#
# or   ~/cs350-os161/os161-1.99/user/my-testbin/simplefork
#         becomes
#      $UM/simplefork
#
ps1_reset()
{
	local FRONT='['
	local USERNAME='\[\e[01;32m\]\u'
	local AT='\[\e[0m\]@'
	local LOCALHOST='\[\e[38;5;11m\]\h'
	local DIRECTORY='\[\e[38;5;105m\]$(pwd_abbr)'
	local BRANCH='\[\e[38;5;68m\]$(__git_ps1 " (%s)")'
	local BACK='\[\e[00m\]]\[\e[1m\]\$\[\e[00m\]'
	export PS1="${FRONT}${USERNAME}${AT}${LOCALHOST} ${DIRECTORY}${BRANCH}${BACK} "
}
ps1_reset





############################################
##        Helper bash commands            ##
############################################

# Obtain copy of OS/161 source code onto the current directory.
os161-setup() {
	tar -xzf /u/cs350/pub/os161_repository/os161.tar.gz
	pushd os161-1.99 > /dev/null
	./configure --ostree=$ROOT --toolprefix=cs350-
	popd > /dev/null
	mkdir root
	cp /u/cs350/sys161/sys161.conf root/sys161.conf
}



#
# Workflow:
#
# Each time switching to a new assignment
#
# 1. Commit the old assignment into version control.
# 2. Modify setup script
# 3. $ osconfig
#
# Testing:
#
# 1. $ osbuild; osinstall
# 2. $ userbuild                   # Optional, for 2a,2b,3
# 3a. $ osrun
# 3b. $ osdebugrun                 # Also $ osdebug on another instance.
#
# Submitting:
# $ ossubmit
#

osconfig() {
	pushd $KC > /dev/null
	./config $ASSIGNMENT
	popd > /dev/null
}

osbuild() {
	pushd $C > /dev/null

	bmake depend
	bmake
	# Remove old, out-of-date image so the user knows to reinstall.
	rm -f $ROOT/kernel-$ASSIGNMENT

	popd > /dev/null
}
userbuild() {
	# Setup symlinks if the directories do not exist.
	if [ ! -e $ROOT/testbin ] || [ ! -e $ROOT/my-testbin ] || [ ! -e $ROOT/uw-testbin ]
	then
		ln -s $B/install/testbin $ROOT/testbin
		ln -s $B/install/my-testbin $ROOT/my-testbin
		ln -s $B/install/uw-testbin $ROOT/uw-testbin
	fi

	pushd $PROJ > /dev/null
	bmake
	popd > /dev/null
}

osinstall() {
	pushd $C > /dev/null
	bmake install
	popd > /dev/null
}

osrun() {
	pushd $ROOT > /dev/null
	sys161 "kernel-$ASSIGNMENT" $1
	popd > /dev/null
}

#
# Usage:
#    userrun PROGRAM [FLAGS]
#
# e.g.
#    userrun my-testbin/simple
#    userrun uw-testbin/onefork dsc         # with syscall debug
#
userrun() {
	pushd $ROOT > /dev/null

	if [ -z "$2" ]
	then
		echo "Program=$1"
		echo "================================================="
		sys161 "kernel-$ASSIGNMENT" "p $1;q"
	else
		echo "Program=$1,	Flags='$2'"
		echo "================================================="
		sys161 "kernel-$ASSIGNMENT" "$2; p $1;q"
	fi

	popd > /dev/null
}


#
# To use the debugging script, have two instances of ssh log into the same
# machine (e.g. ubuntu1804-008).
#
# In one instance (Server),          $ osdebugrun
# Then in another instance (Client), $ osdebug
#
# If the socket connection is successful, the server will prompt that a
# debugger has been attached. It is very important that the server execs first.
# Now in the client:
#
# c: Continue execution
# b: Set breakpoint
# bt: Print backtrace
#
osdebugrun() {
	pushd $ROOT > /dev/null
	sys161 -w kernel # Wait to attach debugger
	popd > /dev/null
}
osdebug() {
	pushd $ROOT > /dev/null
	# Attach debugger
	cs350-gdb kernel \
		-ex "dir ../os161-1.99/kern/compile/$ASSIGNMENT" \
		-ex "target remote unix:.sockets/gdb"
	popd > /dev/null
}

#
# Submit the current assignment
#
ossubmit() {
	pushd $CS350 > /dev/null
	echo "Submitting $SUBMIT"
	rm -f os161kern.tgz
	/u/cs350/bin/cs350_submit $SUBMIT
	popd > /dev/null
}

