#!/bin/bash

#file: benchmark.sh

if [ $# != 1 ]; then
	echo "Usage: $0 <out_file>"
	exit
fi

#clean the file without having errors
touch $1
rm $1

echo "Compiling programs..."
gcc -Wall -Werror -pedantic -O3 -o RWNumsCLIB RWNumsCLIB.c
gcc -O3 -o WriteRandNums WriteRandNums.c
javac RWNums.java

echo "Generating input files..."

i=100
while [ $i -le 1000000 ]; do
	./WriteRandNums in.$i $i
	let i=i*10
done

echo "Benchmarking CLIB program..."

i=100
while [ $i -le 1000000 ]; do
	echo "CLIB performance for $i numbers:" >> $1
	/usr/bin/time -f "%E %M" ./RWNumsCLIB in.$i outC.$i 12.5 2>> $1

	let i=i*10
done

echo "Benchmarking Java program..."
echo "" >> $1

i=100
while [ $i -le 1000000 ]; do
	echo "Java performance for $i numbers:" >> $1
	/usr/bin/time -f "%E %M" java RWNums in.$i outJava.$i 12.5 2>> $1

	let i=i*10
done

echo "Benchmarking Python script..."
i=100
while [ $i -le 1000000 ]; do
	echo "Python performance for $i numbers:" >> $1
	/usr/bin/time -f "%E %M" ./RWNums.py in.$i outPython.$i 12.5 2>> $1

	let i=i*10
done

echo "Cleaning up..."
rm RWNums.class RWNumsCLIB WriteRandNums 

i=100
while [ $i -le 1000000 ]; do
	rm in.$i outC.$i outJava.$i outPython.$i
	
	let i=i*10
done
