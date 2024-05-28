#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {

    printf("Current process is using %d bytes\n", memsize());

    char * ptr = malloc(20000);
    printf("After allocation the process is using %d bytes\n", memsize());

    free(ptr);
    printf("After release the process is using %d bytes\n", memsize());

    exit(0,0);
}