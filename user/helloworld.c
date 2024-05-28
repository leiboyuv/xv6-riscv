#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

    char* to_print = "Hello World xv6\n";

    write(1, to_print, strlen(to_print));

    exit(0,0);

}