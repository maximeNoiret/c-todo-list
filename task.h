#ifndef TASK_H_
#define TASK_H_

#include <sqlite3.h>
#include <stdlib.h>

void create_table(sqlite3 *db);
void task_create(sqlite3 *db, char *title, char *desc);
void task_print(int id, const unsigned char *title, const unsigned char *desc);
void get_tasks(sqlite3 *db, unsigned start_idx);
void task_delete(sqlite3 *db, int idx_start, int idx_end);

#endif // TASK_H_
