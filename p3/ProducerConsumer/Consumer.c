#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>


int n;              // Elements to consume
int N;             // Buffer size
int* out;           // out index
int* buffer;        // the buffer pointer that we will allocate from shared memory
int item;

// Shared memory variables
int SHM_SIZE;
int shmid;
int shmkey = 22222;

sem_t* mutex;
sem_t* fillSlots;
sem_t* emptySlots;

void nap(int t) {
    usleep(10000 + rand() % t);
}

void consumeItem( int num ) {
    int i = num;
    int j;
    int bPrime = 1;  //0 for false, 1 for true
    for (j = i-1; j > 1; j--) {
        if (i%j == 0) {
            bPrime = 0;
            break;
        }
    }
    nap(1000);
    if(bPrime) printf("pid %d: consumed item %d\n",getpid(), num);
    else       printf("pid %d: consumed item %d but it's not prime\n",getpid(), num);
}

int main(int argc, char *argv[])
{
    int i;
    
    if (argc != 3) {
        printf("usage:  consumer  n  N  (where n : elems to consume   N : buffer size)\n");
        return -1;
    }
    n = atoi(argv[1]);
    N = atoi(argv[2]);
    
    // Two necessary calls to allocate the shared memory space
    SHM_SIZE = 3 * sizeof(int) + N * sizeof(item);
    shmid = shmget(shmkey,SHM_SIZE, 0444);  // shared mem for reading purposes
    int *shmpointer = shmat(shmid,NULL,0);  // now attach a memory to this share memory
    if(shmid < 0 || shmpointer == NULL) {
        fprintf(stderr,"error allocating shared memory key %d\n", shmkey);
        return -1;
    }
    
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    fillSlots = sem_open("/fillSlots", O_CREAT, 0644, 0);
    emptySlots = sem_open("/emptySlots", O_CREAT, 0644, N);
    
    
    // **********************************************************************
    // Exercice 2) Add the consumer code here
    
    out = shmpointer + sizeof(int);
    buffer = shmpointer + 3*sizeof(int);
    for(i=0; i<n; i++) {
        sem_wait(fillSlots);
        sem_wait(mutex);
        item = buffer[*out];
        (*out) = ((*out)+1) % N;
        sem_post(mutex);
        sem_post(emptySlots);
        consumeItem(item);
    }
    
    
    shmdt(&shmpointer);
    
    return 0;
}
