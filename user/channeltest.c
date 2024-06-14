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
    printf("0 channel id is: %d\n", cd0);
    channel_create();
    int cd2 = channel_create();
    printf("2 channel id is: %d\n", cd2);
    printf("put into 0 returned: %d\n", channel_put(cd0, 2));
    printf("put into 10 returned: %d\n", channel_put(10, 2));
    printf("put into -3 returned: %d\n", channel_put(-3, 2));
    printf("put into 0 again returned: %d\n", channel_put(cd0, 2));
    return 0;
}
