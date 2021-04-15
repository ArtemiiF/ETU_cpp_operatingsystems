#!/bin/bash

g++ -c threads.cpp
g++ -o threads threads.o -lpthread
./threads


