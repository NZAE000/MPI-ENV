#!bin/bash

SRC=$1
NPROC=$2

make SRCPATH=${SRC} && mpirun -np ${NPROC} bin/exec
