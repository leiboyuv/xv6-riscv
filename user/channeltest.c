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

    // int pid = getpid(); // for testing destroying channels on kill(pid)

    if (fork() == 0) {

        // kill(pid);   // for testing destroying channels on kill(pid)

        if (channel_put(cd, 42) < 0) {
            printf("Failed to put data in channel\n");
            exit(1);
        }

        channel_put(cd, 43); // Sleeps until cleared
        sleep(3);            // synchronize so 2nd take of the parent will happen befor destroying
        channel_destroy(cd);

    } else {
        int data;
        if (channel_take(cd, &data) < 0) { // 42
            printf("Failed to take data from channel #1\n");
            exit(1);
        } else {
            printf("%d\n", data);
        }
        if (channel_take(cd, &data) < 0) { // 43
            printf("Failed to take data from channel #2\n");
            exit(1);
        }
        else {
            printf("%d\n", data);
        }
        if (channel_take(cd, &data) < 0) {
            printf("Failed to take data from channel #3\n");    // This should be printed
            exit(1);
        }
        else {
            printf("%d\n", data);
        }

    }
        return 0;
}
