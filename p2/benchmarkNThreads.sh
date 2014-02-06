#!/bin/bash

#file: benchmarkNThreads.sh

if [ $# != 1 ]; then
	echo "Usage: $0 <out_file>"
	exit
fi

#clean the file without having errors
touch $1
rm $1

echo "Compiling programs..."
gcc -Wall -Werror -pedantic -O3 -o matrixMultNThreads matrixMultNThreads.c -lpthread
gcc -O3 -o writeMatrix writeMatrix.c

echo "Generating input file..."
num=120
./writeMatrix $num $num matA

echo "Benchmarking squaring of matrix..."

i=1
while [ $i -le 50 ]; do
	/usr/bin/time -f "%E %M" ./matrixMultNThreads matA matA matC $i 2>> $1

	let i=i+1
done

echo "Cleaning up..."
rm writeMatrix matrixMultNThreads matA matC
