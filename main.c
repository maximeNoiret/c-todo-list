#include <stdio.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fputs("Wrong usage.\n", stderr);
    fprintf(stderr, "Correct usage: %s [command].", argv[0]);
    fprintf(stderr, "Commands available with %s help\n", argv[0]);
    return 1;
  }
  switch (argv[1][0]) {
  case 'h':
    puts("This is a ToDo list CLI utility written in C because uni is boring.\n"
         "Only the first character of commands are taken into account.\n"
         "  (Yes, 'cbloop' will count as 'create')\n");
    puts("Commands:\n"
         "  c create (title) [description]    Create a task.\n"
         "  g get [n]                         Show all or the nth last tasks.\n"
         "  d delete (title)                  Delete task with title\n"
         "  h help                            Show this menu.");
    break;
  case 'c':
    // TODO: handle_create();
    break;
  case 'g':
    // TODO: handle_get(int n);
    break;
  case 'd':
    // TODO: handle_delete(char *title);
    break;
  default:
    fputs("Unknown argument.\n", stderr);
    fprintf(stderr, "Commands available with %s help\n", argv[0]);
    return 1;
  }
  return 0;
}
