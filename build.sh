#!/bin/sh
# http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
javac App.java
javah App

g++ -g -ldl app.cpp

# https://www.jianshu.com/p/e175d5417e05
gcc -g -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.242.b08-0.el7_7.x86_64/include/ -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.242.b08-0.el7_7.x86_64/include/linux -Wall -fPIC -c  caller.cpp
# https://www.cnblogs.com/LiuYanYGZ/p/6110822.html
# Linux下：一定要将Linux下的共享库（我暂且这么叫:）命名成libxxx.so的形式，"xxx"是你在System.loadLibrary("xxx")中用到的加载库名称。
gcc -shared -Wl,-soname,libcaller.so.1 -o libcaller.so caller.o

gcc -g -Wall -fPIC -c  f1.cpp
gcc -shared -Wl,-soname,f1.so.1 -o f1.so f1.o

gcc -g -Wall -fPIC -c  f2.cpp
gcc -shared -Wl,-soname,f2.so.1 -o f2.so f2.o


