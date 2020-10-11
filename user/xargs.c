#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
isspace(char c)
{
  const char spaces[] = " \t\n\r";

  // isspace('\0') == 0
  for (int i = 0; i < NELEM(spaces) - 1; i++) {
    if (c == spaces[i]) {
      return 1;
    }
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "usage: xargs command [args]\n");
    exit(1);
  }

  char **_argv = malloc(sizeof(char *) * (MAXARG + 1));

  _argv[argc] = 0;
  for (int i = 1; i < argc; i++) {
    _argv[i - 1] = argv[i];
  }

  char buf[512];
  while (strlen(gets(buf, 512)) != 0) {
    char *p = buf;

    while (isspace(*p)) {
      p++;
    }

    while (*p != '\0') {
      _argv[argc - 1] = p;

      while (*p != '\0' && !isspace(*p)) {
        p++;
      }
      *p++ = '\0';

      while (isspace(*p)) {
        p++;
      }

      if (fork() == 0) {
        exec(argv[1], _argv);
      } else {
        wait(0);
      }
    }
  }

  free(_argv);
  exit(0);
}
