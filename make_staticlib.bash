#!/usr/bin/bash

# make sure you've run make first
cd ./src
ar -crs libqtree.a qtree.o aabb.o
cd ..
sudo rm -f ./lib_files/libqtree.a
mv ./src/libqtree.a ./lib_files
make clean