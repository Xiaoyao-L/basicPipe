#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

using namespace std;

void pipeRead(){
    char fifo_name[] = "./process_in";
    char buf[1024];
    int fifo_handle;
    int size;

    mknod(fifo_name, S_IFIFO | 0666, 0);
    printf("c++ is ready\n");

    if ((fifo_handle = open(fifo_name, O_RDONLY)) < 0){
        perror("open failed");
    }

    do{
        size = read(fifo_handle, buf, 1024);
        if(size<0)
            perror("read failed");
        else if(size == 0)
            printf("server left\n");
        else{
            buf[size] = '\n';
            buf[size + 1] = '\0';
            printf("%s\n", buf);
        }
    }while(size>0);
    close(fifo_handle);
}

void pipeWrite(char arr[]){
    char fifo_name[] = "./process_out";
    char buf[1024];
    int fifo_handle;


    mkfifo(fifo_name, 0666);
    strcpy(buf,arr);


    if ((fifo_handle = open(fifo_name, O_WRONLY)) < 0){
        perror("open failed");
    }

    write(fifo_handle, buf, strlen(buf)+1);

    close(fifo_handle);
}

int main(){
    char buf[1024];
    strcpy(buf,"BootNotification={\"chargePointModel\":\"PB\",\"chargePointVendor\":\"TPE\"}");
    pipeWrite(buf);
    pipeRead();
    exit(1);
    return 0;

}
