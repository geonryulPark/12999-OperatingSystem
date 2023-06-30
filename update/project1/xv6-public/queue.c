#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "queue.h"

struct Queue createQueue(int level) 
{
    struct Queue queue;
    queue.capacity = NPROC;
    queue.front = queue.size = 0;
    queue.rear = -1;
    queue.level = level;

    for (int i = 0; i < queue.capacity; i++)
        queue.array[i] = 0; 
    return queue;
}

int isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

int isLevelTwo(struct Queue *queue)
{
    return (queue->level == L2);
}

void enqueueLevelTwo(struct Queue *queue, struct proc *p)
{
    for (int i = 0; i < queue->capacity; i++) {
        if (queue->array[i] == 0) {
            queue->array[i] = p;
            break;
        }
    }
    queue->size = queue->size + 1;
}

void enqueueNormal(struct Queue *queue, struct proc *p)
{
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = p;
    queue->size = queue->size + 1;
}

void enqueue(struct Queue *queue, struct proc *p)
{
    if (isFull(queue))
        return;
    p->queueEnterTime = ticks;
    if (!isLevelTwo(queue)) {
        enqueueNormal(queue, p);
    } else {
        enqueueLevelTwo(queue, p);
    }
}

struct proc* dequeueNormal(struct Queue *queue)
{
    struct proc *item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int haveToChangeItem(struct proc *champion, struct proc *challenger)
{
    if (champion == 0 && challenger != 0)
        return 1;
    if (champion != 0 && challenger == 0)
        return 0;
    if (champion == 0 && challenger == 0)
        return 0;
    return (champion->priority > challenger->priority)
            || (champion->priority == challenger->priority && champion->queueEnterTime > challenger->queueEnterTime);
}

struct proc* dequeueLevelTwo(struct Queue *queue)
{   
    struct proc *item = queue->array[0];
    int toNullIndex = 0;
    for (int i = 0; i < queue->capacity; i++) {
        if (haveToChangeItem(item, queue->array[i])) {
            item = queue->array[i];
            toNullIndex = i;
        }
    }
    queue->array[toNullIndex] = 0;
    queue->size = queue->size - 1;
    return item;
}

struct proc* dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return 0;
    if (!isLevelTwo(queue)) {
        return dequeueNormal(queue);
    } else {
        return dequeueLevelTwo(queue);
    }
}

void printAllQueue(struct Queue *queue)
{
    for (int i = 0; i < queue->capacity; i++) {
        if (queue->array[i] == 0)
            continue;
        cprintf("pid : %d, level : %d, enter time : %d\n", queue->array[i]->pid, queue->array[i]->level, queue->array[i]->queueEnterTime);
    }
}
