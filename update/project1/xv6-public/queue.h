struct Queue {
    int front, rear, size, level;
    uint capacity;
    struct proc *array[NPROC];
};

struct Queue createQueue(int);
void enqueue(struct Queue *, struct proc *);
struct proc* dequeue(struct Queue *);
int isEmpty(struct Queue *);
void printAllQueue(struct Queue *);