#ifndef PTASKING_H
#define PTASKING_H

#ifdef WINDOWS
    #include "pthread.h"
#else /* linux */
    #include <pthread.h>
#endif
    #include "log.h"

/// pthreads thread wrapper
class PThread
{
public:
    volatile bool shutdown, running, detached;
    int stackSize;
    char threadName[16];
public:
    PThread(const char * name = NULL, bool detached = true, int stackSize = -1);
    virtual ~PThread(void);
    void start();
    virtual void stop(void);

protected:
    pthread_t thread_id;

private:
    static void *thread_func(void * const data);
    virtual void run(void) = 0;
};


#endif /* PTASKING_H */
