#!/bin/sh

echo Launching installation

cmake .
cmake --build .
cmake --install .

echo Cleaning directory..

rm CMakeCache.txt
rm -r CMakeFiles
rm cmake_install.cmake
rm Makefile
rm install_manifest.txt

echo Installation finished