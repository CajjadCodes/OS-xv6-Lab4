#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uspinlock.h"
#include "condvar.h"

extern void lock_uspinlock(struct uspinlock* lk);

extern void unlock_uspinlock(struct uspinlock* lk);

void
wait_condvar(struct condvar* cv)
{
    lock_uspinlock(&(cv->lk));
    sleep1(cv);
    unlock_uspinlock(&(cv->lk));
}

void
signal_condvar(struct condvar* cv)
{
    lock_uspinlock(&(cv->lk));
    wakeup(cv);
    unlock_uspinlock(&(cv->lk));
}