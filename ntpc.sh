#!/bin/sh

#taskset -c 1 ./ntpc > ntpc.csv
taskset -c 1 ./spectre> ntpc.csv
./hist_spectre.py
