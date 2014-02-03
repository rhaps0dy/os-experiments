#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readSplit( FILE *fin, char* buff, char s, int maxlen ) {
	int i;
	for(i=0; i<maxlen-1; i++)
	{
		int res = getc(fin);
		if(res==EOF)
		{
			buff[i+1]='\0';
			return -1;
		}
		if(((char)res)==s)
		{
			buff[i+1]='\0';
			return 0;
		}
		buff[i] = (char)res;
	}
	buff[maxlen-1] = '\0';
	return -1;
}

int checkStringIsNumber(char *str)
{
	int i;
	for(i=0; str[i]!='\0'; i++)
	{
		if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '.'))
		{
			return -1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int numvalid, res;
	double num, n;
	FILE *rfile, *wfile;
	char buff[100] = {0};

	if(argc!=4)
	{
		printf("Usage: ./program infile outfile num\n");
		return 0;
	}

	numvalid = checkStringIsNumber(argv[3]);
	if(numvalid==-1)
	{
		printf("Invalid number %s\n", argv[3]);
		return -1;
	}
	num = atof(argv[3]);

	rfile = fopen(argv[1], "r");
	if(rfile == NULL)
	{
		printf("Invalid input file %s\n", argv[1]);
		return -1;
	}
	wfile = fopen(argv[2], "w");
	if(wfile == NULL)
	{
		printf("Invalid output file %s\n", argv[2]);
		fclose(rfile);
		return -1;
	}

	/* all input valid, proceed */

	res = 0;
	while(res != -1) {
		res=readSplit(rfile, buff, ' ', 100);
		numvalid=checkStringIsNumber(buff);
		if(numvalid==-1)
		{
			printf("Invalid number in input file %s\n", argv[1]);
			fclose(rfile);
			fclose(wfile);
			return 0;
		}
		n = atof(buff);
		if(fprintf(wfile, "%f ", n*num)<0)
		{
			printf("Could not write to file %s\n", argv[2]);
			fclose(rfile);
			fclose(wfile);
		}
	}
	
	fclose(rfile);
	fclose(wfile);
	return 0;
}
