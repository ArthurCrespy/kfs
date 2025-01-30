#!/bin/bash

export DIR=$HOME/Documents/KFS/KFS-1/cross-compiler/
export PREFIX="$DIR/local"
export SRC="$DIR/src"
export PATH="$PREFIX/bin:$PATH"

printf 'The following directory will be used :\n'
printf '%s\n' "$DIR"
while true; do
   read -r -p "Continue ? (y/n): " choice
   case "$choice" in
        y|Y ) break;;
        n|N ) exit 0;;
        * ) echo "";;
    esac
done

export GMP_VERSION=6.3.0
export MPFR_VERSION=4.2.1
export MPC_VERSION=1.3.1
export ISL_VERSION=0.27
export BISON_VERSION=3.8.2
export FLEX_VERSION=2.6.4
export TEXINFO_VERSION=7.2

mkdir -p "$DIR"/src
mkdir -p "$DIR"/local/bin

#GMP
cd "$SRC" || exit
wget https://gmplib.org/download/gmp/gmp-$GMP_VERSION.tar.xz
tar -xf gmp-$GMP_VERSION.tar.xz
cd gmp-$GMP_VERSION || exit
./configure --prefix="$PREFIX" --enable-cxx
make -j"$(nproc)"
make install

#MPFR
cd "$SRC" || exit
wget https://www.mpfr.org/mpfr-current/mpfr-$MPFR_VERSION.tar.xz
tar -xf mpfr-$MPFR_VERSION.tar.xz
cd mpfr-$MPFR_VERSION || exit
./configure --prefix="$PREFIX" --with-gmp="$PREFIX"
make -j"$(nproc)"
make install

#MPC
cd "$SRC" || exit
wget https://ftp.gnu.org/gnu/mpc/mpc-$MPC_VERSION.tar.gz
tar -xf mpc-$MPC_VERSION.tar.gz
cd mpc-$MPC_VERSION || exit
./configure --prefix="$PREFIX" --with-gmp="$PREFIX" --with-mpfr="$PREFIX"
make -j"$(nproc)"
make install

#ISL
cd "$SRC" || exit
wget https://libisl.sourceforge.io/isl-$ISL_VERSION.tar.xz
tar -xf isl-$ISL_VERSION.tar.xz
cd isl-$ISL_VERSION || exit
./configure --prefix="$PREFIX"
make -j"$(nproc)"
make install

#BISON
cd "$SRC" || exit
wget http://ftp.gnu.org/gnu/bison/bison-$BISON_VERSION.tar.xz
tar -xf bison-$BISON_VERSION.tar.xz
cd bison-$BISON_VERSION || exit
./configure --prefix="$PREFIX"
make -j"$(nproc)"
make install

#FLEX
cd "$SRC" || exit
wget https://github.com/westes/flex/releases/download/v$FLEX_VERSION/flex-$FLEX_VERSION.tar.gz
tar -xf flex-$FLEX_VERSION.tar.gz
cd flex-$FLEX_VERSION || exit
./configure --prefix="$PREFIX"
make -j"$(nproc)"
make install

#TEXINFO
cd "$SRC" || exit
wget http://ftp.gnu.org/gnu/texinfo/texinfo-$TEXINFO_VERSION.tar.xz
tar -xf texinfo-$TEXINFO_VERSION.tar.xz
cd texinfo-$TEXINFO_VERSION || exit
./configure --prefix="$PREFIX"
make -j"$(nproc)"
make install

rm "$SRC"/*.xz "$SRC"/*.gz
printf "Installed in %s\n\n" "$SRC"