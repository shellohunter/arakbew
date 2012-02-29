
#ifndef IPC_HPP
#define IPC_HPP

#ifdef WINDOWS
    #include <winsock2.h>
    #include <iostream>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
#endif
#include "ptasking.h"
#include "shared.h"

enum
{
    E_IPC_UNDEFINED = 0,
    // E_IPC_XXX, /* define your own IPC type. */
};


typedef struct
{
    int ipcType;
    union
    {
        char cli[128];
        int dbgLevel;
    }u;
}IPCNotify;

typedef void (*IPCCallback)(IPCNotify * notify);


class IPC : public PThread
{
public:
    IPC(const IPCCallback ipc_handler = NULL, const char * name = "ipc");
    virtual ~IPC();
    void stop(void);

protected:
    SOCKET sockfd;
    
private:
    IPCCallback _ipc_handler_;
    void run(void);
    int init();
};


#endif /* IPC_HPP */
