#include "types.h"
#include "user.h"

#define NTHREADS 3

int counter = 0;
int barrier = 0;

void *thread_func(void *arg) {
  int thread_id = *(int *)arg;

  // Perform some work
  printf(1, "Thread %d: Work started\n", thread_id);
  sleep(500);

  // Enter barrier
  while (1) {
    while (barrier != 0)
      ;  // Spin lock

    barrier++;
    counter++;

    if (counter == NTHREADS) {
      barrier = 0;
      break;
    }

    barrier = 0;
  }

  // Wait for all threads to reach the barrier
  while (barrier != 0)
    ;  // Spin lock

  barrier++;

  // Perform additional work after barrier
  printf(1, "Thread %d: Work after barrier\n", thread_id);

  barrier = 0;

  thread_exit(0);
  return 0;
}

int main(void) {
  thread_t threads[NTHREADS];
  int thread_args[NTHREADS];

  // Create threads
  for (int i = 0; i < NTHREADS; i++) {
    thread_args[i] = i + 1;
    thread_create(&threads[i], thread_func, &thread_args[i]);
  }

  // Join threads
  for (int i = 0; i < NTHREADS; i++) {
    thread_join(threads[i], 0);
  }

  exit();
}
