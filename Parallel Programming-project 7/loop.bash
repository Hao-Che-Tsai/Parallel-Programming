#!/bin/bash
mpic++ fourier.cpp -o fourier  -lm
mpiexec -np 1 fourier
mpic++ fourier.cpp -o fourier  -lm
mpiexec -np 2 fourier
mpic++ fourier.cpp -o fourier  -lm
mpiexec -np 4 fourier
mpic++ fourier.cpp -o fourier  -lm
mpiexec -np 8 fourier