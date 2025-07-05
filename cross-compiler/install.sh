#!/bin/bash
set -e -o pipefail

TARGET="i386-elf"

GMP_VERSION=6.3.0
GMP_FILENAME="gmp-$GMP_VERSION.tar.xz"
GMP_BASEURL="https://gmplib.org/download/gmp/$GMP_FILENAME"
MPFR_VERSION=4.2.1
MPFR_FILENAME="mpfr-$MPFR_VERSION.tar.xz"
MPFR_BASEURL="https://www.mpfr.org/mpfr-$MPFR_VERSION/$MPFR_FILENAME"
MPC_VERSION=1.2.1
MPC_FILENAME="mpc-$MPC_VERSION.tar.gz"
MPC_BASEURL="https://ftp.gnu.org/gnu/mpc/$MPC_FILENAME"
ISL_VERSION=0.27
ISL_FILENAME="isl-$ISL_VERSION.tar.xz"
ISL_BASEURL="https://libisl.sourceforge.io/$ISL_FILENAME"
BISON_VERSION=3.8.2
BISON_FILENAME="bison-$BISON_VERSION.tar.xz"
BISON_BASEURL="https://ftp.gnu.org/gnu/bison/$BISON_FILENAME"
FLEX_VERSION=2.6.4
FLEX_FILENAME="flex-$FLEX_VERSION.tar.gz"
FLEX_BASEURL="https://github.com/westes/flex/releases/download/v$FLEX_VERSION/$FLEX_FILENAME"
TEXINFO_VERSION=7.2
TEXINFO_FILENAME="texinfo-$TEXINFO_VERSION.tar.xz"
TEXINFO_BASEURL="https://ftp.gnu.org/gnu/texinfo/$TEXINFO_FILENAME"

GCC_VERSION=13.3.0 # < 14.2.0 needs meson
GCC_FILENAME="gcc-$GCC_VERSION.tar.xz"
GCC_BASEURL="https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/$GCC_FILENAME"
BINUTILS_VERSION=2.43.1
BINUTILS_FILENAME="binutils-$BINUTILS_VERSION.tar.xz"
BINUTILS_BASEURL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILENAME"
GDB_VERSION=16.1
GDB_FILENAME="gdb-$GDB_VERSION.tar.xz"
GDB_BASEURL="https://ftp.gnu.org/gnu/gdb/$GDB_FILENAME"

KFS_DIR="$(pwd)"
PREFIX_DIR="$KFS_DIR/kfs"
BUILD_DIR="/tmp/kfs"
mkdir -p "$BUILD_DIR"

if [[ "$1" == "--rebuild" ]]; then
	REBUILD=1
else
	REBUILD=0
fi

if [[ "$(uname)" == "Linux" ]]; then
	MAKE_JOBS="$(nproc)"
elif [[ "$(uname)" == "Darwin" ]]; then
	MAKE_JOBS="$(sysctl -n hw.physicalcpu)"
fi

# GMP
if [[ -d "$BUILD_DIR/$GMP_FILENAME" ]]; then
	echo "GMP already downloaded."
else
	wget -c "$GMP_BASEURL" -O "$BUILD_DIR/$GMP_FILENAME"
fi
tar -xf "$BUILD_DIR/$GMP_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/gmp-$GMP_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/gmp-$GMP_VERSION/build"
fi
mkdir -p "$BUILD_DIR/gmp-$GMP_VERSION/build"
cd "$BUILD_DIR/gmp-$GMP_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR" --enable-cxx
fi
make -j"$MAKE_JOBS"
make install

# MPFR
if [[ -d "$BUILD_DIR/$MPFR_FILENAME" ]]; then
	echo "MPFR already downloaded."
else
	wget -c "$MPFR_BASEURL" -O "$BUILD_DIR/$MPFR_FILENAME"
fi
tar -xf "$BUILD_DIR/$MPFR_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/mpfr-$MPFR_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/mpfr-$MPFR_VERSION/build"
fi
mkdir -p "$BUILD_DIR/mpfr-$MPFR_VERSION/build"
cd "$BUILD_DIR/mpfr-$MPFR_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR" --with-gmp="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# MPC
if [[ -d "$BUILD_DIR/$MPC_FILENAME" ]]; then
	echo "MPC already downloaded."
else
	wget -c "$MPC_BASEURL" -O "$BUILD_DIR/$MPC_FILENAME"
fi
tar -xf "$BUILD_DIR/$MPC_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/mpc-$MPC_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/mpc-$MPC_VERSION/build"
fi
mkdir -p "$BUILD_DIR/mpc-$MPC_VERSION/build"
cd "$BUILD_DIR/mpc-$MPC_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR" --with-gmp="$PREFIX_DIR" --with-mpfr="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# ISL
if [[ -d "$BUILD_DIR/$ISL_FILENAME" ]]; then
	echo "ISL already downloaded."
else
	wget -c "$ISL_BASEURL" -O "$BUILD_DIR/$ISL_FILENAME"
fi
tar -xf "$BUILD_DIR/$ISL_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/isl-$ISL_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/isl-$ISL_VERSION/build"
fi
mkdir -p "$BUILD_DIR/isl-$ISL_VERSION/build"
cd "$BUILD_DIR/isl-$ISL_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# Bison
if [[ -d "$BUILD_DIR/$BISON_FILENAME" ]]; then
	echo "Bison already downloaded."
