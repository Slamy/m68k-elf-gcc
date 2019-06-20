TARGETARCHITECTURE=m68k-elf
HOSTINSTALLPATH="/opt/m68k-elf.slamy/"
MAKE_JOBS=4

set -e
export PATH=$PATH:$HOSTINSTALLPATH/bin/

function fetch_archives ()
{
	wget https://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.gz
	tar xf binutils-2.32.tar.gz
	
	wget ftp://ftp.fu-berlin.de/unix/languages/gcc/releases/gcc-9.1.0/gcc-9.1.0.tar.gz
	tar xf gcc-9.1.0.tar.gz
	
	wget ftp://sourceware.org/pub/newlib/newlib-3.1.0.tar.gz
	tar xf newlib-3.1.0.tar.gz
	
}

function build_binutils () {
	cd binutils-2.32
	mkdir build
	cd build
	
	BINUTILSFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/" 
	../configure $BINUTILSFLAGS
	make -j$MAKE_JOBS
	make install
	cd ..
}

# --disable-bootstrap						Dont test the GCC as the Target is not the Host
# --disable-newlib-io-float				Disable printf/scanf family float support.
# --enable-newlib-nano-formatted-io		This builds NEWLIB with a special implementation of formatted I/O functions, 
										Designed to lower the size of application on small systems
# --disable-multilib						Do NOT generate for every library version. Only 68000 is needed here
# --enable-cxx-flags='...'				Custom flags for compiling libstdc++. In this case optimizing for size.	
# --disable-libssp						Disable stack smash protection
# --disable-libgomp						GNU Offloading and Multi Processing Runtime Library

function build_bootstrap_gcc () {
	cd gcc-9.1.0/
	mkdir build
	cd build
	
	../configure \
		--target=$TARGETARCHITECTURE \
		--prefix=$HOSTINSTALLPATH \
		--with-cpu=m68000 \
		--enable-languages=c,c++ \
		--disable-bootstrap \
		--with-newlib \
		--disable-libmudflap \
		--enable-lto \
		--disable-libssp \
		--disable-libgomp \
		--disable-threads \
		--with-gnu-as \
		--with-gnu-ld \
		--disable-nls \
		--disable-libstdcxx-threads \
		--disable-multilib \
		--enable-cxx-flags='-fomit-frame-pointer -ffunction-sections -fno-exceptions'
		
	#build only gcc
	make -j$MAKE_JOBS all-gcc
	make install-gcc
	cd ..
}

function build_newlib () {
	cd  newlib-3.1.0/
	mkdir build
	cd build
	
	../configure \
		--target=$TARGETARCHITECTURE \
		--prefix=$HOSTINSTALLPATH \
		--with-cpu=m68000 \
		--disable-multilib \
		--enable-newlib-nano-formatted-io \
		--disable-newlib-io-float \
		--disable-newlib-supplied-syscalls \
		--enable-languages=c,c++
		
	make -j$MAKE_JOBS
	make install
	cd ..
}

function build_final_gcc () {
	cd gcc-9.1.0/build
	make -j$MAKE_JOBS
	make install
}


fetch_archives
build_binutils
build_bootstrap_gcc
build_final_gcc


#JSR $00005152
#JSR $00009a98

# build only libstdc++
# make all-target-libstdc++-v3
# make install-target-libstdc++-v3
