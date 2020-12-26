#include "types.h"
#include "user.h"
#include "uspinlock.h"
#include "condvar.h"


int main()
{
    struct condvar* cvptr = (struct condvar*)get_condvartest_cv();

    int pid = fork();
    if (pid < 0)
    {
        printf(1, "Error forking first child.\n");
    }
    else if (pid == 0)
    {
        sleep(1);
        printf(1, "Child 1 Executing\n");
        cv_signal(cvptr);
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
            cv_wait(cvptr);
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