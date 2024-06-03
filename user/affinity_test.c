#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  set_affinity_mask(4);//-> 0 means run any cpu 1,2,4 run on specific cpus,3,5,6 run on 2 cpu, 7 on every cpus
  while(1){
  printf("process id: %d/n", getpid() );
  }
  exit(0,0);
}
