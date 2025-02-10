#!/bin/bash

export DIR=~/goinfre/.kfs
export PREFIX="$DIR/local"
export SRC="$DIR/src"
export BIN_DIR=~/sgoinfre/.kfs

printf '\nThe following directory will be used :\n'
printf '%s\n' "$DIR"
while true; do
   read -r -p "Continue ? (y/n): " choice
   case "$choice" in
        y|Y ) break;;
        n|N ) exit 0;;
        * ) echo "";;
    esac
done

export TARGET="i386-elf"
export GCC_VERSION=13.3.0 # 14.2.0 needs meson
export BINUTILS_VERSION=2.43.1
export GDB_VERSION=16.1

mkdir -p "$DIR"/src
mkdir -p "$DIR"/local/bin

#Binutils
cd "$SRC" || exit
wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz
tar -xf binutils-$BINUTILS_VERSION.tar.xz
mkdir -p build-binutils
cd build-binutils || exit
# --disable-nls : no language support. Optional but reduces dependencies and compile time.
# --with-sysroot : enable sysroot support in the cross-compiler by pointing it to a default empty directory. By default, the linker refuses to use sysroots for no good technical reason, while gcc is able to handle both cases at runtime.
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror
if [[ "$(uname)" == "Linux" ]]; then
    make -j"$(nproc)"
elif [[ "$(uname)" == "Darwin" ]]; then
    make -j"$(sysctl -n hw.physicalcpu)"
fi
make install

#GDB
cd "$SRC" || exit
wget https://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.xz
tar -xf gdb-$GDB_VERSION.tar.xz
mkdir -p build-gdb
cd build-gdb || exit
../gdb-$GDB_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-werror \
    --with-gmp="$PREFIX" --with-mpfr="$PREFIX"
if [[ "$(uname)" == "Linux" ]]; then
    make -j"$(nproc)" all-gdb
elif [[ "$(uname)" == "Darwin" ]]; then
    make -j"$(sysctl -n hw.physicalcpu)" all-gdb
fi
make install-gdb

#GCC
cd "$SRC" || exit
wget https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz
tar -xf gcc-$GCC_VERSION.tar.xz
export PATH="$PREFIX/bin:$PATH"
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir -p build-gcc
cd build-gcc || exit
# --disable-nls : see binutils above
# --without-headers :  don't rely on any C library (standard or runtime) being present for the target.
# --enable-languages : don't compile all the other language frontends it supports, but only C/C++. 
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls \
    --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx \
    --with-gmp="$PREFIX" --with-mpfr="$PREFIX" --with-mpc="$PREFIX" --with-isl="$PREFIX"
if [[ "$(uname)" == "Linux" ]]; then
    make -j"$(nproc)" all-gcc
    make -j"$(nproc)" all-target-libgcc
    make -j"$(nproc)" all-target-libstdc++-v3
elif [[ "$(uname)" == "Darwin" ]]; then
    make -j"$(sysctl -n hw.physicalcpu)" all-gcc
    make -j"$(sysctl -n hw.physicalcpu)" all-target-libgcc
    make -j"$(sysctl -n hw.physicalcpu)" all-target-libstdc++-v3
fi
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

#rm "$SRC"/*
printf '\n\nInstalled in %s\n\n' "$DIR"

printf "\Copying binaries to : \n%s\n" "$BIN_DIR"
while true; do
   read -r -p "Continue ? (y/n): " choice
   case "$choice" in
        y|Y ) break;;
        n|N ) exit 0;;
        * ) echo "";;
    esac
done

mkdir -p $BIN_DIR
cp -r $DIR/local/* $BIN_DIR

printf "Add gcc, ld to PATH (via ~/.zshenv)? "
while true; do
   read -r -p "(y/n): " choice
   case "$choice" in
        y|Y ) break;;
        n|N ) exit 0;;
        * ) echo "";;
    esac
done

echo "export CX=$TARGET-gcc" >> ~/.zshenv
echo "export LDX=$TARGET-ld" >> ~/.zshenv
echo "export PATH="\"$BIN_DIR/bin:'$PATH'\""" >> ~/.zshenv

printf "\n%s, %s have been added to PATH (\$CX, \$LDX)\n\n" "$CX" "$LDX"