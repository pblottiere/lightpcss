#! /bin/sh

wget http://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.50.tar.gz
tar zxvf libmicrohttpd-0.9.50.tar.gz
cd libmicrohttpd-0.9.50
./configure
make
sudo make install
