#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  set_affinity_mask(1);
  fprintf(2, "Done");
  exit(0,0);
}
