#include <func.h>

void recvCycle(int sfd,void* buf,int recvLen)
{
    char *p=(char*)buf;
    int total =0,ret;
    while(total<recvLen)
    {
        ret = recv(sfd,p+total,recvLen-total,0);
        total += ret;

    }

}



