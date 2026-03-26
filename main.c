#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fputs("Wrong usage.\n", stderr);
    fprintf(stderr, "Correct usage: %s [command].", argv[0]);
    fprintf(stderr, "Commands available with %s help", argv[0]);
    return 1;
  }
  if (strcmp(argv[1], "help") == 0) {
    puts("Commands:\n"
         "\tcreate [title]        Create a task.\n"
         "\thelp                  Show this menu.");
  }

  return 0;
}
