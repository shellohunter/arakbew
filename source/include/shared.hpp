
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

#include "log.hpp"
#include "cli.hpp"
#include "ipc.hpp"
#include "ptasking.hpp"


using namespace std;


#if WINDOWS /* win32 */
    #define sleep(ms) Sleep(ms)
    #define print_socket_error() LOG_VERBOSE("<socket> error %d. %s, L%d.\n", WSAGetLastError(), __FUNCTION__, __LINE__)
#else /* linux */
    #define print_socket_error() LOG_VERBOSE("<socket error> %s. %s, L%d.\n", strerror(errno), __FUNCTION__, __LINE__)
    typedef struct sockaddr_in sockaddr_in;
    typedef struct sockaddr sockaddr;
    #define SOCKET int
#endif


typedef enum
{
    OK,
    FAIL = -1,
    E_SQLITE_FAIL = -2,
    E_OUT_OF_MEMORY = -3,
    //... Please put your error code here.

}E_RET_CODE;


#undef DELETE
#define DELETE(p) if(p) {LOG_VERBOSE("delete %s. %s\n", #p, __FUNCTION__); delete p;}

#undef FREE
#define FREE(p) if(p) {LOG_VERBOSE("free %s. %s\n", #p, __FUNCTION__); free(p);}

#undef DELETE_ARRAY
#define DELETE_ARRAY(p) if(p) {LOG_VERBOSE("delete %s. %s\n", #p, __FUNCTION__); delete[] p;}









#endif /* SHARED_HPP */
