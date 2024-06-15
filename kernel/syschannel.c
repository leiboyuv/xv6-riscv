// Channels system calls

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"

extern int channel_create(void);
extern int channel_put(int id, int data);
extern int channel_take(int cd, uint64 data);
extern int channel_destroy(int cd);

uint64
sys_channel_create(void)
{
    return channel_create();
}

uint64
sys_channel_put(void)
{
    int cd, data;

    argint(0, &cd);
    argint(1, &data);

  return channel_put(cd, data);
}

uint64
sys_channel_take(void)
{
    int cd;
    uint64 data;

    argint(0, &cd);
    argaddr(1, &data);

  return channel_take(cd, data);
}

uint64
sys_channel_destroy(void)
{
  int cd;
  argint(0, &cd);

  return channel_destroy(cd);
}