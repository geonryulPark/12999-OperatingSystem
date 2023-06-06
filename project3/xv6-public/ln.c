#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 4){
    printf(2, "Usage: ln -h/s old new\n");
    exit();
  }
  printf(1, "argv[0]: %s\nargv[1]: %s\nargv[2]: %s\nargv[3]: %s\n", argv[0], argv[1], argv[2], argv[3]);
  if(!strcmp(argv[1], "-h")) {
    if(link(argv[2], argv[3]) < 0)
      printf(2, "hard link %s %s: failed\n", argv[2], argv[3]);
  }
  if(!strcmp(argv[1], "-s")) {
    if(symlink(argv[2], argv[3]) < 0)
      printf(2, "soft link %s %s: failed\n", argv[2], argv[3]);
  }
  exit();
}
