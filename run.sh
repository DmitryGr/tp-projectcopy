#!/bin/bash

sudo cmake CMakeLists.txt
sudo make

g++ tests.cpp -std=c++14 -o tests -lgtest -lpthread
g++ main.cpp -std=c++14 -o main

./tests
./main
