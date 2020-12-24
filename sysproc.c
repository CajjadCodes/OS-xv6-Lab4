#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int
semaphore_initialize(void)
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
semaphore_aquire(void)
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
semaphore_release(void)
{
  int i;
  if(argint(0, &i) < 0)
    return -1;
  if (i < 0)
    return -1;

  release_semaphore(i);
  return 0;
}