#include "task.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_index(sqlite3 *db) {
  // TODO: implement getting the min index from 'free'
  //       OR, get MAX(idx) + 1 from 'task'
  return 0;
} // get_index

void create_table(sqlite3 *db) {
  sqlite3_exec(db,
               "CREATE TABLE IF NOT EXISTS task ("
               "idx INTEGER PRIMARY KEY,"
               "title TEXT NOT NULL,"
               "desc TEXT);"
               "CREATE TABLE IF NOT EXISTS free ("
               "idx INTEGER PRIMARY KEY);",
               NULL, NULL, NULL);
  return;
} // create_table

void task_create(sqlite3 *db, char *title, char *desc) {
  sqlite3_stmt *statement;
  sqlite3_prepare_v2(db,
                     "INSERT INTO task (title, desc) "
                     "VALUES (?, ?);",
                     -1, &statement, NULL);
  sqlite3_bind_text(statement, 1, title, strlen(title), SQLITE_STATIC);
  if (desc)
    sqlite3_bind_text(statement, 2, desc, strlen(desc), SQLITE_STATIC);
  else
    sqlite3_bind_null(statement, 2);

  int return_code = sqlite3_step(statement);
  if (return_code != SQLITE_DONE) {
    fprintf(stderr,
            "Create Database Error.\n"
            "Statement Step didn't return SQLITE_DONE\n"
            "Expected %d, got %d\n%s\n",
            SQLITE_DONE, return_code, sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    exit(-1);
  }

  sqlite3_finalize(statement);
} // task_create

void task_print(int id, const unsigned char *title, const unsigned char *desc) {
  printf("%d - %s\n"
         "\t%s\n",
         id, title, desc);
} // task_print

void get_tasks(sqlite3 *db, unsigned start_idx) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db,
                              "SELECT idx, title, desc FROM task "
                              "LIMIT 10 OFFSET ?;",
                              -1, &statement, NULL);
  sqlite3_bind_int(statement, 1, (int)start_idx);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return;
  }

  int return_code = sqlite3_step(statement);
  if (return_code == SQLITE_DONE) {
    puts("No tasks.");
    sqlite3_finalize(statement);
    return;
  }

  for (; return_code == SQLITE_ROW; return_code = sqlite3_step(statement)) {
    int id = sqlite3_column_int(statement, 0);
    const unsigned char *title = sqlite3_column_text(statement, 1);
    const unsigned char *desc = sqlite3_column_text(statement, 2);
    task_print(id, title, desc);
  }

  sqlite3_finalize(statement);
} // get_tasks

void task_delete(sqlite3 *db, int idx_start, int idx_end) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db,
                              "DELETE FROM task "
                              "WHERE idx BETWEEN ? AND ?;",
                              -1, &statement, NULL);
  sqlite3_bind_int(statement, 1, idx_start);
  sqlite3_bind_int(statement, 2, idx_end);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Delete Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return;
  }

  if (sqlite3_step(statement) != SQLITE_DONE) {
    fprintf(stderr, "Delete Step Error: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    exit(-1);
  }
  // Add all indexes into "free"
  for (int idx = idx_start; idx <= idx_end; ++idx) {
    sqlite3_finalize(statement);
    rc = sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO free VALUES (?);", -1,
                            &statement, NULL);
    sqlite3_bind_int(statement, 1, idx);
    if (rc != SQLITE_OK) {
      fprintf(stderr, "Add free Preparation Error: %s\n", sqlite3_errmsg(db));
      sqlite3_finalize(statement);
      return;
    }
    if (sqlite3_step(statement) != SQLITE_DONE) {
      fprintf(stderr, "Add free Step Error: %s\n", sqlite3_errmsg(db));
      sqlite3_finalize(statement);
      return;
    }
  }

  sqlite3_finalize(statement);
} // task_delete

void task_edit(sqlite3 *db, int idx, char *title, char *desc) {
  sqlite3_stmt *statement;

  int rc = sqlite3_prepare_v2(db,
                              "UPDATE task SET title = ?, desc = ? "
                              "WHERE idx = ?;",
                              -1, &statement, NULL);
  sqlite3_bind_text(statement, 1, title, strlen(title), SQLITE_STATIC);
  if (desc)
    sqlite3_bind_text(statement, 2, desc, strlen(desc), SQLITE_STATIC);
  else
    sqlite3_bind_null(statement, 2);
  sqlite3_bind_int(statement, 3, idx);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Editing - Problem during preparation: %s\n",
            sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return;
  }

  if (sqlite3_step(statement) != SQLITE_DONE) {
    fprintf(stderr, "Editing - Problem during step: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_finalize(statement);
} // task_edit
