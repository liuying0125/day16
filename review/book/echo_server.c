#include <func.h>

#define BUF_SIZE 1024

int main(int argc,char* argv[])
{
    int ret;
    int serv_sock,clnt_sock;
    char message[BUF_SIZE];
    int str_len,i;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    ARGS_CHECK(argc,3);

    serv_sock = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(serv_sock,-1,"socket");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    ret = bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
    ERROR_CHECK(ret,-1,"bind");

    ret = listen(serv_sock,5);
    ERROR_CHECK(ret,-1,"listen");

    clnt_adr_sz = sizeof(clnt_adr);
    for(i=0;i<5;i++)
    {
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        ERROR_CHECK(clnt_sock,-1,"accept");
        printf("Connect client %d  \n",i+1);

        while((str_len = read(clnt_sock,message,BUF_SIZE))!=0){
            write(clnt_sock,message,str_len);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

