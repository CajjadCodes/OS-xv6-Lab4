
// Semaphore Locks

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "semaphore.h"

struct semaphore semaphores[SEMAPHORE_ARRAY_SIZE];

void
init_semaphore(int i, int v, int m)
{
    initlock(&(semaphores[i].lk), "semaphore lock");
    semaphores[i].value = v - m;
    semaphores[i].max_value = v;
    for (int wait_proc_idx = 0; wait_proc_idx < NPROC; wait_proc_idx++)
        semaphores[i].waiting[wait_proc_idx] = 0;
    
}

void 
aquire_semaphore(int i)
{
    acquire(&(semaphores[i].lk));
    semaphores[i].value--;
    if (semaphores[i].value < 0) // must wait?
    {
        // add to queue
        for (int wait_proc_idx = 0; wait_proc_idx < NPROC; wait_proc_idx++) {
            if(semaphores[i].waiting[wait_proc_idx] == 0) {
                semaphores[i].waiting[wait_proc_idx] = myproc();
                break;
            }
        }
        // now sleep
        sleep(0, &(semaphores[i].lk)); // chan = 0 -> because we want to wake it up manually, not by a channel
    }
    release(&(semaphores[i].lk));
}

extern 

void
release_semaphore(int i)
{
    acquire(&(semaphores[i].lk));
    if (semaphores[i].value < semaphores[i].max_value)
        semaphores[i].value++;
    if (semaphores[i].value <= 0)
    {
        if (semaphores[i].waiting[0] != 0)  // is there some process waiting to wakeup?
        {
            //wake up queue head
            acquire_ptablelock();
            semaphores[i].waiting[0]->state = RUNNABLE;
            release_ptablelock();

            //shift queue to the left
            for (int wait_proc_idx = 0; wait_proc_idx < NPROC - 1; wait_proc_idx++) {
                semaphores[i].waiting[wait_proc_idx] = semaphores[i].waiting[wait_proc_idx + 1];
                if (semaphores[i].waiting[wait_proc_idx + 1] == 0)
                    break;
            }
        }  
    }
    release(&(semaphores[i].lk));
}