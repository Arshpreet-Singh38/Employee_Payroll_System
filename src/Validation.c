/**
* @file Validation.c
*
* Perform the validations for most of the functions
*
*/

/* Including basic libraries */
#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<mysql.h>
#include <stdlib.h>
#include<time.h>
#include<ctype.h>

/* Include preprocessor for declaration of the functions. */
#include "..\include\validation.h"

/* Declaration of connection to MYSQL Database pointers and database port number */
MYSQL *conn9;
int port9=3306;

char query[1500];

/**
* \brief Removes the extra spacing
*
* It will delete or trim the extra space in the array of characters by calling this function
*
* @param[in] char *s Array of Character Pointer, pointing to the character of size char
*
* \return User_Type: Pointer to the array of character
*
*/

char *rtrim(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

/**
* \brief Validating the attendance
*
* Accessing the database,and compare the string (L), (PHL), (ML), (PL), (LWP), (WOFF), (A), (P)
* with the fields accessed from the excel file
*
* @param[in] char attnd[] Array of Character Pointer with the capacity to store 4 elements.
* @param[in] int yy Integer value for specifying the leave year
* @param[in] int emp_id Employee ID of the user
*
* \return User_Type: 0: For false Case or Error
* 					  1 or 2: Positive Response
*
*/

int validate_attendance(char attnd[4],int yy, int emp_id){
	/* Initializing pointers, to access data from MYSQL database */
	MYSQL_RES *read=NULL;
	MYSQL_RES *res2=NULL;
	MYSQL_ROW row=NULL;
	conn9=mysql_init(NULL);

	/*setting up the connection for *conn9 */
	mysql_real_connect(conn9, "localhost", "root", "1234","payroll", port9, NULL, 0);
	if(!conn9){
		printf("Connection error");
		return 0;
	}
	/* Called rtrim function to remove the extra spaces */
	attnd=rtrim(attnd);

	/* Comparing attnd array with the different types of leaves */
	if(strcmp(attnd,"L")==0 || strcmp(attnd,"PHL")==0 || strcmp(attnd,"ML")==0 || strcmp(attnd,"PL")==0 || strcmp(attnd,"LWP")==0 || strcmp(attnd,"WOFF")==0 || strcmp(attnd,"A")==0 || strcmp(attnd,"P")==0){
		if(strcmp(attnd,"ML")==0 ){
			return leave_check(yy,emp_id,"ML");
			/* Accessing the leave_details table to select leave year with the emp_id */

		}else if(strcmp(attnd,"PL")==0){
			/* Accessing the leave_details table to select leave year with the emp_id */
			return leave_check(yy,emp_id,"PL");
		}else{
			return 1;
		}
	}else
	{
		return 7;
	}
}
/**
* \brief Validating the current month
*
* Compairing the given date,month,year by the Admin against the actual number of days in that month
* Month is of 29 or 30 or 31 days in the given year
*
* @param[in] int dd Integer input from the user
* @param[in] int mm Integer input from the user
* @param[in] int yy Integer input from the user
*
* \return User_Type: 0: For Error of the invalid month
* 					  1: Positive case that month is valid
*
*/

int validate_current_month(int dd, int mm, int yy){
    int month, year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    month = local->tm_mon + 1;   	/*get month of year (0 to 11)*/
    year = local->tm_year + 1900;	/*get year since 1900*/
    if( (mm== month || mm == month-1) && yy == year){
        if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12)){
            return 1;
        }else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11)){
            return 1;
        }else if((dd>=1 && dd<=28) && (mm==2)){
            return 1;
        }
        else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0))){
            return 1;
        }else{
            printf("Day is invalid.\n");
            return 0;
        }
    }else{
        printf("\nPlease choose current month and year\n");
        return 0;
    }
}

/**
* \brief Validating the current date
*
* Compairing the given date,month,year by the Admin against the actual number of days in that month
* Month is of 29 or 30 or 31 days in the given year
*
* @param[in] int dd Integer input from the user
* @param[in] int mm Integer input from the user
* @param[in] int yy Integer input from the user
*
* \return User_Type: 0: For Error of the invalid date
* 					  1: Positive case that date is valid
*
*/

