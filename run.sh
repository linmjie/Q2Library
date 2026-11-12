#!/usr/bin/env bash

cmake -B .
cmake --build .
./bin/"$1"
