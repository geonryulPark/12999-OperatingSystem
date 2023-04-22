#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "procheap.h"

void createHeap(Heap *minHeap, enum quelevel level) {
    minHeap->qtime = 2*level + 4;
    
    // priority queue initialize
    minHeap->capacity = NPROC;
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
        heap->size += 1;
        heap->rear = (heap->rear + 1) % heap->capacity;
        heap->procs[heap->rear] = p;
    } else {
        // cprintf("heapsize : %d\n", heap->size);
        // cprintf("push L2 : level, priority : %d %d\n", p->level, p->priority);
        int i;
        heap->size += 1;
        for (i = heap->size; i > 1 && heap->procs[i/2]->priority > p->priority; i /= 2) {
            heap->procs[i] = heap->procs[i/2];
        }
        heap->procs[i] = p;
        // cprintf("heapsize : %d\n", heap->size);
        for (int i = 1; i <= heap->size; i++) {
            // cprintf("after push, pid, priority, rtime : %d %d %d\n", heap->procs[i]->pid, heap->procs[i]->priority, heap->procs[i]->rtime);
        }
    }
}

struct proc* pop(Heap* heap) {
    if (heap->level != L2) {
        struct proc* ret = heap->procs[heap->front];
        heap->size -= 1;
        heap->front = (heap->front + 1) % heap->capacity;
        return ret;
    } else {
        int index, child = 1;
        struct proc *min_proc, *last_proc;

        min_proc = heap->procs[1];
        last_proc = heap->procs[heap->size];
        heap->size -= 1;

        for (index = 1; index*2 <= heap->size; index = child) {
            child = index*2;
            // if right child's priority is larger than left child's, 
            // candidate child change
            if ((child < heap->size && heap->procs[child+1]->priority < heap->procs[child]->priority)
                || (child < heap->size && (heap->procs[child+1]->priority == heap->procs[child]->priority) && (heap->procs[child+1]->etime < heap->procs[child]->etime))) {
                child += 1;
            }

            // child occupy root's place because child's priority < last_proc's priority
            if (last_proc->priority > heap->procs[child]->priority
                || ((last_proc->priority == heap->procs[child]->priority) && (last_proc->etime > heap->procs[child]->etime))) {
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

/*
void setPriority(Heap* heap, int pid, int cpriority) {
    int i = 1;
    int originPriority;

    while (i < NPROC && heap->procs[i]->pid != pid) i++;
    originPriority = heap->procs[i]->priority;
    heap->procs[i]->priority = cpriority;

    if (heap->procs[i]->level != L2) {
        heap->procs[i]->priority = cpriority;
    }
    else
    {

    if (originPriority < cpriority) {
        while (i > 1 && heap->procs[i]->priority < heap->procs[i/2]->priority) {
            swap(heap->procs[i/2], heap->procs[i]);
            i /= 2;
        }
    } else {
        int child = i;
        for (i = i; i*2 <= heap->size; i = child)
        {
            child = i*2;
            if (child < heap->size && heap->procs[child+1]->priority < heap->procs[child]->priority)
                child += 1;
            
            if (heap->procs[child]->priority < heap->procs[i]->priority) {
                swap(heap->procs[i], heap->procs[child]);
            }
            else {
                break;
            }
        }    
    }
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
*/
