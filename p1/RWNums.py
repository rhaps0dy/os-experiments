#!/usr/bin/python3
import sys

if len(sys.argv)!=4:
	print("Usage: %s infile outfile num"%sys.argv[0])
	sys.exit(0)

infile = open(sys.argv[1], "r")
outfile = open(sys.argv[2], "w")
num = float(sys.argv[3])

cont = True
while cont:
    curnum = ""
    c = infile.read(1)
    while c!=" " and c!="":
        curnum+=c
        c = infile.read(1)
    if c=="":
        cont = False
    try:
        outnum = num*float(curnum)
        outfile.write("%f "%outnum)
    except:
        pass

infile.close()
outfile.close()
