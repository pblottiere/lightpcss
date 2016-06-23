#! /bin/sh

git clone https://github.com/etr/libhttpserver
cd libhttpserver
./bootstrap
mkdir build
cd build
../configure
make
sudo make install
