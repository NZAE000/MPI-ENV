#!bin/bash

SRC=$1
NPROC=$2

make SRCPATH=${SRC} && mpirun --oversubscribe -np ${NPROC} bin/exec
