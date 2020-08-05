/** @file Validation.h
 */
#ifndef Validation_H
#define Validation_H

/**  @brief Function prototypes to validate a phone number
 *
 *
 * This function is used to validate a phone number
 *
 *
 */

int valid_phone(char[]);

/**  @brief Function prototypes to validate a email id
 *
 *
 * This function is used to validate a email id
 *
 *
 */

int valid_email(char []);

/**  @brief Function prototypes to check if year is leap or not
 *
 *
 * This function is used to check if year is leap or not
 *
 *
 */
int isleap(int );
/**  @brief Function prototypes to Validate given date
 *
 *
 * This function is used to validate given date
 *
 *
 */
int datevalid(int , int , int );

/**  @brief Function prototypes to check if given data is empty or not
 *
 *
 * This function is used to check if given data is empty or not
 *
 *
 */
int notempty(char [] );

/**  @brief Function prototypes to validate attendance
 *
 *
 * This function is used to validate attendance
 *
 *
 */
int validate_attendance(char[4],int,int );

/**  @brief Function prototypes to validate current month
 *
 *
 * This function is used to validate current month
 *
 *
 */

int validate_current_month(int, int, int);

/**  @brief Function prototypes to validate date
 *
 *
 * This function is used to validate date
 *
 *
 */
int validate_date(int ,int ,int);


/**  @brief Function prototypes to check the leaves
 *
 *
 * This function is used to find out availability of leaves
 *
 *
 */
int leave_check(int,int, char[]);
/**  @brief Function prototypes to check the emp_type salaried or hourly
 *
 *
 * This function is used to check emp_type
 *
 */

int chk_emp_type(int);


#endif
