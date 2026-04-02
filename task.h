#ifndef TASK_H_
#define TASK_H_

#include <sqlite3.h>
#include <stdlib.h>

void create_table(sqlite3 *db);
void task_create(sqlite3 *db, const char *title, const char *desc);
void get_tasks(sqlite3 *db, const int start_idx);
void task_delete(sqlite3 *db, const int idx_start, const int idx_end);
void task_edit(sqlite3 *db, const int idx, const char *title, const char *desc);

#endif // TASK_H_
