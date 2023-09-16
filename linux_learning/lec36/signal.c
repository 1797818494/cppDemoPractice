#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_handler(int signum) {
  if (signum == SIGUSR1) {
    printf("Received SIGUSR1 signal.\n");
  }
}

int main() {
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("PID: %d\n", getpid());
  printf("Send SIGUSR1 signal using the 'kill' command to see the output.\n");

  while (1) {
    // 无限循环，等待信号
    sleep(1);
  }

  return 0;
}