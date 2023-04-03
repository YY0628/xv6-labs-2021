#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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


#ifdef LAB_PGTBL
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);
int
sys_pgaccess(void)
{
  uint64 start_va;
  int page_num;
  uint64 res_va;

  if (argaddr(0, &start_va) < 0)
  {
    printf("start_va wrong!");
    return -1;
  }
  if (argint(1, &page_num) < 0)
  {
    printf("page_num wrong!");
    return -1;
  }  
  if (argaddr(2, &res_va) < 0)
  {
    printf("res_va wrong!");
    return -1;
  }  
  if (page_num > 64)
  {
    panic("pgaccess: too much pages!\n");
    return -1;
  }

  
  struct proc *p = myproc();
  uint32 mask = 0;
  pte_t* pte;
  for (int cnt=0; cnt<page_num; cnt++, start_va+=PGSIZE)
  {
    if ( (pte=walk(p->pagetable, start_va, 0)) == 0)
      panic("pgaccess: pte should exit\n");
    
    if (*pte & PTE_A)
    {
      mask |= (1 << cnt);
      *pte = (*pte) & (~PTE_A);       // 清除
    }
  }

  // 传出用户空间
  return copyout(p->pagetable, res_va, (char*)&mask, sizeof(unsigned int));
}

#endif



uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



