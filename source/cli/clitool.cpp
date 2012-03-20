
#include <stdio.h>
#include <string.h>
#ifdef WINDOWS
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
#include "ptasking.h"
#endif

#ifdef WINDOWS /* win32 */
    #define sleep(ms) Sleep(ms)
    #define close(fd) closesocket(fd)
    #define print_socket_error() printf("socket error %d. %s, L%d.\n", WSAGetLastError(), __FUNCTION__, __LINE__)
#else /* linux */
    #define print_socket_error() printf("socket error %s. %s, L%d.\n", strerror(errno), __FUNCTION__, __LINE__)
    typedef struct sockaddr_in sockaddr_in;
    typedef struct sockaddr sockaddr;
    #define SOCKET int
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
            printf("%s\n>>",buffer);
        else
            break;
    }
    printf("[cli rsp exit!]\n");
    return NULL;
}


int tcp_task(int argc, char ** argv)
{
    int ret = 0;
    SOCKET sockfd;
#ifdef WINDOWS
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

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    printf("[connecting to %s:%d ......]\n", ip, 6789);
    ret = connect(sockfd,(sockaddr*)&serverAddr,sizeof(sockaddr));
    if(ret < 0)
    {
        print_socket_error();
        printf("[connection abort!]\n");
        close(sockfd);
        return -1;
    }

    printf("[connection OK , server at [%s:%d]]\n", ip, 6789);


    pthread_t tid;
    pthread_create(&tid, NULL, cli_rsp, (void*)sockfd);

    char buffer[1024];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
#ifdef WINDOWS
        if(gets(buffer))
#else
        if(fgets(buffer, sizeof(buffer), stdin))
#endif
        {
            if(send(sockfd, buffer, sizeof(buffer), 0) < 0)
            {
                print_socket_error();
                /* lost connection to the server? */
                printf("[connection abort!]\n");
                break;
            }
        }
        printf(">>");
    }
    close(sockfd);
#ifdef WINDOWS
    WSACleanup();
#endif

    return 0;
}

int main(int argc, char ** argv)
{
    char x;
    do
    {
        tcp_task(argc, argv);
        printf("reconnect? y/n\n");
        x = getchar();getchar();
    }while(x == 'y' || x == 'Y');
    return 0;
}

