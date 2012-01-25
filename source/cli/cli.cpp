
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include "cli.hpp"
#include "log.hpp"



#if WINDOWS /* win32 */
    #define close(fd) closesocket(fd)
#else /* linux */
#endif

typedef struct _CliTable
{
    int cliNum;
    CliItem * cliItems;
    _CliTable * pNext;
}CliTable;



static int _cli_list(int argc, const char ** argv);
static int _cli_echo(int argc, const char ** argv);
static int _cli_1w(int argc, const char ** argv);
static string& _cli_trim(string& s);
static vector<string>& _cli_spliter(vector<string>& tokens, const string& str, const string& delimiters);
static CliItem* _cli_search_cmd(const char * string);

static CliItem _default_cli_[] = 
{
    {"echo",         _cli_echo,         "echo service", 0},
    {"ls",           _cli_list,         "list all commands", 0},
    {"1w",           _cli_1w,           "from 0 to 9999", 0},
};

static CliTable * p_cli_tables = NULL;



Cli::Cli() : PThread("cli"), sockfd(0), started(false)
{
    LOG_API();
    cli_append(_default_cli_, sizeof(_default_cli_)/sizeof(CliItem));
    start();
}


Cli::~Cli()
{
    LOG_API();
    stop();
}


void Cli::stop()
{
    close(sockfd);
#if WINDOWS
    WSACleanup(); 
#endif
    started = false;
}


int Cli::start()
{
    int iRet = 0;

    /* in case of restart by mistake. */
    if(started) return RET_OK;

#if WINDOWS
    WSADATA wsaData;
    iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
    { 
        WSACleanup( ); 
    }
#endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0)
    {
        print_socket_error();
    }
    assert_return(sockfd >= 0);

    sockaddr_in serverAddr;   
#if WINDOWS
    serverAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
#else
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
#endif
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(6789);
    iRet = bind(sockfd,(sockaddr*)&serverAddr,sizeof(sockaddr));
    if(iRet != 0)
    {
        print_socket_error();
        return iRet;
    }


    iRet = listen(sockfd,5);
    if(iRet != 0)
    {
        print_socket_error();
    }

    LOG_INFO("cli server listening at port %d.... \n", htons(serverAddr.sin_port));
    assert_return(iRet == 0);

    PThread::start();

    started = true;
    return iRet;
}


void Cli::run()
{
    do
    {
        sockaddr_in clientAddr;
        SOCKET clientSocket = 0;
#if WINDOWS
        int len=sizeof(sockaddr);
#else
        unsigned int len=sizeof(sockaddr);
#endif
        clientSocket = accept(sockfd,(sockaddr*)&clientAddr,&len);
        if(clientSocket > 0)
        {
#if WINDOWS
            LOG_INFO("cli client [%s:%d] connected. \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#else
            LOG_INFO("cli client [%s:%d] connected. \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
        }
        else
        {
            continue;
        }

        char buffer[1024];
        int i = 0;
        while(true)
        {
            memset(buffer, 0, sizeof(buffer));
            i = recv(clientSocket, buffer, sizeof(buffer), 0);
            if( i > 0)
            {
                cli_parser(buffer);
            }
            else if (i == 0)
            {
#if WINDOWS
                LOG_INFO("cli client [%s:%d] disconnected. \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#else
                LOG_INFO("cli client [%s:%d] disconnected. \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
                break;
            }
            else
            {
                print_socket_error();
                close(clientSocket);
                break;
            }
        }
    }while(0);
}


int Cli::cli_append(CliItem * cliItems, int cliNum)
{
    CliTable * newtable = (CliTable*)malloc(sizeof(CliTable));
    if(!newtable)
    {
        LOG_ERROR("<cli> memory allocation failure.\n");
        return RET_FAIL;
    }

    newtable->cliNum = cliNum;
    newtable->cliItems = cliItems;
    newtable->pNext = NULL;

    if(!p_cli_tables)
    {
        p_cli_tables = newtable;
    }
    else
    {
        CliTable * p = p_cli_tables;
        while(p->pNext) p=p->pNext;
        p->pNext = newtable;
    }

    return RET_OK;
}



int Cli::cli_parser(char * clistring)
{
    int     argc = 0;
    char ** argv = NULL;
    char  * temp = NULL;
    vector<string> splited;

    string clis(clistring);
    _cli_spliter(splited, _cli_trim(clis), " ");

    do {
        argc = splited.size();
        if(0 == argc) 
            break;

        temp = (char *)malloc(strlen(clistring)+1);
        if(!temp)
        {
            LOG_ERROR("<cli> memory allocation failure.\n");
            break;
        }

        argv = (char **)malloc(argc*sizeof(char*));
        if(!argv)
        {
            LOG_ERROR("<cli> memory allocation failure.\n");
            break;
        }

        char  * p = temp;
        char  * q = temp+strlen(clistring);

        for(int i=0; i<argc; i++)
        {
            argv[i]=p;
            sprintf(p, "%s", splited[i].c_str());
            p += strlen(splited[i].c_str())+1;
        }

        CliItem * cliitem = _cli_search_cmd(splited[0].c_str());
        if(cliitem)
        {
            int ret = cliitem->action(argc, (const char **)argv);
            if(RET_OK != ret)
            {
                LOG_ERROR("<cli> command \"%s\" error, %d!\n", cliitem->command, ret);
            }
        }
        else
        {
                LOG_ERROR("<cli> command \"%s\" not found!\n", splited[0].c_str());
        }
    }while(0);

    if(argv) {free(argv); argv=NULL;};
    if(temp) {free(temp); temp=NULL;};

    return RET_OK;
}



static string& _cli_trim(string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}


static vector<string>& _cli_spliter(vector<string>& tokens, const string& str, const string& delimiters = " ")
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
}


static CliItem* _cli_search_cmd(const char * string)
{
    int i = 0;
    CliTable * p = p_cli_tables;

    while(p)
    {
        for(i=0;i<p->cliNum;i++)
        {
            if(strlen(string) == strlen(p->cliItems[i].command)
            && 0 == strcmp(string, p->cliItems[i].command))
            {
                return &p->cliItems[i];
            }
        }
        p=p->pNext;
    }

    return NULL;
}

static int _cli_list(int argc, const char ** argv)
{
    int i = 0;
    CliTable * p = p_cli_tables;

    printf("<cli> Command\t Description\n");
    do{
        for(i=0;i<p->cliNum;i++)
        {
            printf("      %s\t %s\n", p->cliItems[i].command, p->cliItems[i].description);
        }
        p=p->pNext;
    }while(p);

    return RET_OK;
}

static int _cli_echo(int argc, const char ** argv)
{
    for(int i=1; i<argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return RET_OK;
}

static int _cli_1w(int argc, const char ** argv)
{
    for(int i=0; i<10000; i++)
    {
        printf("%4d ",i);
    }
    return RET_OK;
}


