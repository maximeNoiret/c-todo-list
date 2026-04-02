/** @file terminalManagement.h
 *  @brief Set of functions to manage output to terminal.
 *  @author NOIRET Maxime
 */

#ifndef TERMINALMANAGEMENT_H_
#define TERMINALMANAGEMENT_H_


/** @brief Prints the header before the list of tasks.
 *  @param[in] page Current page number.
 */
void head_print(const unsigned page);

/** @brief Prints a shortened version of a task.
 *  @param[in] id Index of the task
 *  @param[in] title Title of the task
 *  @param[in] desc Description of the task
 *  @author NOIRET Maxime
 */
void task_print(int id, const char *title, const char *desc);

/** @brief Prints the full title and description of a task.
 *  @param[in] title Title of the task
 *  @param[in] desc Description of the task
 *  @author NOIRET Maxime
 */
void task_details(const char *title, const char *desc);


#endif // TERMINALMANAGEMENT_H_