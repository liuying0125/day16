#include <func.h>
void recvCycle(int,void*,int);
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd = socket(AF_INET,SOCK_STREAM,0);// AF_INET ipv4协议  tcp：SOCK_STREAM  后面socket所使用的传输协议编号，通常为0
    ERROR_CHECK(socketFd,-1,"socket"); 
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));  //清零是必须的
   //底下几行也是用于初始化
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(atoi(argv[2])); //atoi函数将字符串转换为整型数
                    //htons函数  将主机的  无符号短整型  转换成网络字节
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret;
    ret = connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr)); 
    //用来请求连接远程服务器，将参数 sockfd 的 socket 连至参数 serv_addr 指定的服务器
    //IP 和端口号上去。   
    //中间那个参数   为结构体指针变量，存储着远程服务器的 IP 与端口号信息。
    ERROR_CHECK(ret,-1,"connect");
    int fd;
    int dataLen;
    char buf[1000]={0};
    recvCycle(socketFd,&dataLen,4);
    recvCycle(socketFd,buf,dataLen);
  
    fd =open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");

    //recieve  file size
    off_t fileSize,downLoadsize = 0,lastLoadSize=0,slice;
 
    recvCycle(socketFd,&dataLen,4); //begin receive
    recvCycle(socketFd,&fileSize,dataLen);
    slice = fileSize/50;
    struct timeval start,end;
    gettimeofday(&start,NULL);
    while(1)
    {
        recvCycle(socketFd,&dataLen,4);
        if(dataLen>0)
        {
            recvCycle(socketFd,buf,dataLen);
            write(fd,buf,dataLen);
            downLoadsize+=dataLen;
            if(downLoadsize - lastLoadSize >= slice)
            {

                printf("%5.2f%s\r",(float)downLoadsize/fileSize*100,"%");
                fflush(stdout);
                lastLoadSize = downLoadsize;
            }
        }else{
            printf("100.00%%\n");
            break;
        }
    }
    gettimeofday(&end,NULL);
    printf("use time = %ld\n",(end.tv_sec - start.tv_sec)*1000000+end.tv_usec -start.tv_usec);
    close(fd);
    close(socketFd);
}

