#include <func.h>
#include <unistd.h>
#define BUF_SZIE 1024
int main(int argc,char* argv[])
{
    int sock;
    char message[BUF_SZIE];
    int str_len;
    struct sockaddr_in serv_adr;
    int ret;

    ARGS_CHECK(argc,3);
    sock = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sock,-1,"socket");
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    ret = connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
    ERROR_CHECK(ret,-1,"connect");

    while(1)
    {

        printf("fd = %d\n",sock);
        fputs("Input message(Q to quit): ",stdout);
        fgets(message,BUF_SZIE,stdin);
        printf("%s",message);
        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"));
            break;
        
        printf("ddd");
        printf("fd = %d\n",sock);
        write(sock,message,strlen(message));
        str_len = read(sock,message,BUF_SZIE-1);
        message[str_len] = 0;
        printf("Message from server:%s\n",message);
    }
    close(sock);
    return 0;
}

