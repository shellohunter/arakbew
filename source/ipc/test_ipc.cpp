
#include <stdio.h>

#include "ipc.hpp"
#include "shared.hpp"


using namespace std;

#if WINDOWS /* win32 */
    #define close(fd) closesocket(fd)
#endif


static void myIPCHandler(IPCNotify * notify)
{
    printf("<receiver> [%s].\n", notify->u.cli);
}

int main()
{
    IPC * ipc = new IPC(myIPCHandler);

    int ret = 0; 

#if WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 0 ); 
    ret = WSAStartup( wVersionRequested, &wsaData );
    assert_return( 0 == ret );

    if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) 
    {
        WSACleanup( );
        return -1; 
    }
#endif

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM/*SOCK_STREAM*/, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
    }
    assert_return(sockfd >= 0);

#if 0
    bool opt = true;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR *>(&opt), sizeof(opt));
#endif
    sockaddr_in addrto;
    memset(&addrto,0,sizeof(addrto));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    addrto.sin_port = htons(6789);
    int nlen=sizeof(addrto);

    IPCNotify notify;
    int counter = 0;
    while(counter < 10)
    {
        sleep(1000);
        notify.ipcType=counter;
        sprintf(notify.u.cli,"udp test package %d",counter);
        printf("<sender>   [%s].\n", notify.u.cli);
        if( sendto(sockfd, (char *)&notify, sizeof(notify), 0, (sockaddr*)&addrto,nlen)<0)
        {
            print_socket_error();
        }
        counter++;
    }
    assert_return (0 == close(sockfd));
#if WINDOWS
    assert_return (0 == WSACleanup());
#endif

    DELETE(ipc);
    
    return 0;
}









