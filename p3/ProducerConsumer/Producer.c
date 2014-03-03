#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <math.h>


int n;
int N;
int* in;
int* buffer;
int* currentItem;
int item = 1;

int SHM_SIZE;
int shmid;
int shmkey = 22222;

sem_t* mutex;
sem_t* fillSlots;
sem_t* emptySlots;

void nap(int t) {
    usleep(10000 + rand() % t);
}

int produceItem( int num ) {
    int i = num+1;
    int j;
    while(i < 999999) {
        for (j = i-1; j > 1; j--) {
            if (i%j == 0) break;
        }
        if (j == 1) break;
        else i++;
    }
    nap(10000);
    printf("pid %d : produced %d\n", getpid(), i);
    return i;
}


int main(int argc, char *argv[])
{
    int i;
    
    if (argc != 3) {
        printf("usage: producer n  N   (where n : elems to produce   N : buffer size)\n");
        return -1;
    }
    
    n = atoi(argv[1]);
    N = atoi(argv[2]);
    
    // Two necessary calls to allocate the shared memory space
    SHM_SIZE = 3 * sizeof(int) + N * sizeof(item);
    shmid = shmget(shmkey,SHM_SIZE, 0666);  // shared mem for writting purposes
    int *shmpointer = shmat(shmid,NULL,0);  //now attach a memory to this share memory
    if(shmid < 0 || shmpointer == NULL) {
        printf("error allocating shared memory key %d\n", shmkey);
        return -1;
    }
    
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    fillSlots = sem_open("/fillSlots", O_CREAT, 0644, 0);
    emptySlots = sem_open("/emptySlots", O_CREAT, 0644, N);
    
    
    // **********************************************************************
    // Exercice 1) Add the producer code here
    in = shmpointer;
    currentItem = shmpointer + 2*sizeof(int);
    buffer = currentItem + sizeof(int);
    for(i=0; i<n; i++) {
        item = produceItem(*currentItem);
        (*currentItem)++;
        
        sem_wait(emptySlots);
        sem_wait(mutex);
        buffer[*in] = item;
        (*in) = ((*in)+1) % N;
        sem_post(mutex);
        sem_post(fillSlots);
    }
    
    shmdt(&shmpointer);
    
    return 0;
}