else
	wget -c "$BISON_BASEURL" -O "$BUILD_DIR/$BISON_FILENAME"
fi
tar -xf "$BUILD_DIR/$BISON_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/bison-$BISON_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/bison-$BISON_VERSION/build"
fi
mkdir -p "$BUILD_DIR/bison-$BISON_VERSION/build"
cd "$BUILD_DIR/bison-$BISON_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# Flex
if [[ -d "$BUILD_DIR/$FLEX_FILENAME" ]]; then
	echo "Flex already downloaded."
else
	wget -c "$FLEX_BASEURL" -O "$BUILD_DIR/$FLEX_FILENAME"
fi
tar -xf "$BUILD_DIR/$FLEX_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/flex-$FLEX_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/flex-$FLEX_VERSION/build"
fi
mkdir -p "$BUILD_DIR/flex-$FLEX_VERSION/build"
cd "$BUILD_DIR/flex-$FLEX_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# Texinfo
if [[ -d "$BUILD_DIR/$TEXINFO_FILENAME" ]]; then
	echo "Texinfo already downloaded."
else
	wget -c "$TEXINFO_BASEURL" -O "$BUILD_DIR/$TEXINFO_FILENAME"
fi
tar -xf "$BUILD_DIR/$TEXINFO_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/texinfo-$TEXINFO_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/texinfo-$TEXINFO_VERSION/build"
fi
mkdir -p "$BUILD_DIR/texinfo-$TEXINFO_VERSION/build"
cd "$BUILD_DIR/texinfo-$TEXINFO_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --prefix="$PREFIX_DIR"
fi
make -j"$MAKE_JOBS"
make install

# Binutils
if [[ -d "$BUILD_DIR/$BINUTILS_FILENAME" ]]; then
	echo "Binutils already downloaded."
else
	wget -c "$BINUTILS_BASEURL" -O "$BUILD_DIR/$BINUTILS_FILENAME"
fi
tar -xf "$BUILD_DIR/$BINUTILS_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/binutils-$BINUTILS_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/binutils-$BINUTILS_VERSION/build"
fi
mkdir -p "$BUILD_DIR/binutils-$BINUTILS_VERSION/build"
cd "$BUILD_DIR/binutils-$BINUTILS_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --target="$TARGET" --prefix="$PREFIX_DIR" --with-sysroot --disable-nls --disable-werror
fi
make -j"$MAKE_JOBS"
make install

# GDB
if [[ -d "$BUILD_DIR/$GDB_FILENAME" ]]; then
	echo "GDB already downloaded."
else
	wget -c "$GDB_BASEURL" -O "$BUILD_DIR/$GDB_FILENAME"
fi
tar -xf "$BUILD_DIR/$GDB_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/gdb-$GDB_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/gdb-$GDB_VERSION/build"
fi
mkdir -p "$BUILD_DIR/gdb-$GDB_VERSION/build"
cd "$BUILD_DIR/gdb-$GDB_VERSION/build" || exit 1
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --target="$TARGET" --prefix="$PREFIX_DIR" --with-gmp="$PREFIX_DIR" --with-mpfr="$PREFIX_DIR" --disable-werror
fi
make -j"$MAKE_JOBS" all-gdb
make install-gdb

# GCC
if [[ -d "$BUILD_DIR/$GCC_FILENAME" ]]; then
	echo "GCC already downloaded."
else
	wget -c "$GCC_BASEURL" -O "$BUILD_DIR/$GCC_FILENAME"
fi
tar -xf "$BUILD_DIR/$GCC_FILENAME" -C "$BUILD_DIR"
if [ -d "$BUILD_DIR/gcc-$GCC_VERSION/build" ]; then
	EXISTING=1
else
	EXISTING=0
fi
if [ "$EXISTING" = "1" ] && [ "$REBUILD" = "1" ]; then
	rm -rf "$BUILD_DIR/gcc-$GCC_VERSION/build"
fi
mkdir -p "$BUILD_DIR/gcc-$GCC_VERSION/build"
cd "$BUILD_DIR/gcc-$GCC_VERSION/build" || exit 1
export PATH="$PREFIX_DIR/bin:$PATH"
which -- "$TARGET-as" || { echo "$TARGET-as is not in PATH"; exit 1; }
if [ "$EXISTING" = "0" ] || [ "$REBUILD" = "1" ]; then
	../configure --target="$TARGET" --prefix="$PREFIX_DIR" \
		--with-gmp="$PREFIX_DIR" --with-mpfr="$PREFIX_DIR" --with-mpc="$PREFIX_DIR" --with-isl="$PREFIX_DIR" \
		--enable-languages=c,c++ --disable-nls --without-headers --disable-hosted-libstdcxx
fi
make -j"$MAKE_JOBS" all-gcc all-target-libgcc all-target-libstdc++-v3
make install-gcc install-target-libgcc install-target-libstdc++-v3

if which -- "$TARGET-gcc" &>/dev/null; then
    echo -e "\n# KFS cross-compiler toolchain\nexport PATH=\"\$PATH:$PREFIX_DIR/bin\"" >> ~/.zshprofile
fi

echo "Cross-compiler toolchain for $TARGET installed successfully in:\n$PREFIX_DIR."
