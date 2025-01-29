export DIR=$HOME/Documents/KFS/KFS-1/cross-compiler/
mkdir -p $DIR/local
mkdir -p $DIR/src
export PREFIX="$DIR/local"
export SRC="$DIR/src"
export PATH="$PREFIX/bin:$PATH"
export LD_LIBRARY_PATH="$PREFIX/lib:$LD_LIBRARY_PATH"

#GMP
cd $SRC
wget https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
tar -xf gmp-6.3.0.tar.xz
cd gmp-6.3.0
./configure --prefix=$PREFIX --enable-cxx
make -j$(nproc)
make install

#MPFR
cd $SRC
wget https://www.mpfr.org/mpfr-current/mpfr-4.2.1.tar.xz
tar -xf mpfr-4.2.1.tar.xz
cd mpfr-4.2.1
./configure --prefix=$PREFIX --with-gmp=$PREFIX
make -j$(nproc)
make install

#MPC
cd $SRC
wget https://ftp.gnu.org/gnu/mpc/mpc-1.3.1.tar.gz
tar -xf mpc-1.3.1.tar.gz
cd mpc-1.3.1
./configure --prefix=$PREFIX --with-gmp=$PREFIX --with-mpfr=$PREFIX
make -j$(nproc)
make install

#ISL
cd $SRC
wget https://libisl.sourceforge.io/isl-0.27.tar.xz
tar -xf isl-0.27.tar.xz
cd isl-0.27
./configure --prefix=$PREFIX
make -j$(nproc)
make install

#BISON
cd $SRC
wget http://ftp.gnu.org/gnu/bison/bison-3.8.2.tar.xz
tar -xf bison-3.8.2.tar.xz
cd bison-3.8.2
./configure --prefix=$PREFIX
make -j$(nproc)
make install

#FLEX
cd $SRC
wget https://github.com/westes/flex/releases/download/v2.6.4/flex-2.6.4.tar.gz
tar -xf flex-2.6.4.tar.gz
cd flex-2.6.4
./configure --prefix=$PREFIX
make -j$(nproc)
make install

#TEXINFO
cd $SRC
wget http://ftp.gnu.org/gnu/texinfo/texinfo-7.2.tar.xz
tar -xf texinfo-7.2.tar.xz
cd texinfo-7.2
./configure --prefix=$PREFIX
make -j$(nproc)
make install

rm *.xz *.gz
