#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int block;
    block = sync();
    printf(1, "sync : %d\n", block);
    
    exit();
}