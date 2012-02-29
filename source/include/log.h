
#ifndef LOG_HPP
#define LOG_HPP

#include <stdio.h>

#define LOG_LEVEL_NONE    (0x0)
#define LOG_LEVEL_ERROR   (0x1)
#define LOG_LEVEL_INFO    (0x1<<1)
#define LOG_LEVEL_API     (0x1<<2)
#define LOG_LEVEL_TRACE   (0x1<<3)
#define LOG_LEVEL_ASSERT  (0x1<<4)
#define LOG_LEVEL_WARNING (0x1<<5)
#define LOG_LEVEL_VERBOSE (0x1<<6)



#define LOG_TRACE(...) \
    do \
    { \
        if(get_log_lvl()&LOG_LEVEL_TRACE) \
            printf("<trace> %s,%s, L%d, %s.\n", __VA_ARGS__, __FUNCTION__,__LINE__,__FILE__); \
    }while(0)


#define LOG_API() \
    do \
    { \
        if(get_log_lvl()&LOG_LEVEL_API) \
            printf("<api> %s, L%d, %s.\n",__FUNCTION__,__LINE__,__FILE__); \
    }while(0)


#define LOG_INFO(...) \
    do \
    { \
        if(get_log_lvl()&LOG_LEVEL_INFO) \
        {\
            printf(__VA_ARGS__);\
        } \
    }while(0)

#define LOG_ERROR(...) \
    do \
    { \
        if(get_log_lvl()&LOG_LEVEL_ERROR) \
        {\
            printf(__VA_ARGS__);\
        } \
    }while(0)


#define ASSERT(_stmt) \
    do{ \
        if(get_log_lvl()&LOG_LEVEL_ASSERT) \
        { \
            if(!(_stmt)){ \
                printf("<assert> FAIL!\n+++++++++++ %s, L%d, %s.\n",#_stmt,__LINE__,__FUNCTION__);\
              while(1); \
            } \
        } \
    } \
    while(0)

#define LOG_WARNING(_stmt) \
        do \
        { \
            if(get_log_lvl()&LOG_LEVEL_WARNING) \
            {\
                printf(_stmt);\
            } \
        }while(0)

#define LOG_VERBOSE(...) \
        do \
        { \
            if(get_log_lvl()&LOG_LEVEL_VERBOSE) \
            {\
                printf(__VA_ARGS__);\
            } \
        }while(0)

#define CHK_FAIL(iRet,iRet_Return) \
    do {                                                                    \
        if(iRet < 0)                                                            \
        {                                                                   \
            LOG_ERROR(("file = %s, line = %d, reason = %d\n\r",      \
            __FILE__, __LINE__, iRet));                                         \
            return iRet_Return;                                                 \
        }                                                                   \
    } while (0)/* return value */


#ifdef __cplusplus
extern "C"{
#endif

void set_log_lvl(unsigned int lvl);
unsigned int get_log_lvl(void);
void log_init();

#ifdef __cplusplus
}
#endif
#endif /* LOG_HPP */

