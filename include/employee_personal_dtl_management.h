/** @file employee_personal_dtl_management.h
 */
#ifndef employee_personal_dtl_management_H
#define employee_personal_dtl_management_H

/**  @brief Function prototypes to view menu for employee personal details
 *
 *  
 * This function is used to view menu for employee personal details
 * 
 *
 */
 
int emp_detail_mgmt (int);

/**  @brief Function prototypes to raise grievances
 *
 *  
 * This function is used to raise grievances
 * 
 *
 */
char* raise_grievances(int);

/**  @brief Function prototypes to update details by employee
 *
 *  
 * This function is used to update details by employee
 * 
 *
 */
char* update_employee(int );

/**  @brief Function prototypes to view the raised grievances
 *
 *  
 * This function is used to view the raised grievances
 * 
 *
 */
char* view_raised_grievances(int,int);

/**  @brief Function prototypes to give employee rating
 *
 *  
 * This function is used to give employee rating
 * 
 *
 */
char* employee_rating(int,int,char [],int);

/**  @brief Function prototypes to display employee information
 *
 *  
 * This function is used to display employee information
 * 
 *
 */
void emp_display(char []);

#endif
