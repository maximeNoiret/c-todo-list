/**
 * @author NOIRET Maxime
 */

#include <stdio.h>
#include <string.h>


void head_print(const unsigned page) {
  printf("Pg %d - %15s - Description\n", page, "Title");
  for (unsigned i = 0; i < 37; ++i)
    putc('-', stdout);
  putc('\n', stdout);
} // head_print


void task_print(int id, const unsigned char *title, const unsigned char *desc) {
  unsigned n;
  bool is_long = false;
  if (desc) {
    char *c = (char*)strchr((const char *)desc, '\n');
    if (c) {
      n = (unsigned)(c - (char*)desc);
      is_long = true;
    }
    else
      n = strlen((const char*)desc);
    if (n > 30) {
      n = 30;
      is_long = true;
    }
  }
  printf("%4d - %15s - %.*s", id, title, n, desc);
  if (is_long) puts("...");
  else putc('\n', stdout);
} // task_print

void task_details(const unsigned char *title, const unsigned char *desc) {
  puts((const char*)title);
  puts("-----");
  puts((const char*)desc);
} // task_details