
#include <stdio.h>
#include <string.h>
#if WINDOWS
    #include <winsock2.h>
    #include <iostream>
    #include "pthread.h"
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    #include <pthread.h>
    #include <errno.h>
#endif
#if 0
#include "ptasking.hpp"
#endif

#if WINDOWS /* win32 */
    #define sleep(ms) Sleep(ms)
    #define close(fd) closesocket(fd)
    #define print_socket_error() printf("socket error %d. %s, L%d.\n", WSAGetLastError(), __FUNCTION__, __LINE__)
#else /* linux */
    #define print_socket_error() printf("socket error %s. %s, L%d.\n", strerror(errno), __FUNCTION__, __LINE__)
    typedef struct sockaddr_in sockaddr_in;
    typedef struct sockaddr sockaddr;
    #define SOCKET int
#endif


#if 0

class CliResonse : public PThread
{
public:
    CliResonse() : PThread("cliresp"), flag(true) {}
	void start(SOCKET fd) {sockfd = fd; PThread::start();} 
    void run()
    {
        printf("[start listening cli response.\n>");
        char buffer[1024];
        while(flag)
        {
            memset((void *)buffer,0,sizeof(buffer));
            if(recv(sockfd,buffer,101,0) > 0)
            {
                printf(" %s\n",buffer);
            }
        }
    }
    void stop()
    {
        flag = false;
    }
private:
    int sockfd;
    bool flag;
};

int main_udp(int argc, char ** argv)
{

#if WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 0 ); 
    int ret = WSAStartup( wVersionRequested, &wsaData );
    if( 0 != ret )
    {
        print_socket_error();
        return -1;
    }

    if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) 
    {
        WSACleanup( );
        return -1;
    }
#endif

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
        return -1;
    }

#if 0
    bool opt = true;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR *>(&opt), sizeof(opt));
#endif

    char * ip = NULL;
    if(argc == 2)
    {
        ip = argv[1];
    }
    else
    {
        ip = "127.0.0.1";
    }

    sockaddr_in addrto;
    memset(&addrto,0,sizeof(addrto));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = inet_addr(ip); 
    addrto.sin_port = htons(6789);
    int nlen=sizeof(addrto);

    printf("[open connection to %s:%d]\n>", ip, 6789);

    char buffer[512];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        if(gets(buffer))
        {
            if( sendto(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&addrto,nlen)== SOCKET_ERROR )
            {
                print_socket_error();
            }
            printf(">");
        }
    }
    close(sockfd);
#if WINDOWS
    WSACleanup();
#endif

    return 0;
}
#endif


void * cli_rsp(void * data)
{
    printf("[start listening cli response.\n>");
    char buffer[1024];
    SOCKET sockfd = (SOCKET)data;
    while(1)
    {
        memset((void *)buffer,0,sizeof(buffer));
        if(recv(sockfd, buffer,sizeof(buffer), 0) > 0)
        {
            printf(" %s\n",buffer);
        }
    }
}


int main_tcp(int argc, char ** argv)
{
    int ret = 0;
#if WINDOWS
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 0 ); 
    ret = WSAStartup( wVersionRequested, &wsaData );
    if( 0 != ret )
    {
        print_socket_error();
        return -1;
    }

    if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) 
    {
        WSACleanup( );
        return -1;
    }
#endif

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
        return -1;
    }

    const char * ip = NULL;
    if(argc == 2)
    {
        ip = argv[1];
    }
    else
    {
        ip = "127.0.0.1";
    }

    sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip); 
    serverAddr.sin_port = htons(6789);
    int nlen=sizeof(serverAddr);

    do {
        printf("[connecting to %s:%d ......]\n", ip, 6789);
        ret = connect(sockfd,(sockaddr*)&serverAddr,sizeof(sockaddr));
        if(ret < 0)
        {
            printf("[connection NG, will retry in 5 seconds!]\n", ip, 6789);
            print_socket_error();
            sleep(5000);
        }
    }while(ret !=0);

    printf("[connection OK , server at [%s:%d]]\n", ip, 6789);


    pthread_t tid;
    pthread_create(&tid, NULL, cli_rsp, (void*)sockfd);

    char buffer[1024];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
#if WINDOWS
        if(gets(buffer))
#else
        if(fgets(buffer, sizeof(buffer), stdin))
#endif
        {
            if( send(sockfd, buffer, sizeof(buffer), 0) < 0)
            {
                print_socket_error();
            }
            printf(">");
        }
    }
    close(sockfd);
#if WINDOWS
    WSACleanup();
#endif

    return 0;
}

int main(int argc, char ** argv)
{
    main_tcp(argc, argv);
    return 0;
}

