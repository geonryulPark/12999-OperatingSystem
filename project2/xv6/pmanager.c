#include "types.h"
#include "stat.h"
#include "user.h"
#define BUFFERMAX 70
#define OPMAX     10
#define ARGMAX    60

void parse(char *, char *, char *, char *);
void delete(char *, char *, char *);
int
main(int argc, char *argv[])
{
    char buffer[BUFFERMAX];
    char op[OPMAX], arg1[ARGMAX], arg2[ARGMAX];
    char *argvs[] = {};
    int pid1, pid2, stacksize, limit;

    while (1) {
        /* read whole line */
        printf(1, ">> ");
        gets(buffer, BUFFERMAX);
        parse(buffer, op, arg1, arg2);

        if (!strcmp(op, "list")) {
            getproclist();
        } 
        else if (!strcmp(op, "kill")) {
            pid1 = atoi(arg1);
            if (kill(pid1) == 0) {
                printf(1, "kill %d process success\n", pid1);
            } else {
                printf(1, "kill %d process fail\n", pid1);
            }
        } 
        else if (!strcmp(op, "execute")) {
            if ((pid1 = fork()) == 0) {
                // Parent : will be killed directly
                if ((pid2 = fork()) == 0) {
                    // Grandson, will be orphan
                    stacksize = atoi(arg2);
                    if (exec2(arg1, argvs, stacksize) < 0) {
                        exit();
                    }
                } else {
                    exit();
                }
            } else {
                // GrandParent
                wait();
            }
        }
        else if (!strcmp(op, "memlim")) {
            pid1 = atoi(arg1);
            limit = atoi(arg2);
            
            if (setmemorylimit(pid1, limit) < 0) {
                printf(1, "setting process %d memory limit to %d fail\n", pid1, limit);
            }
            else {
                printf(1, "setting process %d memory limit to %d success\n", pid1, limit);
            }
        }
        else if (!strcmp(op, "exit")) {
            printf(1, "pmanager exit\n");
            break;
        }
        /* delete prior values */
        delete(op, arg1, arg2);
    }

    exit();
}

void
parse(char *buffer, char *op, char *arg1, char *arg2)
{
    char *source, *target;
    source = buffer;

    for (int i = 0; i < 3; i++) {
        if (i == 0) target = op;
        else if (i == 1) target = arg1;
        else target = arg2;

        while (*source != ' ' && *source != '\0') {
            if (*source == '\n')
                return;
            *target++ = *source++;
        }
        source++;
    }
}

void
delete(char *op, char *arg1, char *arg2)
{
    int i, j;
    for (i = 0; i < OPMAX; i++) {
        op[i] = '\0';
    } 

    for (j = 0; j < ARGMAX; j++) {
        arg1[j] = arg2[j] = '\0';
    }
}


