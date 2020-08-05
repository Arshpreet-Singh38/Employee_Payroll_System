/** @file employee.h
 */
#ifndef employee_H
#define employee_H

/**  @brief Function prototypes to view employee menu
 *
 *  
 * This function is used to view employee menu
 * 
 *
 */
int employee(int );


 /**  @brief Function prototypes to raise grievance by employee
 *
 *  
 * This function is used to raise grievance by employee
 * 
 *
 */

char* raise_grievances(int);

 /**  @brief Function prototypes to change password by employee
 *
 *  
 * This function is used to change password by employee
 * 
 *
 */

void change_password(int);




 /**  @brief Function prototypes to get character
 *
 *  
 * This function is used to get character
 * 
 *
 */

int getch(void);




 /**  @brief Function prototypes to display salary for employee
 *
 *  
 * This function is used to display salary for employee
 * 
 *
 */


int display_salary(int,int);

 /**  @brief Function prototypes to view raised grievances
 *
 *  
 * This function is used to view raised grievances
 * 
 *
 */

char* view_raised_grievances(int,int);


 /**  @brief Function prototypes to change password
 *
 *  
 * This function is used to change password
 * 
 *
 */

char* change_pass(int ,char [45], char [45],char [45]);

#endif
