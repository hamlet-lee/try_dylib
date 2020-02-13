#!/bin/sh
# http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
javac HelloWorld.java
javah HelloWorld
cargo build
