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
  create_table(db);
  switch (argv[1][0]) {
  case 'h': {
    puts("This is a ToDo list CLI utility written in C because uni is boring.\n"
         "Only the first character of commands is taken into account.\n"
         "  (Yes, 'cbloop' will count as 'create')\n"
         "Tasks are displayed using g (get).\n"
         "The format is as follows:\n"
         "INDEX - TITLE\n\tDESCRIPTION\n"
         "Commands:\n"
         "  c create <title> [description]       Create a new task.\n"
         "  g get [page]                         Show tasks. 10 tasks per "
         "page.\n"
         "  e edit <id> <title> [description]    Edit an existing task.\n"
         "  d delete <start> [end]               Delete a single task or a "
         "range.\n"
         "  h help                               Show this menu.");
    printf("\nExamples:\n"
           "  %s c \"Push Project\"\n"
           "  %s c \"Fix bug\" \"Fix the memory leak in the scraper.\"\n"
           "  %s e 1 \"new title\" \"new description\"\n"
           "  %s g 1         (show page 1 of tasks)\n"
           "  %s d 4         (delete task with index 4)\n"
           "  %s d 3 9       (delete tasks 3 to 9, both included)\n",
           argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
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
  case 'l': {
    if (argc < 3) {
      fputs("Incorrect use. Lookup requires one argument (id)\n", stderr);
      sqlite3_close(db);
      return 1;
    }
    unsigned idx = strtoul(argv[2], NULL, 10);
    task_lookup(db, idx);
    break;
  }
  case 'd': {
    if (argc < 3) {
      fputs("Incorrect use. Delete requires one argument (id)\n", stderr);
      sqlite3_close(db);
      return 1;
    }
    unsigned idx_start = strtoul(argv[2], NULL, 10);
    unsigned idx_end = (argc >= 4 ? strtoul(argv[3], NULL, 10) : idx_start);
    task_delete(db, idx_start, idx_end);
    break;
  }
  case 'e': {
    if (argc < 4) {
      fputs("Incorrect use. Editing requires 2 arguments (id, title)\n",
            stderr);
      sqlite3_close(db);
      return 1;
    }
    int idx = strtoul(argv[2], NULL, 10);
    task_edit(db, idx, argv[3], (argc >= 5 ? argv[4] : NULL));
    break;
  }
  default: {
    fputs("Unknown argument.\n", stderr);
    fprintf(stderr, "Commands available with %s help\n", argv[0]);
    sqlite3_close(db);
    return 1;
  }
  }

  sqlite3_close(db);
  return 0;
}
