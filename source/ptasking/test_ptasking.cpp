

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptasking.h"
#include "shared.h"
#include "log.h"

#ifdef WINDOWS /* win32 */
    #include <windows.h>
#else /* linux */

#endif

using namespace std;

class PThreadTest : public PThread
{
public:
    PThreadTest(const char * name) : PThread(name), flag(true) {}
    void run()
    {
        while(flag)
        {
            printf("[%p] Hey! I am %s.\n", &thread_id, this->threadName);
            sleep(500);
        }
    }
    void stop()
    {
        flag = false;
    }
private:
    bool flag;
};

int main(int argc, char ** argv)
{
    log_init();

    PThreadTest * t1 = new PThreadTest("Jim Green");
    PThreadTest * t2 = new PThreadTest("Mr. Wang");
    PThreadTest * t3 = new PThreadTest("Han Meimei");
    PThread * t4 = (PThread *)new PThreadTest("Li Lei");
    PThread * t5 = (PThread *)new PThreadTest("Kate Green");
    PThread * t6 = (PThread *)new PThreadTest("Miss Gao");

    t1->start();
    t2->start();
    t3->start();
    t4->start();
    t5->start();
    t6->start();

    sleep(3000);

    t1->stop();
    t2->stop();
    t3->stop();
    t4->stop();
    t5->stop();
    t6->stop();

    sleep(3000);

    return 0;
}
