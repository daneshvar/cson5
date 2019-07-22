#!/bin/bash
gcc -std=c11 ../json5.c main.c -I../ -lgomp -o json5
exitCode=$?
if [[ $exitCode == 0 ]]; then
  ./json5
fi
