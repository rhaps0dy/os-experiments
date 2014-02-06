#!/bin/bash

#file: benchmarkRowThreads.sh

if [ $# != 1 ]; then
	echo "Usage: $0 <out_file>"
	exit
fi

#clean the file without having errors
touch $1
rm $1

echo "Compiling programs..."
gcc -Wall -Werror -pedantic -O3 -o matrixMult matrixMult.c -lpthread
gcc -O3 -o writeMatrix writeMatrix.c

echo "Generating input files..."

i=1
while [ $i -le 50 ]; do
	./writeMatrix $i $i inp.$i
	let i=i+1
done

echo "Benchmarking squaring of matrix..."

i=1
while [ $i -le 50 ]; do
	echo "Matrix of ${i}x$i" >> $1
	/usr/bin/time -f "%E %M" ./matrixMult inp.$i inp.$i out.$i 2>> $1

	let i=i+1
done

echo "Cleaning up..."
rm writeMatrix matrixMult

i=1
while [ $i -le 50 ]; do
	rm inp.$i out.$i
	
	let i=i+1
done
