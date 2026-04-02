/**
 * @author NOIRET Maxime
 */

#ifndef TASK_H_
#define TASK_H_

#include <sqlite3.h>
#include <stdlib.h>

/**
 * @brief Creates the 'task' and 'free' tables in the database. Ignores if
 * exists.
 * @param db Pointer to sqlite3 database
 */
void create_table(sqlite3 *const db);

/**
 * @brief Creates a task in the database.
 * @param db Pointer to sqlite3 database
 * @param title String containing the task title
 * @param desc  String containing the task description
 */
void task_create(sqlite3 *const db, const char *const title,
                 const char *const desc);

/**
 * @brief Gets a list of 10 tasks, starting from start_idx.
 * @param db Pointer to sqlite3 database
 * @param start_idx Index from which to start listing.
 */
void get_tasks(sqlite3 *const db, const int start_idx);

/**
 * @brief Gets the details of a single task.
 * @param db Pointer to sqlite3 database
 * @param idx Index of the task to get details for
 */
void task_lookup(sqlite3 *const db, const int idx);

/**
 * @brief Deletes a single or a range of tasks from the database.
 * @param db Pointer to sqlite3 database
 * @param idx_start Start of the indexes range to delete tasks
 * @param idx_end End of indexes range to delete tasks
 */
void task_delete(sqlite3 *const db, const int idx_start, const int idx_end);

/**
 * @brief Edits a task from the database.
 * @param db Pointer to sqlite3 database
 * @param idx Index of task to edit
 * @param title String containing the new title
 * @param desc  String containing the new description
 */
void task_edit(sqlite3 *const db, const int idx, const char *const title,
               const char *const desc);

#endif // TASK_H_
