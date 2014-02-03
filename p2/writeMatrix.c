#include <unistd.h>     // unix-like system calls read and write
#include <fcntl.h>      // unix-like file handling : open
#include <stdio.h>
#include <string.h>



int main(int argc, char* argv[])
{
    char buff[100];
    int num;
    int i,j;
    int m,n;

    if(argc < 4) {
        printf("Usage:  ./mat 5 8 out.txt\n");
        return -1;
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    int fout = open( argv[3],  O_CREAT | O_RDWR, 0640 );
    if(fout < 0) return -1;

    sprintf(buff,"%d %d\n",n,m);
    write(fout,buff,strlen(buff));

    for(i=0;i<n;i++) {
        for(j=0;j<m;j++) {
            num = rand()%10;
            sprintf(buff,"%d ",num);
            write(fout,buff,strlen(buff));
        }
        sprintf(buff,"\n",num);
        write(fout,buff,strlen(buff));
    }        
    
    close( fout );
    return 0;
}