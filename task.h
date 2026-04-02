/** @file task.h
 *  @brief Set of functions related to tasks.
 *  @author NOIRET Maxime
 */

#ifndef TASK_H_
#define TASK_H_

#include <sqlite3.h>
#include <stdlib.h>

/** @brief Creates the 'task' and 'free' tables in the database. Ignores if exists.
 *  @param[in] db Pointer to sqlite3 database
 *  @author NOIRET Maxime
 */
void create_table(sqlite3 *const db);

/** @brief Creates a task in the database.
 *  @param[in] db Pointer to sqlite3 database
 *  @param[in] title String containing the task title
 *  @param[in] desc  String containing the task description
 *  @author NOIRET Maxime
 */
void task_create(sqlite3 *const db, const char *const title,
                 const char *const desc);

/** @brief Gets a list of 10 tasks, starting from start_idx.
 *  @param[in] db Pointer to sqlite3 database
 *  @param[in] start_idx Index from which to start listing.
 *  @author NOIRET Maxime
 */
void get_tasks(sqlite3 *const db, const unsigned page);

/** @brief Gets the details of a single task.
 *  @param[in] db Pointer to sqlite3 database
 *  @param[in] idx Index of the task to get details for
 *  @author NOIRET Maxime
 */
void task_lookup(sqlite3 *const db, const int idx);

/** @brief Deletes a single or a range of tasks from the database.
 *  @param[in] db Pointer to sqlite3 database
 *  @param[in] idx_start Start of the indexes range to delete tasks
 *  @param[in] idx_end End of indexes range to delete tasks
 *  @author NOIRET Maxime
 */
void task_delete(sqlite3 *const db, const int idx_start, const int idx_end);

/** @brief Edits a task from the database.
 *  @param[in] db Pointer to sqlite3 database
 *  @param[in] idx Index of task to edit
 *  @param[in] title String containing the new title
 *  @param[in] desc  String containing the new description
 *  @author NOIRET Maxime
 */
void task_edit(sqlite3 *const db, const int idx, const char *const title,
               const char *const desc);

#endif // TASK_H_
