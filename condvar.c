#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uspinlock.h"
#include "condvar.h"

void
wait_condvar(struct condvar* cv)
{
    /*all wakeup with cv_signal*/
    sleep1(cv);

    /*only 1 wakeup with cv_signal*/
    //to do
}

void
signal_condvar(struct condvar* cv)
{
    /*all wakeup with cv_signal*/
    wakeup(cv);


    /*only 1 wakeup with cv_signal*/
    //to do
}