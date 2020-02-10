#!/bin/sh
# http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
g++ -g -ldl app.cpp
gcc -g -Wall -fPIC -c  caller.cpp
gcc -shared -Wl,-soname,caller.so.1 -o caller.so caller.o
