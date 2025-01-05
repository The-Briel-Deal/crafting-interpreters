#!/bin/sh

mkdir -p build
gcc src/* -Iinclude -o build/clox
./build/clox
