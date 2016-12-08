#!/bin/bash

#g++ -std=c++11 new_planar.cc -o out -g
g++ -std=c++11 parallel.cc -o out -g -fopenmp
g++ -std=c++11 other.cc -o innn -g -fopenmp

echo "thinking..."
time out > output.txt
echo " done "
time cat output.txt | innn 
