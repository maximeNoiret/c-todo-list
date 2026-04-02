/**
 * @author NOIRET Maxime
 */

#ifndef TERMINALMANAGEMENT_H_
#define TERMINALMANAGEMENT_H_


/**
 * @brief Prints the header before the list of tasks.
 * @param page Current page number.
 */
void head_print(const unsigned page);

/**
 * @brief Prints a shortened version of a task.
 * @param id Index of the task
 * @param title Title of the task
 * @param desc Description of the task
 */
void task_print(int id, const char *title, const char *desc);

/**
 * @brief Prints the full title and description of a task.
 * @param title Title of the task
 * @param desc Description of the task
 */
void task_details(const char *title, const char *desc);


#endif // TERMINALMANAGEMENT_H_