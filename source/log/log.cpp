#include <string.h>
#include "log.h"
#include "cli.h"
#include "shared.h"



static unsigned int _log_level_ = LOG_LEVEL_NONE;

void set_log_lvl(unsigned int lvl)
{
    _log_level_ = lvl;
}


unsigned int get_log_lvl(void)
{
    return _log_level_;
}

static int _cli_set_log_lvl(int argc, const char ** argv)
{
    const char* p = NULL;
    unsigned int newLevel = 0;

    if ((argc < 2) || (argv == NULL))
    {
        printf("example: sdl aei\n");
        return FAIL;
    }

    p = argv[1];

    if (((strchr(p, 'n') != NULL) ||
         (strchr(p, 'N') != NULL)) &&
        (strlen(p) > 1))
    {
        return FAIL;
    }

    while (*p != 0)
    {
        switch (*p)
        {
            case 'e':
            case 'E':
                newLevel |= LOG_LEVEL_ERROR;
                break;

            case 'a':
            case 'A':
                newLevel |= LOG_LEVEL_API;
                break;

            case 'i':
            case 'I':
                newLevel |= LOG_LEVEL_INFO;
                break;

            case 't':
            case 'T':
                newLevel |= LOG_LEVEL_TRACE;
                break;

            case 's':
            case 'S':
                newLevel |= LOG_LEVEL_ASSERT;
                break;

            case 'w':
            case 'W':
                newLevel |= LOG_LEVEL_WARNING;
                break;

            case 'n':
            case 'N':
                newLevel = 0;
                break;

            case 'v':
            case 'V':
                newLevel = 0xFF;
                break;
         default:
                break;
        }
        p++;
    }

    set_log_lvl(newLevel);

    return OK;
}


static int _cli_get_log_lvl(int argc, const char **argv)
{
    char loglvl[32] = {0};
    if(_log_level_ & LOG_LEVEL_ERROR)   strcat(loglvl, "e");
    if(_log_level_ & LOG_LEVEL_API)     strcat(loglvl, "a");
    if(_log_level_ & LOG_LEVEL_INFO)    strcat(loglvl, "i");
    if(_log_level_ & LOG_LEVEL_ASSERT)  strcat(loglvl, "s");
    if(_log_level_ & LOG_LEVEL_TRACE)   strcat(loglvl, "t");
    if(_log_level_ & LOG_LEVEL_WARNING) strcat(loglvl, "w");
    if(_log_level_ & LOG_LEVEL_VERBOSE) strcat(loglvl, "v");
    if(!_log_level_)                    strcat(loglvl, "n");
    printf("<cli> current log level [%s].\n", loglvl);
    return OK;
}

CliItem log_cli[] = 
{
    {"sdl",         _cli_set_log_lvl,   "set log level", 0},
    {"gdl",         _cli_get_log_lvl,   "get log level", 0},
};

void log_init()
{
    _log_level_ = LOG_LEVEL_ERROR
                | LOG_LEVEL_API
                | LOG_LEVEL_INFO
                | LOG_LEVEL_ASSERT
                | LOG_LEVEL_TRACE
                | LOG_LEVEL_WARNING;

    Cli::cli_append(log_cli, sizeof(log_cli)/sizeof(CliItem));
}

