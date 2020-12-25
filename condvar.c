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
    sleep1(cv);
}

void
signal_condvar(struct condvar* cv)
{
    wakeup(cv);
}