int validate_date(int dd,int mm,int yy){
    if(yy>=1900 && yy<=9999){
        /*check month*/
        if(mm>=1 && mm<=12){
            /*check days*/
            if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12)){
                return 1;
            }else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11)){
                return 1;
            }else if((dd>=1 && dd<=28) && (mm==2)){
                return 1;
            }else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0))){
                return 1;
            }else{
                printf("Day is invalid.\n");
                return 0;
            }
        }else{
            printf("Month is not valid.\n");
            return 0;
        }
    }else{
        printf("Year is not valid.\n");
    }
}
/**
* \brief Validating the Phone number
*
* Validating the given array of character that it is of length 10
* If not of length 10, gives an invalid message
*
* @param[in] char phone[] Array of character pointers
*
* \return User_Type: 0: For Error of the invalid phone number
* 					  1: Positive case the valid phone number
*/

int valid_phone(char phone[]){
    if(strlen(phone)!=10){
        printf("Invalid phone number\n");
        return 0;
    }else{
        int i;
        for(i=0; i<strlen(phone);i++){
            if(isalpha(phone[i]) != 0){
                printf("Invalid phone number\n");
                return 0;
            }
        }
        return 1;
    }
}

/**
* \brief Validating that string is empty or not
*
* Given string by the user is validated that it is empty or not
*
* @param[in] char num[] Array of characters given by the user
*
* \return User_Type: 0: For Error of the invalid character
* 					  1: Positive case for valid character
*
*/

int notempty(char num[] ){
    if(strlen(num)<=1){
        printf("Invalid value\n");
        return 0;
    }
    return 1;
}

/**
* \brief Validating the given string
*
* Given string by the user is validated that email is having with '@' and '.'
*
* @param[in] char test[] Array of characters given by the user
*
* \return User_Type: 0: For Error of the invalid email
* 					  1: Positive case for valid email
*
*/

int valid_email(char test[]){
    int     i;
    char    *pos1, *pos2;
    char    *ch;
    while (1){
        for (i = 0, ch = test; *ch; *ch++){
            if (*ch == '@'){
                pos1 = ch;  /*stores last @ address*/
                i++;
            }
        }
        pos2 = ch;
        /* If there is only one "@" */
        if (i == 1){
            while (pos1 - test && ch - pos1 > 1) {
                /* From the end of string to "." has to be atleast 2 chars */
                if ((pos2 - ch) > 2 && *ch == '.') {
                    return 1;
                }
                ch--;
            }
        }
        printf("Email wrong!\n");
        return 0;
    } /* End while */
    return 1;
}

/**
* \brief Validating the year as leap or not
*
* Test the year for leap year or not
*
* @param[in] int y Integer value by the user
*
* \return User_Type: 0: For Error of the invalid date
* 					  1: Positive case that date is valid
*
*/

int isleap(int y) {
    if((y % 4 == 0) && (y % 100 != 0) && (y % 400 == 0)){
        return 1;
    }else{
        return 0;
    }
}

/**
* \brief Validating the date,month and year
*
* It will validate the dat,month and year given by the user
*
* @param[in] int d Integer input from the user
* @param[in] int m Integer input from the user
* @param[in] int y Integer input from the user
*
* \return User_Type: 0: For Error of invalid input
* 					  1: Positive case for the value is valid
*
*/

int datevalid(int d, int m, int y) {
    if(y < 1800 || y > 9999)
    return 0;
    if(m < 1 || m > 12){
        printf("Invalid month\n");
        return 0;
    }

    if(d < 1 || d > 31){
        printf("Invalid day\n");
        return 0;
    }

    if( m == 2 ){
        if(isleap(y)){
            if(d <= 29)
            return 1;
            else{
                printf("Invalid day\n");
                return 0;
            }
        }
    }
    if ( m == 4 || m == 6 || m == 9 || m == 11 ){
        if(d <= 30)
        return 1;
        else{
            printf("Invalid day\n");
            return 0;
        }
        return 1;
    }
}



