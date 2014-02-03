#include <unistd.h>     // unix-like system calls read and write
#include <fcntl.h>      // unix-like file handling : open
#include <stdio.h>
#include <string.h>



int main(int argc, char* argv[])
{
    char buff[100];
    float num;
    int i = 0;
    
    if(argc < 3) {
        printf("Usage:  ./Program out.txt 1000\n");
        return -1;
    }
    
    int fout = open( argv[1],  O_CREAT | O_RDWR, 0640 );
    if(fout < 0) return -1;
    
    int n = 0;
    sscanf(argv[2],"%d",&n);
    
    while(i < n) {
        num = rand()%1000 / 1000.0;
        sprintf(buff,"%f ",num);
        write(fout,buff,strlen(buff));
        i++;
    }
    
    close( fout );
    return 0;
}