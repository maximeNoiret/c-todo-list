#include "task.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fputs("Wrong usage.\n", stderr);
    fprintf(stderr, "Correct usage: %s [command].", argv[0]);
    fprintf(stderr, "Commands available with %s help\n", argv[0]);
    return 1;
  }
  sqlite3 *db;
  sqlite3_open("task.db", &db);
  switch (argv[1][0]) {
  case 'h': {
    puts("This is a ToDo list CLI utility written in C because uni is boring.\n"
         "Only the first character of commands are taken into account.\n"
         "  (Yes, 'cbloop' will count as 'create')\n");
    puts("Commands:\n"
         "  c create (title) [description]    Create a task.\n"
         "  g get [n]                         Show all or the nth last tasks.\n"
         "  d delete (title)                  Delete task with title\n"
         "  h help                            Show this menu.");
    break;
  }
  case 'c': {
    if (argc < 3) {
      fputs("Incorrect Usage.\n"
            "Create requires 1 argument (title)\n"
            "There is 1 optional argument [description]\n",
            stderr);
      sqlite3_close(db);
      return 1;
    }
    task_create(db, argv[2], (argc >= 4 ? argv[3] : NULL));
    break;
  }
  case 'g': {
    unsigned page = (argc >= 3 ? strtoul(argv[2], NULL, 10) : 0);
    if (page > 0)
      --page;
    get_tasks(db, page * 10);
    break;
  }
  case 'd': {
    if (argc < 3) {
      fputs("Incorrest use. Delete requires one argument (id)\n", stderr);
      return 1;
    }
    unsigned id = strtoul(argv[2], NULL, 10);
    task_delete(db, id);
    break;
  }
  default: {
    fputs("Unknown argument.\n", stderr);
    fprintf(stderr, "Commands available with %s help\n", argv[0]);
    return 1;
  }
  }

  sqlite3_close(db);
  return 0;
}
