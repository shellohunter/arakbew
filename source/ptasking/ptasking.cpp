#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.hpp"
#include "ptasking.hpp"
#include "shared.hpp"


#if WINDOWS
    #include "pthread.h"
    #include <windows.h>
#else /* linux */
    #include <unistd.h>
    #include <pthread.h>
    #include <string.h>
#endif


PThread::PThread(const char * name, bool detached, int stackSize) :
    shutdown(0),
    running(0),
    detached(detached),
    stackSize(stackSize)
{
    if(strlen(name)<sizeof(threadName))
    {
        sprintf(threadName, "%s", name);
    }
    else
    {
        memset(threadName,0,sizeof(threadName));
    }
}

PThread::~PThread(void)
{
    stop();
}

void *PThread::thread_func(void * const data)
{
    PThread * const th = (PThread *) data;
    th->running = 1;
    th->run();
    pthread_exit((void*) data);
    th->running = 0;
    return NULL;
}

void PThread::start()
{
    shutdown = 0;
    running = 1;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (stackSize > 0) {
        pthread_attr_setstacksize(&attr, stackSize);
    }

    if (pthread_create(&thread_id, &attr, thread_func, this))
        LOG_ERROR("pthread_create() failed\n");

    pthread_attr_destroy(&attr);
    if (detached) {
        pthread_detach(thread_id);
    }
}

void PThread::stop(void)
{
    shutdown = 1;
    if (!detached) {
        pthread_join(thread_id, NULL);
    }
    running = 0;
}

