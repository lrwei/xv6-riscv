#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(const char *path, const char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: invalid directory %s\n", path);
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) {
      continue;
    }
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = '\0';

    if (stat(buf, &st) < 0) {
      fprintf(2, "find: can not stat %s\n", buf);
      continue;
    }
    if (!strcmp(name, de.name)) {
      printf("%s\n", buf);
    }
    if (st.type == T_DIR) {
      if (strcmp(".", de.name) && strcmp("..", de.name)) {
        find(buf, name);
      }
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    find(".", argv[1]);
  } else {
    find(argv[1], argv[2]);
  }

  exit(0);
}
