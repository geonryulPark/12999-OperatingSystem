// Processor Heap used by priority queue

typedef struct Heapstruct {
  int capacity;
  int size;
  enum quelevel level;
  struct proc* procs[1024 + 1];

// for normal queue
  int front;
  int rear;

  uint qtime;               // quantum time of each queue
} Heap;

void createHeap(Heap *minHeap, enum quelevel level);
void push(Heap *heap, struct proc* proc);
struct proc* pop(Heap *heap);
int isFull(Heap *heap);
int isEmpty(Heap *heap);
void setPriority(Heap* heap, int pid, int cpriority);
void expiredTime(Heap MLFQ[], struct proc* p);