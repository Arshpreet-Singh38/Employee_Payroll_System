/** @file employee_management.h
 */
#ifndef emp_management_H
#define emp_management_H

/**  @brief Function prototypes to view employee details
 *
 *  
 * This function is used to view employee details
 * 
 *
 */
void emp_display_details(char []);



/**  @brief Function prototypes to add employee by admin
 *
 *  
 * This function is used to add employee by admin
 * 
 *
 */


char* add_employee(char [],char [],int,int );




/**  @brief Function prototypes to view employee management menu
 *
 *  
 * This function is used to view employee management menu
 * 
 *
 */


int emp_management(int,int );




/**  @brief Function prototypes to modify employee details 
 *
 *  
 * This function is used to modify employee details
 * 
 *
 */


char* modify_employee(int);



/**  @brief Function prototypes to view pending requests 
 *
 *  
 * This function is used to view pending requests.
 * 
 *
 */


char* view_pending_requests(char []);





/**  @brief Function prototypes to add employee by department.
 *
 *  
 * This function is used to add employee by department.
 * 
 *
 */


char* add_employee_department(int );




/**  @brief Function prototypes to display leaves.
 *
 *  
 * This function is used to display leaves.
 * 
 *
 */


int display_leaves(int,int);



/**  @brief Function prototypes to view raised grievances
 *
 *  
 * This function is used to view raised grievances
 * 
 *
 */




char* view_raised_grievances(int,int);




/**  @brief Function prototypes to update salary by admin
 *
 *  
 * This function is used to update salary by admin
 * 
 *
 */


char* update_salary(int);




/**  @brief Function prototypes to raise leave request
 *
 *  
 * This function is used to raise leave request
 * 
 *
 */


char* leave_request(int,int,int,int,int,char []);



/**  @brief Function prototypes to view pending leave requests
 *
 *  
 * This function is used to view pending leave requests
 * 
 *
 */

char* view_pending_leave_requests(char []);



/**  @brief Function prototypes to change attendance
 *
 *  
 * This function is used to change attendance
 * 
 *
 */
char* attendance_change(int, int,int,int, char []);

/**  @brief Function prototypes to deactivate employee
 *
 *  
 * This function is used to deactivate employee
 *
 * 
 *
 */
char* deactivate (int , int);


/**  @brief Function prototypes to give decision on the leave request raised by employee
 *
 *  
 * This function is used to give decision on the leave request by admin raised by employee
 * 
 *
 */

char* decision_leave_request(int,int);


#endif


