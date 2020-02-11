#!/bin/sh
# http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
g++ -g -ldl app.cpp
gcc -g -Wall -fPIC -c  caller.cpp

gcc -shared -Wl,-soname,caller.so.1 -o caller.so caller.o
gcc -g -Wall -fPIC -c  f1.cpp
gcc -shared -Wl,-soname,f1.so.1 -o f1.so f1.o

gcc -g -Wall -fPIC -c  f2.cpp
gcc -shared -Wl,-soname,f2.so.1 -o f2.so f2.o


