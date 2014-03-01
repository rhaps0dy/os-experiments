#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

sem_t* mutex;
sem_t* fillSlots;
sem_t* emptySlots;

#define N 10
#define Nstr "10"
int bEnd = 0;
char cmd[120];
char role[120];
char arg[120];


int SHM_SIZE;
int shmid;
int shmkey = 22222; // you can choose it as your choice

int i,pid;

char programpath[80];
char *args[4];

int main(int argc, char *argv[])
{
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    fillSlots = sem_open("/fillSlots", O_CREAT, 0644, 0);
    emptySlots = sem_open("/emptySlots", O_CREAT, 0644, N);
    
    // Two necessary calls to allocate the shared memory space
    SHM_SIZE = 3 * sizeof(int) + N * sizeof(int);
    shmid = shmget(shmkey, SHM_SIZE, 0666 | IPC_CREAT);  // IPC_CREAT flag to create shared mem
    int *shmpointer = shmat(shmid,NULL,0);  //now attach a memory to this share memory
    if(shmid < 0 || shmpointer == NULL) {
        printf("error allocating shared memory key %d\n", shmkey);
        return -1;
    }
    
    shmpointer[0] = 0;   // in
    shmpointer[1] = 0;   // out
    shmpointer[2] = 1;   // last item produced
    
    printf("*** FactoryShell Usage **** \n");
    printf("FactoryShell> producer 3      (type this to produce 3 items)\n");
    printf("FactoryShell> consumer 4      (type this to consume 4 items,\n");
    printf("                               3 will be consumed and then block)\n");
    printf("Return or end to quit\n");
    
    while(!bEnd) {
        printf("FactoryShell> ");
        fgets(cmd, sizeof(cmd), stdin);
        
        strcpy(arg,"");
        if (strchr(cmd, ' ') != NULL) sscanf(cmd,"%s %s",role,arg);
        else sscanf(cmd,"%s",role);
        if(strcmp(role,"end") == 0) { bEnd = 1; break; }
        
        //fill the args we will pass to the child programs
        getcwd(programpath, 80);
        strcat(programpath, "/");
        strcat(programpath, role);
        args[0] = programpath;
        args[1] = arg;
        args[2] = Nstr;
        args[3] = NULL;
        
        if(strcmp(role,"consumer") == 0 || strcmp(role, "producer") == 0) {
            
            // **********************************************************************
            // Exercice) Add the necessary code here to call the children
            pid = fork();
            if(pid==0) {
                execve(programpath, args, NULL);
            }
            else if(pid<0) {
                printf("Error when forking the program\n");
                bEnd = 1;
            }
        }
        else {
            printf("Command not recognized\n");
            printf("Usage: producer integer  (num of elems to produce)\n");
            printf("       consumer integer  (num of elems to consume)\n\n");
            if(strlen(cmd) == 1) bEnd = 1;
        }
        usleep(1000000);
    }
    
    shmdt(shmpointer);
    shmctl(shmid, IPC_RMID, 0); 
    
    sem_close(mutex);
    sem_close(fillSlots);
    sem_close(emptySlots);
    
    sem_unlink("/mutex");
    sem_unlink("/fillSlots");
    sem_unlink("/emptySlots");
    
    return 0;
}
