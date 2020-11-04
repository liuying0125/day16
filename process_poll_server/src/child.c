#include "process_pool.h"
//创建子进程，并初始化主数据结构
int makeChild(process_data_t *p,int processNum)
{
    int i;
    pid_t pid;
    int fds[2];
    int ret;
    for(i=0;i<processNum;i++)
    {
        //ono socketpair is a new fds[]
        ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        ERROR_CHECK(ret,-1,"socketpair");
        pid = fork();
        if(0==pid)  //子进程
        {
            close(fds[0]);
            childHandle(fds[1]);
        }
        close(fds[1]);
        p[i].pid = pid;   //子进程 pid father's return value is child id 
        p[i].pipeFd = fds[0]; //存储每个子进程的管道对端
    }
    return 0;
}
int childHandle(int pipeFd)
{
    int newFd;
    char finishFlag;
    while(1)
    {
        recvFd(pipeFd,&newFd);  //没有任务 就睡眠
        transFile(newFd);
        close(newFd); //close connect
        write(pipeFd,&finishFlag,1);  //子进程通知父进程完成任务
    }
    
}
