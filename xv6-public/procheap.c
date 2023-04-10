#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "procheap.h"
#define MAX_QUEUE_SIZE 1024+1

void createHeap(Heap *minHeap, enum quelevel level) {
    cprintf("createHeap\n");
    minHeap->qtime = 2*level + 4;
    
    // priority queue initialize
    minHeap->capacity = MAX_QUEUE_SIZE;
    minHeap->size = 0;
    minHeap->level = level;
    minHeap->procs[0] = 0x0;

    // normal queue
    minHeap->front = 0;
    minHeap->rear = -1;
}

int isFull(Heap* heap) {
    if (heap == 0x0) {
        panic("ERROR proheap.c_isFull() : Heap* heap is NULL\n");
        exit();
    }
    return heap->size >= heap->capacity;
}

int isEmpty(Heap* heap) {
    if (heap == 0x0) {
        panic("ERROR proheap.c_isEmpty() : Heap* heap is NULL\n");
        exit();
    }
    return heap->size == 0;
}

void push(Heap* heap, struct proc* p) {
    // cprintf("push: pid_%d, state_%d\n", p->pid, p->state);
    if (heap->level != L2) {
        // p->priority = 3; // don't care priority
        heap->size++;
        heap->rear = (heap->rear + 1) % heap->capacity;
        heap->procs[heap->rear] = p;
    } else {
        int i;
        for (i = ++heap->size; heap->procs[i/2]->priority > p->priority && i > 1; i /= 2) {
            heap->procs[i] = heap->procs[i/2];
        }
        heap->procs[i] = p;
    }
}

struct proc* pop(Heap* heap) {
    if (isEmpty(heap)) return 0;
    if (heap->level != L2) {
        struct proc* ret = heap->procs[heap->front];
        heap->size--;
        heap->front = (heap->front + 1) % heap->capacity;
        return ret;
    } else {
        int index, child = 1;
        struct proc *min_proc, *last_proc;

        min_proc = heap->procs[1];
        last_proc = heap->procs[heap->size--];

        for (index = 1; index*2 <= heap->size; index = child) {
            child = index*2;
            // if right child's priority is larger than left child's, 
            // candidate child change
            if (child < heap->size && heap->procs[child+1]->priority < heap->procs[child]->priority) {
                child++;
            }

            // child occupy root's place because child's priority < last_proc's priority
            if (last_proc->priority > heap->procs[child]->priority) {
                heap->procs[index] = heap->procs[child];
            } else {
                break;
            }
        }

        heap->procs[index] = last_proc;
        return min_proc;
    }
}

// in proc.c, we can decide whether proc exists in ptable
// if it exists, we can know what that's queue level in proc->level;
// And this func must be used in L2 queue
void swap(struct proc* parent, struct proc* child) {
    struct proc* tmp = parent;
    parent = child;
    child = tmp;
}


void setPriority(Heap* heap, int pid, int cpriority) {
    int i = 1;
    if (heap->level != L2) {
        panic("ERROR procheap.c_setPriority() : The MLFQ level is not L2 but func was called\n");
        exit();
    }
    if (cpriority == -1) {
        panic("ERROR procheap.c_setPriority() : saved Priority is -1\n");
        exit();
    }

    while (i < MAX_QUEUE_SIZE && heap->procs[i]->pid != pid) i++;
    heap->procs[i]->priority = cpriority;

    while (i > 0 && heap->procs[i]->priority < heap->procs[i/2]->priority) {
        swap(heap->procs[i/2], heap->procs[i]);
        i /= 2;
    }
}

void expiredTime(Heap MLFQ[], struct proc *p) {
    if (p->level != L2) {
        int l = p->level;
        while (l <= L2 && !isEmpty(&MLFQ[l])) l++;
        if (l == 3) {
            panic("ERROR procheap.c : at expiredTime()\n");
            exit();
        } push(&MLFQ[l], p);
        p->level = l;
        p->rtime = 0;
    } else {
        // MLFQ's level is L2
        if (p->priority > 0)
            p->priority--;
    }
}
