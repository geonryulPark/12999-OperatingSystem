struct Queue {
    int front, rear, size, level;
    int haveToBeInFront;            // be used same as isSchedulerLock
    uint capacity;
    struct proc *array[NPROC];
    struct proc *reservedForFront[1];
};

struct Queue createQueue(int);
void enqueue(struct Queue *, struct proc *);
void enqueueFrontOfL0(struct Queue *, struct proc *);
struct proc* dequeue(struct Queue *);
struct proc* dequeueFrontOfL0(struct Queue *);
int isEmpty(struct Queue *);
void printAllQueue(struct Queue *);
