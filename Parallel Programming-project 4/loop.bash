#!/bin/bash
for t in 1000 5000 10000 50000 100000 250000 500000 1000000 2000000 4000000 8000000 
do
        g++  proj04.cpp  -DARRAYSIZE=$t  -o proj04  -lm  -fopenmp
    ./proj04
done
