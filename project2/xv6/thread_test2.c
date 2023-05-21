#include "types.h"
#include "user.h"

#define NTHREADS 5

void *thread_func(void *arg) {
  int thread_id = *(int *)arg;

  printf(1, "Thread %d started\n", thread_id);

  // Perform some work
  for (int i = 0; i < 5; i++) {
    printf(1, "Thread %d: %d\n", thread_id, i);
    sleep(100);
  }

  printf(1, "Thread %d finished\n", thread_id);

  thread_exit((void *)(2 * thread_id));
  return 0;
}

int main(void) {
  thread_t threads[NTHREADS];
  int thread_args[NTHREADS];
  void *retvals[NTHREADS];

  // Create threads
  for (int i = 0; i < NTHREADS; i++) {
    thread_args[i] = i + 1;
    thread_create(&threads[i], thread_func, &thread_args[i]);
  }

  // Join threads and retrieve return values
  for (int i = 0; i < NTHREADS; i++) {
    thread_join(threads[i], &retvals[i]);
    printf(1, "Thread %d return value: %d\n", i + 1, (int)retvals[i]);
  }

  exit();
}
