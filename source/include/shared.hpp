
#ifndef SHARED_HPP
#define SHARED_HPP

#include <string>
#include <vector>
#include <stack>

#if WINDOWS
#else
#include <sys/socket.h>
#include <errno.h>
#endif

using namespace std;

#define assert_return(__exp__)\
    do{\
        if(!(__exp__))\
        {\
            printf("Assert(\"%s\") fail! line %d, %s.\n",#__exp__, __LINE__, __FUNCTION__);\
            return -1;\
        }\
    }while(0)


#define assert_break(__exp__)\
    do{\
        if(!(__exp__))\
        {\
            printf("Assert(\"%s\") fail! line %d, %s.\n",#__exp__, __LINE__, __FUNCTION__);\
            break;\
        }\
    }while(0)

#define API()  printf("%s()\n", __FUNCTION__)


#if WINDOWS /* win32 */
    #define sleep(ms) Sleep(ms)
    #define print_socket_error() printf("socket error %d. %s, L%d.\n", WSAGetLastError(), __FUNCTION__, __LINE__)
#else /* linux */
    #define print_socket_error() printf("socket error %s. %s, L%d.\n", strerror(errno), __FUNCTION__, __LINE__)
    typedef struct sockaddr_in sockaddr_in;
    typedef struct sockaddr sockaddr;
    #define SOCKET int
#endif


typedef enum
{
    RET_OK = 0,
    RET_FAIL = -1
    //... Please put your error code here.
}eRetCode;



#endif /* SHARED_HPP */
