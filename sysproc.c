#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uspinlock.h"
#include "condvar.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


/* Semaphore System Calls */

int
sys_semaphore_initialize(void)
{
  int i, v, m;
  if((argint(0, &i) < 0) || (argint(1, &v) < 0) || (argint(2, &m) < 0))
    return -1;
  if ((i < 0) || (v < 0) || (m < 0))
    return -1;

  init_semaphore(i, v, m);
  return 0;
}

int
sys_semaphore_aquire(void)
{
  int i;
  if(argint(0, &i) < 0)
    return -1;
  if (i < 0)
    return -1;

  aquire_semaphore(i);
  return 0;
}

int
sys_semaphore_release(void)
{
  int i;
  if(argint(0, &i) < 0)
    return -1;
  if (i < 0)
    return -1;

  release_semaphore(i);
  return 0;
}

/* helper System Calls for Producer Consumer program */

#define BUFFER_SIZE 5

int _buffer[BUFFER_SIZE];

int
sys_set_buffer(void)
{
  int idx, data;
  if((argint(0, &idx) < 0) || (argint(1, &data) < 0))
    return -1;
  _buffer[idx] = data;
  return 0;
}

int
sys_get_buffer(void)
{
  int idx;
  if(argint(0, &idx) < 0)
    return -1;
  return _buffer[idx];
}


/*Condition Varaible System Calls*/
int
sys_cv_wait(void)
{
  struct condvar* cv;
  int addr;
  if(argint(0, &addr) < 0)
    return -1;
  cv = (struct condvar*)addr;

  wait_condvar(cv);

  return 1;
}

int
sys_cv_signal(void)
{
  struct condvar* cv;
  int addr;
  if(argint(0, &addr) < 0)
    return -1;
  cv = (struct condvar*)addr;

  signal_condvar(cv);

  return 1;
}


/* Readers Writers System Calls */

#define READ_NUM 10
#define WRITE_NUM 5

void 
init_lock_uspinlock(struct uspinlock* lk)
{
  lk->locked = 0;
}

void 
lock_uspinlock(struct uspinlock* lk)
{
  while(xchg(&lk->locked, 1) != 0)
    ;
  __sync_synchronize();
}

void 
unlock_uspinlock(struct uspinlock* lk)
{
  __sync_synchronize();
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

//variables
struct uspinlock mutex, wrt;
int readcount;
int _data;

//init
int
sys_init_readers_writers(void)
{
  init_lock_uspinlock(&mutex);
  init_lock_uspinlock(&wrt);
  readcount = 0;
  _data = 0;
  return 1;
}

//reader
int
sys_start_reading(void)
{
  for (int i = 0; i < READ_NUM; i++)
  {
    lock_uspinlock(&mutex);
    readcount++;
    if (readcount == 1)
      lock_uspinlock(&wrt);
    unlock_uspinlock(&mutex);

    cprintf("Process pid=%d is READING for %dth time, data == %d, Readers Inside == %d\n", myproc()->pid, i+1, _data, readcount);

    lock_uspinlock(&mutex);
    readcount--;
    if (readcount == 0)
    {
      unlock_uspinlock(&wrt);
      cprintf("No Readers Left. Writers can proceed.\n");
    }
    unlock_uspinlock(&mutex);
  }

  return 1;
}

//writer
int
sys_start_writing(void)
{
  for (int i = 0; i < WRITE_NUM; i++)
  {
    lock_uspinlock(&wrt);

    cprintf("Process pid=%d is WRITING for %dth time, data <- %d\n", myproc()->pid, i+1, ++_data);

    unlock_uspinlock(&wrt);
  }

  return 1; 
}