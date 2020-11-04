#include <func.h>

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char* argv[])
{
    int ret;
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "hello,world!";
    ARGS_CHECK(argc,3);

    serv_sock = socket(AF_INET,SOCK_STREAM,0);
    if(serv_sock==-1){
        error_handling("socket() error");
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    ret = bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    ERROR_CHECK(ret,-1,"bind");

    ret = listen(serv_sock,5);
    ERROR_CHECK(ret,-1,"listen");
    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    ERROR_CHECK(clnt_sock,-1,"accept");
    write(clnt_sock,message,sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

