
#ifndef CLI_PARSER_HPP
#define CLI_PARSER_HPP

#if WINDOWS
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    #define SOCKET int
#endif
#include "shared.hpp"
#include "ptasking.hpp"

typedef int cli_exec(int argc, const char ** argv);

typedef struct _CliItem
{
    const char      * command;
    cli_exec  * action;
    const char      * description;
    int         accessLevel;
}CliItem;


class Cli : public PThread
{
public:
    Cli();
    virtual ~Cli();
    void stop(void);
    int start(void);

    static int cli_parser(char * clistring);
    static int cli_append(CliItem * cliItems, int cliNum);

protected:
    SOCKET sockfd;
    
private:
    void run(void);
    bool started;
};



#endif /* CLI_PARSER_HPP */
