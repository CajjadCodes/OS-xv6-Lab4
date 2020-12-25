#include "types.h"
#include "user.h"
#include "uspinlock.h"
#include "condvar.h"


struct condvar cv;
struct condvar* cvptr = &cv;

int main()
{
    init_lock(&(cvptr->lk));

    int pid = fork();
    if (pid < 0)
    {
        printf(1, "Error forking first child.\n");
    }
    else if (pid == 0)
    {
        sleep(1);
        printf(1, "Child 1 Executing\n");
        lock(&(cvptr->lk));
        cv_signal(cvptr);
        unlock(&(cvptr->lk));
    }
    else
    {
        pid = fork();
        if (pid < 0)
        {
            printf(1, "Error forking second child.\n");
        }
        else if(pid == 0)
        {
            lock(&(cvptr->lk));
            cv_wait(cvptr);
            unlock(&(cvptr->lk));
            printf(1, "Child 2 Executing\n");
        }
        else
        {
            int i;
            for (i = 0; i < 2; i++)
                wait();
        }
    }
    exit();
}