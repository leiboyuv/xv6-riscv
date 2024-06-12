#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
    int cd0 = channel_create();
    printf("1st channel id is: %d\n", cd0);
    channel_create();
    int cd2 = channel_create();
    printf("3rd channel id isL %d\n", cd2);
    return 0;
}
