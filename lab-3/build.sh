#!bin/bash

cd xmalloc
make clean
make
cd ..
mpicc -o ex main.c xmalloc/libxmalloc.a -Ixmalloc -lm
