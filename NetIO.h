
int _NetIO_NewSock()
{
    return socket(AF_INET,SOCK_STREAM,0);
}

int _NetIO_Connect(int sfd,const char* IPStr,int Port)
{
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_addr.s_addr=inet_addr(IPStr);
    saddr.sin_port=htons(Port);
    saddr.sin_family=AF_INET;
    return connect(sfd,(const struct sockaddr*)&saddr,sizeof(saddr));
}
int _NetIO_SendN(int sfd,const void* pdata,int size)
{
    const char* p=(const char*)pdata;
    int done=0;
    while(done<size)
    {
        int ret=send(sfd,p+done,size-done,0);
        if(ret<0)
            return ret;
        done+=ret;
    }
    return done;
}

int _NetIO_RecvN(int sfd,void* pdata,int size)
{
    char* p=(char*)pdata;
    int done=0;
    while(done<size)
    {
        int ret=recv(sfd,p+done,size-done,0);
        if(ret<=0)
            return ret;
        done+=ret;
    }
    return done;
}

int _NetIO_CloseSock(int sfd)
{
    return closesocket(sfd);
}
