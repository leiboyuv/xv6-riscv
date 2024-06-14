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
    int cd = channel_create();
    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }

    if (fork() == 0) {

        if (channel_put(cd, 42) < 0) {
            printf("Failed to put data in channel\n");
            exit(1);
        }

        channel_put(cd, 43); // Sleeps until cleared

    } else {
        int data;
        if (channel_take(cd, &data) < 0) { // 42
            printf("Failed to take data from channel\n");
            exit(1);
        }

        printf("%d\n", data);
        channel_take(cd, &data); // 43
        printf("%d\n", data);

        // data = channel_take(cd, &data); 
        // printf("THIS WILL NEVER BE PRINTED\n");    
    }
        return 0;
}
