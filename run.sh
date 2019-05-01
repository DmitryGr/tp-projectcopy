#!/bin/bash

sudo cmake CMakeLists.txt
sudo make

g++ tests.cpp -std=c++11 -o tests -lgtest -lpthread
g++ main.cpp -std=c++11 -o main

./tests
./main