/**
* \brief Validating empType Salaried or Hourly
*
*
*
* @param[in] char temp_id employee id
*
* \return User_Type: 9: database error
* 					  1: Positive case for valid email
*
*/

int chk_emp_type(int emp_id){
	MYSQL_RES *read=NULL;
	MYSQL_RES *res=NULL;
	MYSQL_ROW row=NULL;
	char stmt[2500];
	char qry_id[2000];
	conn9=mysql_init(NULL);
	mysql_real_connect(conn9, "localhost", "root", "1234","payroll", port9, NULL, 0);
	strcpy(qry_id,"select *from salary where emp_id='%d' and sal_year=%d");
	sprintf(stmt,qry_id,emp_id,2020);
	if (mysql_query(conn9,stmt)){
        printf(" Error: %s\n", mysql_error(conn9));
        printf("Failed to execute query.");
        return 9;
    }else{
    	int i=0;
    	read = mysql_store_result(conn9);
        int count=mysql_num_rows(read);
        if(count>=1){

        	row = mysql_fetch_row(read);
        	if(strcmp(row[1],"Salaried")==0){
        		return 1;
			}else if(strcmp(row[1],"Hourly")==0){
				return 2;
			}
        }else{
        	printf("No Record");
        	return 0;
		}
    }

}

/**
 * \brief check the availability of leaves in the database for the particular year
 * 
 * The function checks for the availability of leaves 
 * when it sees a leave (ML,PL,LWP) in attendance data
 * and deducts from the leave balance accordingly
 *
 * @param[in] yy year in which leave requested and balance to be checked 
 * @param[in] emp_id unique user id
 * @param[in] leave type to be checked
 * 
 * \return int according to the execution status
 */

int leave_check(int yy,int emp_id, char le_type[]){
	MYSQL_RES *read=NULL;
	MYSQL_RES *res2=NULL;
	MYSQL_ROW row=NULL;
	conn9=mysql_init(NULL);

	/*setting up the connection for *conn9 */
	mysql_real_connect(conn9, "localhost", "root", "1234","payroll", port9, NULL, 0);
	if(!conn9){
		printf("Connection error");
		return 9;
	}
	char qry_id[]={"select * from leave_details where leave_year='%d' and emp_id=%d"};
	sprintf(query,qry_id,yy,emp_id);
	if (mysql_query(conn9,query)){
        printf(" Error: %s\n", mysql_error(conn9));
        printf("Failed to execute query.");
        return 9;
	}else{
		res2=mysql_store_result(conn9);
		int count=mysql_num_rows(res2);
		if(count>=1){
			if(strcmp(le_type,"ML")==0){
				row = mysql_fetch_row(res2);
				int l_balance=atoi(row[5]);
				if(l_balance==0){
					printf("Employee=%d has no medical leave. Please add some leaves\n",emp_id);
					return 2;
				}else{
					l_balance=l_balance-1;
					char qry_id[]={"update leave_details set balance_ML='%d' where leave_year='%d' and emp_id=%d"};
					sprintf(query,qry_id,l_balance,yy,emp_id);
					if (mysql_query(conn9,query)){
		        		printf(" Error: %s\n", mysql_error(conn9));
		        		printf("Failed to execute query.");
		        		return 9;
					}else{
                        return 1;
                    }
				}
			}else if(strcmp(le_type,"PL")==0){
				row = mysql_fetch_row(res2);
				int l_balance=atoi(row[6]);
				if(l_balance==0){
					printf("Employee=%d has no paid leave. Please add some leaves\n",emp_id);
					return 2;
				}else{
					l_balance=l_balance-1;
					char qry_id[]={"update leave_details set balance_PL='%d' where leave_year='%d' and emp_id=%d"};
					sprintf(query,qry_id,l_balance,yy,emp_id);
					if (mysql_query(conn9,query)){
		        		printf(" Error: %s\n", mysql_error(conn9));
		        		printf("Failed to execute query.");
		        		return 9;
					}else{
                        return 1;
                    }
				}
			}

		}else{
			printf("\nNo record in the database of employee=%d",emp_id);
			return 2;
		}
	}
			//printf("%s",attnd);
}

