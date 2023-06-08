#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

char buf[8192];
char name[3];
char *echoargv[] = { "echo", "ALL", "TESTS", "PASSED", 0 };
int stdout = 1;

void
linktest(void)
{
  int fd;

  printf(1, "linktest\n");

  unlink("lf1");
  unlink("lf2");

  fd = open("lf1", O_CREATE|O_RDWR);
  if(fd < 0){
    printf(1, "create lf1 failed\n");
    exit();
  }
  if(write(fd, "hello", 5) != 5){
    printf(1, "write lf1 failed\n");
    exit();
  }
  close(fd);

  if(symlink("lf1", "lf2") < 0){
    printf(1, "link lf1 lf2 failed\n");
    exit();
  }

  fd = open("lf2", 0);
  if(fd < 0){
    printf(1, "open lf2 failed\n");
    exit();
  }

  unlink("lf1");

  if(open("lf1", 0) >= 0){
    printf(1, "unlinked lf1 but it is still there!\n");
    exit();
  }

  if(read(fd, buf, sizeof(buf)) != 5){
    printf(1, "read lf2 failed\n");
    exit();
  }
  close(fd);

  unlink("lf2");
  if(symlink("lf2", "lf1") >= 0){
    printf(1, "link non-existant succeeded! oops\n");
    exit();
  }

  if(symlink(".", "lf1") >= 0){
    printf(1, "link . lf1 succeeded! oops\n");
    exit();
  }

  if(symlink("lf2", "lf2") >= 0){
    printf(1, "link lf2 lf2 succeeded! oops\n");
    exit();
  }

  printf(1, "symlink test ok\n");
}

int main(void)
{
    printf(1, "symlink test starting\n");
    linktest();
    exit();
}