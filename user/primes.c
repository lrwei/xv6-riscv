#include "kernel/types.h"
#include "user/user.h"

void primes_impl(int _fd)
{
  int fd[2], pid;
  int prime, _;

  // sys_fileno();

  if (read(_fd, &prime, 4) == 0) {
    return;
  }
  printf("prime %d\n", prime);

  if (pipe(fd) < 0) {
    fprintf(2, "primes: pipe failed\n");
    return;
  }
  if ((pid = fork()) < 0) {
    fprintf(2, "primes: fork failed\n");
    return;
  }

  if (pid == 0) {
    close(_fd);
    close(fd[1]);
    primes_impl(fd[0]);
  } else {
    close(fd[0]);
    while (read(_fd, &_, 4)) {
      if (_ % prime != 0) {
        write(fd[1], &_, 4);
      }
    }
    close(fd[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  int fd[2], pid;

  pipe(fd);
  pid = fork();

  if (pid == 0) {
    close(fd[1]);
    primes_impl(fd[0]);
  } else {
    close(fd[0]);
    for (int i = 2; i < 35; i++) {
      write(fd[1], &i, 4);
    }
    close(fd[1]);
    wait(0);
  }

  exit(0);
}
