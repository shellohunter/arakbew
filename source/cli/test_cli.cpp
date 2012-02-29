#include <string.h>

#include "log.h"
#include "cli.h"
#include "shared.h"


#ifdef WINDOWS /* win32 */
    #include "pthread.h"
    #define close(fd) closesocket(fd)
#else
    #include <pthread.h>
#endif


static int _cli_test1(int argc, const char ** argv)
{
    LOG_API();
    const char * comment = 
        "Always code as if the guy who ends up maintaining your code will be a violent psychopath who knows where you live.\n"
        "               ----- Rick Osborne\n";
    printf(comment);;
    return OK;
}

static int _cli_test2(int argc, const char ** argv)
{
    LOG_API();
    printf("This cli test 2, i am going to print out all arguments.\n");
    for(int i=0; i<argc; i++)
    {
        printf("%s ",argv[i]);
    }
    printf("\n");
    return OK;
}

CliItem _my_cli_table_[] = 
{
    {"test.1",      _cli_test1,         "test 1", 0},
    {"test.2",      _cli_test2,         "test 2", 0},
};


const char * test_cli[] =
{
    "ls ", /* default command */
    "echo    hello_cli! 1234567890", /* default command */
    "gdl", /* log command */
    "sdl aei ", /* log command */
    "gdl", /* log command */
    "test.1",
    "test.2 arg1 arg2 arg3  ",
    "no-such-cmd", /* no such command */
    NULL,
};


void * cli_server(void * data)
{
    Cli * cli = new Cli();
    return cli;
}

/* test cli */
int main(int argc, char ** argv)
{

    int ret = 0; 

    log_init();

    Cli::cli_append(_my_cli_table_, sizeof(_my_cli_table_)/sizeof(CliItem));

    Cli * cli = new Cli();
    if(cli);

#if 0
    pthread_t tid;
    pthread_create(&tid, NULL, cli_server, NULL);

    sleep(10000);
#endif

#ifdef WINDOWS
    WSADATA wsaData;
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
    { 
        WSACleanup( ); 
    }
#endif

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM/*SOCK_STREAM*/, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
    }
    ASSERT(sockfd >= 0);

    sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serverAddr.sin_port = htons(6789);

    do {
        printf("[connecting to %s:%d ......]\n", "127.0.0.1", 6789);
        ret = connect(sockfd,(sockaddr*)&serverAddr,sizeof(sockaddr));
        if(ret < 0)
        {
            printf("[connection NG, will retry in 5 seconds!]\n");
            print_socket_error();
            sleep(5000);
        }
    }while(ret !=0);

    printf("[connection OK , server at [%s:%d]]\n", "127.0.0.1", 6789);

    char buffer[1024];
    int counter = 0;
    while(test_cli[counter])
    {
        sleep(1000);
        sprintf(buffer, "%s", test_cli[counter]);
        printf("<sender>   [%s].\n", buffer);
        if(send(sockfd, buffer, sizeof(buffer), 0)<0)
        {
            print_socket_error();
        }
        counter++;
    }
    ASSERT(0 == close(sockfd));
#ifdef WINDOWS
    ASSERT(0 == WSACleanup());
#endif

    return 0;
}


