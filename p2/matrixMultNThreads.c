#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* fork */
#include <pthread.h>   /* UNIX-like thread library */

/* global variables */
int An,Am,Bn,Bm,Cn,Cm;
int** A;
int** B;
int** C;

int nthreads;


void *multThread(void *param)
{
	int i, j, k;
	int n = * (int *) param;
	printf("Started thread %d\n", n);
	i=n%Cm;
	j=n/Cm;
	while(j<Cn)
	{
		C[j][i] = 0;
		for(k=0; k<Am; k++)
			C[j][i] += A[j][k]*B[k][i];
		i += nthreads;
		if(i>=Cm)
		{
			i = i%Cm;
			j++;
		}
	}
	return NULL;
}


int** createMatrix( int n, int m) {
    int i;
    int** mat = malloc( n * sizeof(int*) );
    for(i=0;i<n;i++) {
        mat[i] = malloc( m * sizeof(int) );
    }
    return mat;
}

void destroyMatrix(int **mat, int n)
{
	int i;
	for(i=0; i<n; i++)
		free(mat[i]);
	free(mat);
}

void fprintMatrix(FILE* f, int **mat, int n, int m)
{
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<m; j++)
			fprintf(f, "%d ", mat[i][j]);
		fprintf(f, "\n");
	}
}

int write_int_to_pipe(FILE *r, int w)
{
	/* valgrind gives uninitialized warning but it's not really bad */
	int num;
	fscanf(r, "%d", &num);
	write(w, &num, sizeof(int));
	if(feof(r))
		return -1;
	return 0;
}

int main(int argc, char* argv[])
{
    int i,j;
    int pid;
    FILE *f;

    int exit_code;

    pthread_t* tid;
    pthread_attr_t* attr;

    int fds[2][2];

    int* row;

    if(argc != 5) {
        printf("usage: %s A.txt B.txt C.txt nthreads\n", argv[0]);
        return -1;
    }

    nthreads = atoi(argv[4]);
 
    /* 1) Create two processes that will read A and B matrices
    /    Create also the two necessary pipes with associated file
    /    descriptors fd[0]
    /    with for example read fd[0][0] and fd[0][1] for writting */

    for(i=0;i<2;i++)
	    pipe(fds[i]);
    for(i=0;i<2;i++)
    {
	    pid = fork();
	    if(pid<0)
	    {
		    fprintf(stderr, "FORK FALLA\n");
		    return -1;
	    }
	    else if(pid==0)
		    break;
    }

    /* 2) Close unnecessary pipe channels fds[0][1], fds[1][1]
    /  childs read */
    if(i<2)
    {
	    close(fds[0][0]);
	    close(fds[1][0]);
	    close(fds[(i+1)%2][1]);

	    f = fopen(argv[i+1], "r");
	    while(write_int_to_pipe(f, fds[i][1])==0);
	    fclose(f);

	    close(fds[i][1]);
	    return 0;
    }
    else
    {
	    close(fds[0][1]);
	    close(fds[1][1]);
    }

    /* 3) read dimensions from pipes using read system call
        dimensions are directly ints */
    
    read(fds[0][0], &An, sizeof(int));
    read(fds[0][0], &Am, sizeof(int));
    read(fds[1][0], &Bn, sizeof(int));
    read(fds[1][0], &Bm, sizeof(int));

    /* 4) create the matrices using the supplied createMatrix */
    
    A = createMatrix(An, Am);
    B = createMatrix(Bn, Bm);

    /* 5) check if dimensions match, and set Cm, Cn dimensions */
    
    if(Am != Bn)
    {
	    printf("%d %d %d %d", An, Am, Bn, Bm);
	    fprintf(stderr, "Matrix dimensions do not match\n");
	    destroyMatrix(A, An);
	    destroyMatrix(B, Bn);
	    return -1;
    }
    Cn = An;
    Cm = Bm;
    C = createMatrix(Cn, Cm);
    printf("C is %dx%d\n",Cn,Cm);
    
    /* 6) read the two matrices from the two pipes */

    for(i=0; i<An; i++)
	    for(j=0; j<Am; j++)
		    read(fds[0][0], &A[i][j], sizeof(int));
    for(i=0; i<Bn; i++)
	    for(j=0; j<Bm; j++)
		    read(fds[1][0], &B[i][j], sizeof(int));

    close(fds[0][0]);
    close(fds[1][0]);
    /* 7) wait for the child processes */

    while (waitpid(-1,&exit_code,0)>0);
    /* open the output file and write the dimensions */
    f = fopen(argv[3], "w+");
    fprintf(f ,"%d %d\n",Cn,Cm);

    /* create the structures holding the threads */
    
    tid = malloc(nthreads * sizeof(pthread_t));
    attr = malloc(nthreads * sizeof(pthread_attr_t));

    
    /* 8) Create the threads */
    row = malloc(nthreads * sizeof(int));
    for(i=0; i<nthreads; i++)
    {
	    row[i] = i;
	    pthread_attr_init(&attr[i]);
	    pthread_create(&tid[i], &attr[i], multThread, &row[i]);
    }
    
    /* 9) join the threads */
    for(i=0; i<nthreads; i++)
	    pthread_join(tid[i], NULL);
    
    /* 10) write the result */

    fprintMatrix(f, C, Cn, Cm);
    
    /* close the output file */

    free(row);
    free(tid);
    free(attr);

    destroyMatrix(A, An);
    destroyMatrix(B, Bn);
    destroyMatrix(C, Cn);

    fclose(f);

    return 0;
}
