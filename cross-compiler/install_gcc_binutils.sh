export GCC_VERSION=14.2.0
export BINUTILS_VERSION=2.43.1
export 

export DIR=$HOME/Documents/KFS/KFS-1/cross-compiler/
export PREFIX="$DIR/local"
export TARGET="i386-elf"
export PATH="$PREFIX/bin:$PATH"

cd $DIR/src

wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz
wget https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz
#wget https://ftp.gnu.org/gnu/gdb/gdb-16.1.tar.xz

cd $DIR
mkdir -p build-binutils && cd build-binutils

$DIR/src/binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix=$PREFIX \
    --with-sysroot --disable-nls --disable-werror
make
make install
	
