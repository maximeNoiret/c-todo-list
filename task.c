/**
 * @author NOIRET Maxime
 */

#include "task.h"
#include "terminalManagement.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned get_free(sqlite3 *const db) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db, "SELECT MIN(idx) FROM free;", -1, &statement, NULL);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Get Free Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return 0xDEADBEEF;
  }
  int step_code = sqlite3_step(statement);
  if (step_code == SQLITE_ROW) {
    unsigned idx = sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return idx;
  }
  sqlite3_finalize(statement);
  return 0xDEADBEEF;
}

unsigned get_max_idx(sqlite3 *const db) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db, "SELECT MAX(idx)+1 FROM task;", -1, &statement, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Get MAX Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return 0xDEADBEEF;
  }
  int step_code = sqlite3_step(statement);
  if (step_code == SQLITE_ROW) {
    unsigned idx = sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return idx;
  }
  sqlite3_finalize(statement);
  return 0xDEADBEEF;
}

unsigned get_index(sqlite3 *const db) {
  unsigned idx = get_free(db);
  if (idx == 0xDEADBEEF) {
    idx = get_max_idx(db);
  }
  return idx != 0xDEADBEEF ? idx : 1;
} // get_index

void create_table(sqlite3 *const db) {
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

void task_create(sqlite3 *const db, const char *title, const char *desc) {
  sqlite3_stmt *statement;
  sqlite3_prepare_v2(db,
                     "INSERT INTO task (idx, title, desc) "
                     "VALUES (?, ?, ?);",
                     -1, &statement, NULL);
  unsigned idx = get_index(db);
  sqlite3_bind_int(statement, 1, idx);
  sqlite3_bind_text(statement, 2, title, strlen(title), SQLITE_STATIC);
  if (desc)
    sqlite3_bind_text(statement, 3, desc, strlen(desc), SQLITE_STATIC);
  else
    sqlite3_bind_null(statement, 3);

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

  sqlite3_prepare_v2(db, "DELETE FROM free WHERE idx = ?;", -1, &statement, NULL);
  sqlite3_bind_int(statement, 1, idx);
  sqlite3_step(statement);
  sqlite3_finalize(statement);
} // task_create

void get_tasks(sqlite3 *const db, const int start_idx) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db,
                              "SELECT idx, title, desc FROM task "
                              "LIMIT 10 OFFSET ?;",
                              -1, &statement, NULL);
  sqlite3_bind_int(statement, 1, start_idx);

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
  head_print(start_idx / 10 + 1);
  for (; return_code == SQLITE_ROW; return_code = sqlite3_step(statement)) {
    int id = sqlite3_column_int(statement, 0);
    const char *title = (const char *)sqlite3_column_text(statement, 1);
    const char *desc = (const char *)sqlite3_column_text(statement, 2);
    task_print(id, title, desc);
  }

  sqlite3_finalize(statement);
} // get_tasks

void task_lookup(sqlite3 *const db, const int idx) {
  sqlite3_stmt *statement;
  int rc = sqlite3_prepare_v2(db,
                              "SELECT idx, title, desc FROM task "
                              "WHERE idx = ?;",
                              -1, &statement, NULL);
  sqlite3_bind_int(statement, 1, idx);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return;
  }
  int return_code = sqlite3_step(statement);
  if (return_code == SQLITE_DONE) {
    puts("Task doesn't exist.");
    sqlite3_finalize(statement);
    return;
  }
  const char *title = (const char *)sqlite3_column_text(statement, 1);
  const char *desc = (const char *)sqlite3_column_text(statement, 2);
  task_details(title, desc);
  sqlite3_finalize(statement);
}

void task_delete(sqlite3 *const db, const int idx_start, const int idx_end) {
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

void task_edit(sqlite3 *const db, const int idx, const char *const title,
               const char *const desc) {
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
