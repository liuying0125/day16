#include "process_pool.h"
void sigFunc(int signum)
{
    printf("%d is coming\n",signum);
}

int transFile(int newFd)
{
    int ret;
    signal(SIGPIPE,sigFunc);
    train_t train;
    train.dataLen = strlen(FILENAME); //file name must be string type 
    strcpy(train.buf,FILENAME);
    send(newFd,&train,4+train.dataLen,0);
    //send file size to client
    struct stat buf;
    int fd = open(FILENAME,O_RDWR);
    fstat(fd,&buf);
    train.dataLen = sizeof(buf.st_size);
    
    send(newFd,&train,4+train.dataLen,0);

    memcpy(train.buf,   &buf.st_size,  train.dataLen);
    
    while((train.dataLen = read(fd,train.buf,sizeof(train.buf))))
    {
        ret = send(newFd,&train,4+train.dataLen,0);
        if(ret ==-1)
        {
            return -1;
        }
    }
    send(newFd,&train,4,0);
    return 0;
}

