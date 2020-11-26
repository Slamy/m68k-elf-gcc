#!/bin/bash

TARGETARCHITECTURE=mipsel-unknown-elf

# Configure your path
HOSTINSTALLPATH="/opt/mipsel-unknown-elf.slamy"

# Configure as much as you like
MAKE_JOBS=${nproc}

if [ ! -d "$HOSTINSTALLPATH" ]; then
	echo Please create $HOSTINSTALLPATH and give the current user write rights to it
	exit 1
fi

set -e
export PATH=$PATH:$HOSTINSTALLPATH/bin/

function fetchAndExtract()
{
	url=$1
	file=$(echo $url | sed -E 's#.*/(.*)#\1#')
	folder=$2
	
	if [ ! -d "$folder" ]; then
		wget $url
		tar xf $file
	fi
}

function fetch_archives ()
{
	fetchAndExtract https://ftp.gnu.org/gnu/binutils/binutils-2.31.tar.gz binutils-2.31
	fetchAndExtract ftp://ftp.fu-berlin.de/unix/languages/gcc/releases/gcc-7.4.0/gcc-7.4.0.tar.gz gcc-7.4.0
}

function build_binutils () {
	echo ---------------------- Building binutils -------------------------
	
	cd binutils-2.31
	mkdir -p build
	cd build
	
	../configure \
		--target=$TARGETARCHITECTURE \
		--prefix=$HOSTINSTALLPATH/ \
		--with-float=soft
	
	make -j$MAKE_JOBS
	make install
	cd ../..
}

function build_final_gcc () {
	echo ---------------------- Building bootstrap GCC -------------------------
	
	cd gcc-7.4.0/
	mkdir -p build
	cd build
	
	../configure \
		--target=$TARGETARCHITECTURE \
		--prefix=$HOSTINSTALLPATH \
		--disable-nls \
		--disable-libada \
		--disable-libssp \
		--disable-libquadmath \
		--disable-libstdc++-v3 \
		--with-float=soft \
		--enable-languages=c,c++ \
		--with-gnu-as \
		--with-gnu-ld
		
	#build only gcc
	make -j$MAKE_JOBS
	make install
	cd ../..
}


fetch_archives
build_binutils
build_final_gcc

