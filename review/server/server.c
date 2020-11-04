#include <func.h>

int tcpInit(int*,char*,char*);

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]);
    int newFd = accept(socketFd,NULL,NULL);

    int ret;
    char buf[10];
    int epfd = epoll_create(1);
    struct epoll_event event,evs[2];
    event.events = EPOLLIN;
    event.data.fd  =STDIN_FILENO;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event); //add input
    ERROR_CHECK(ret,-1,"epoll_ctl");
    event.data.fd = newFd;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,newFd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    int readyFdCount,i;
    while(1)
    {
        readyFdCount = epoll_wait(epfd,evs,2,-1);
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].events==EPOLLIN && evs[i].data.fd == STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf)-1);
                if(ret==0)
                {
                    printf("bye\n");
                    break;
                }
                send(newFd,buf,strlen(buf)-1,0);
            }
            if(evs[i].data.fd == newFd)
            {
                bzero(buf,sizeof(buf));
                ret = recv(newFd,buf,sizeof(buf)-1,0);
                if(ret==0)
                {
                    printf("bye\n");
                    break;
                }
                printf("%s\n",buf);
            }
        }
    }
    return 0;
}

