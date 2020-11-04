#include "process_pool.h"

int main(int argc,char* argv[])
{
    if(argc!=4){
        printf("./process_pool_server ip port process_num\n");
        return -1;
    }   
    int processNum = atoi(argv[3]);  //得到进程数
    process_data_t *pData = (process_data_t*)calloc(processNum,sizeof(process_data_t));
    makeChild(pData,processNum); //创建 processNum 个子进程
//#ifdef DEBUG
//    int i;
  //  for(i=0;i<processNum;i++)
    //{
   //     printf("pid=%d,pipeFd=%d\n", pData[i].pid , pData[i].pipeFd);
  //  }
//#endif
    int i;
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]); //初始化socket 并开启监听
    int epfd = epoll_create(1);
    struct epoll_event *evs;
    evs =(struct epoll_event*)(struct epoll_event*)calloc(processNum+1,sizeof(struct epoll_event));
    epollInAdd(epfd,socketFd);
    for(i=0;i<processNum;i++)
    {
        epollInAdd(epfd,pData[i].pipeFd);   //注册监听每一个子进程的管道的管道对端

    }
    int readyFdCount,newFd,j;
    int noBusyflag;
    while(1){
        readyFdCount = epoll_wait(epfd,evs,processNum+1,-1);
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].data.fd == socketFd)
            {
                newFd = accept(socketFd,NULL,NULL); //接收客户端请求
                for(j=0;j<processNum;j++)//要找非忙碌的子线程
                {   
                    if(pData[j].busy==0)
                    {
                        sendFd(pData[j].pipeFd,newFd); //把任务发给对应的子进程
                        pData[j].busy=1;
                        printf("%d pid is busy",pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }
            for(j=0;j<processNum;j++)
            {
                if(evs[i].data.fd == pData[j].pipeFd)
                {
                    read(pData[j].pipeFd,&noBusyflag,1);
                    pData[j].busy=0;
                    printf("%d pid is not busy\n",pData[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}

