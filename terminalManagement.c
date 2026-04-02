/**
 * @author NOIRET Maxime
 */

#include <stdio.h>
#include <string.h>


void head_print(const unsigned page) {
  printf("Page %d\n", page);
  puts("  Id -           Title - Descritpion\n"
       "------------------------------------");
} // head_print


void task_print(int id, const char *title, const char *desc) {
  unsigned n = 0;
  bool is_long = false;
  if (desc) {
    char *c = (char*)strchr(desc, '\n');
    if (c) {
      n = (unsigned)(c - desc);
      is_long = true;
    }
    else
      n = strlen(desc);
    if (n > 30) {
      n = 30;
      is_long = true;
    }
  }
  printf("%4d - %15s - %.*s", id, title, n, desc);
  if (is_long) puts("...");
  else putc('\n', stdout);
} // task_print

void task_details(const char *title, const char *desc) {
  puts((const char*)title);
  if (desc) {
    puts("-----");
    puts((const char*)desc);
  }
} // task_details