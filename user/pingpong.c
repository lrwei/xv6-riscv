#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  const char byte = 'W';
  char _ = '\0';
  int fd[2][2], pid;

  pipe(fd[0]);
  pipe(fd[1]);
  pid = fork();

  if (pid == 0) {
    close(fd[0][1]);
    close(fd[1][0]);
    read(fd[0][0], &_, 1);
    if (_ == byte) {
      printf("%d: received ping\n", getpid());
      write(fd[1][1], &byte, 1);
    }
  } else {
    close(fd[0][0]);
    close(fd[1][1]);
    write(fd[0][1], &byte, 1);
    read(fd[1][0], &_, 1);
    if (_ == byte) {
      printf("%d: received pong\n", getpid());
    }
  }

  exit(0);
}
