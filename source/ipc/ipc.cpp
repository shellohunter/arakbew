
#include <iostream>
#include "ipc.hpp"
#include "shared.hpp"
#include "log.hpp"

using namespace std;

#if WINDOWS /* win32 */
    #define close(fd) closesocket(fd)
#else /* linux */
#endif


IPC::IPC(const IPCCallback ipc_handler, const char * name)
    : PThread(name, true, -1), _ipc_handler_(ipc_handler)
{
    if(0 != init())
    {
        LOG_ERROR("IPC init error!\n");
    }
}

IPC::~IPC()
{
    this->stop();
}


int IPC::init()
{
    int iRet = 0;

#if WINDOWS
    WSADATA wsaData;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
    { 
        WSACleanup( ); 
    }
#endif

    sockfd = socket(AF_INET, SOCK_DGRAM/*SOCK_STREAM*/, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
    }
    ASSERT(sockfd >= 0);

    sockaddr_in serverAddr;   

#if WINDOWS
        serverAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
#else
        serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
#endif
    serverAddr.sin_family=AF_INET;   
    serverAddr.sin_port=htons(6790);   
    iRet = bind(sockfd,(sockaddr*)&serverAddr,sizeof(sockaddr));   
    if(iRet != 0)
    {
        print_socket_error();
    }
    ASSERT(iRet == 0);

    this->start();

    return iRet;
}


void IPC::stop(void)
{
    close(sockfd);
#if WINDOWS
    WSACleanup( ); 
#endif
}

void IPC::run()
{
    while(true)
    {
        sockaddr_in addrClient;   
#if WINDOWS
        int len=sizeof(sockaddr);
#else
        unsigned int len=sizeof(sockaddr);
#endif
        char recvBuf[512];  
        memset(recvBuf,0,sizeof(recvBuf));
        int iret = recvfrom(sockfd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *)&addrClient, &len);
        if(iret < 0)
        {
            print_socket_error();
            break;
        }          
        if(_ipc_handler_)
            _ipc_handler_((IPCNotify*)recvBuf);
        else
            LOG_INFO("ipc msg arrrives, len == %d, non-receiver, dropped.\n", len);
    }
}





