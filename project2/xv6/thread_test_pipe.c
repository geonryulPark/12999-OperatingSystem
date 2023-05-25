#include "types.h"
#include "user.h"

#define BUFFER_SIZE 20

void *thread_func(void *arg) {
  char message[BUFFER_SIZE];
//   int thread_id = *(int *)arg;

//   sprintf(message, "Thread %d: Hello from pipe\n", thread_id);

  int pipefd[2];
  pipe(pipefd);

  write(pipefd[1], message, strlen(message));
  close(pipefd[1]);

  thread_exit(0);
  return 0;
}

int main(void) {
  thread_t thread1, thread2;
  int thread_id1 = 1, thread_id2 = 2;

  thread_create(&thread1, thread_func, &thread_id1);
  thread_create(&thread2, thread_func, &thread_id2);

  thread_join(thread1, 0);
  thread_join(thread2, 0);

  int pipefd[2];
  pipe(pipefd);

  char buffer[BUFFER_SIZE];
  int n = read(pipefd[0], buffer, sizeof(buffer) - 1);

  if (n > 0) {
    buffer[n] = '\0';
    printf(1, "Main thread: Read from pipe: %s", buffer);
  }

  close(pipefd[0]);

  exit();
}
