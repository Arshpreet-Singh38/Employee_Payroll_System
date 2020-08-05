/**
*
* @file leave_request.c
*
* The file contains functions relating to the leave management
* The users can raise leave requests,the admin can view pending
* leave requests and take decisions on them accordingly
*
*/

/* Including libraries */
#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<mysql.h>
#include<stdlib.h>

/* Including dependencies */
#include "..\include\validation.h"

/* Declaration of connection to MYSQL Database pointers */
MYSQL *connect7,*connect8,*connect9,*connect10,*connect2,*connect1,*connect11;

/* Database connection port number*/
int port10=3306;

/**
*
* \fn char* leave_request(int emp_id,int dd,int mm,int yy,int no_of_days,char leave_type[])
* \brief Allow the users to raise a leave request
*
* The user can raise a request for a planned leave for the admin to review by specifying
* the start date for the leave, no of days and leave request type
*
* @param[in] int emp_id employee id of the user who wants to raise leave request
* @param[in] int dd     start day for the leave request
* @param[in] int mm     month of the start day for the leave request
* @param[in] int yy     year of the start day for the leave request
* @param[in] int no_of_days number of days for the leave required
* @param[in] char leave_type[] the type of leave required(ML/PL/LWP)
*
* \return string message corresponding to leave request status
*
*/

char* leave_request(int emp_id,int dd,int mm,int yy,int no_of_days,char leave_type[]){
    int x,r;
    int j=0;
    char no_days[10];

    /*Validating the date provided for leave request*/
    r=validate_date(dd,mm,yy);
    if(r!=1){
        printf("\nInvalid Date\n");
        return "Invalid Date";
    }

    /* Validating the length of the leave type*/
    if(strlen(leave_type)>10){
        printf("\nInvalid string length for leave_type\n");
        return "Invalid string length for leave_type";
    }

    /* Validating the leave type which must be from (ML/PL/LWP)*/
    if (strcasecmp(leave_type,"ML")==0 || strcasecmp(leave_type,"PL")==0 || strcasecmp(leave_type,"LWP")==0){
        j=1;
    }else{
        printf("\nInvalid Leave Type\n");
        return "Invalid Leave Type";
    }

    /* Initializing the pointers to access data from MYSQL database*/
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    char stmt[1500];

    /*Connection to database initialization*/
    connect11=mysql_init(NULL);
    mysql_real_connect(connect11,"localhost", "root", "1234","payroll", port10, NULL, 0);

    /* Checking for employee existence in the database */
    char qry_emp_id[]={"select * from emp_details where emp_id='%d'"};
    if(connect11){
        int n=sprintf(stmt,qry_emp_id,emp_id);
        mysql_query(connect11,stmt);
        read = mysql_store_result(connect11);
        row = mysql_fetch_row(read);
        if (row == NULL){
            printf("\nUser does not exist\n");
            return "User does not exist";
        }
    }

    connect7=mysql_init(NULL);
    mysql_real_connect(connect7,"localhost", "root", "1234","payroll", port10, NULL, 0);
    char status[15];

    /* Inserting the leave request in the leave_request table for the employee*/
    char qry_leave_request[]={"insert into leave_request (emp_id,start_day,leave_month,leave_year,no_of_days,leave_type,status) values ('%d','%d','%d','%d','%d','%s','%s')"};

    /* leave_request status will be pending when request is raised*/
    strcpy(status,"Pending");
    if(connect7){
        int n=sprintf(stmt,qry_leave_request,emp_id,dd,mm,yy,no_of_days,leave_type,status);
        if (mysql_query(connect7,stmt)){
            printf(" Error: %s\n", mysql_error(connect7));
            printf("\nFailed to execute query\n");
            return "Failed to execute query";
        }else{
            printf ("\nRequest raised successfully\n");
            return "Request raised successfully";
        }
    }else{
        printf("\nDatabase connection error\n");
        return "Database connection error";
    }
}

/**
*
* \fn char* view_pending_leave_requests(char status[])
* \brief Allows the admin to view all the leave requests that are still pending
*
* The admin can view the leave requests on which no action has been taken yet.
*
* @param[in] char status[] status of the requests on which no action taken yet
*
* \return string message corresponding to display status
*
*/

char* view_pending_leave_requests(char status[]){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;

    char stmt[1500];

    /* Retrieving the requests from database*/
    char qry_view_leave_req[]={"select * from leave_request where status='%s'"};
    connect8=mysql_init(NULL);
    mysql_real_connect(connect8, "localhost", "root", "1234","payroll", port10, NULL, 0);

    if(connect8){
        int n=sprintf(stmt,qry_view_leave_req,status);
        mysql_query(connect8,stmt);
        read = mysql_store_result(connect8);
        if (mysql_query(connect8,stmt)){
            printf(" Error: %s\n", mysql_error(connect8));
            printf("Failed to execute query.");
        }else{
            int i=0;
            int count1=0;
            int num=0;
            printf("\n");
            printf("REQ_ID  |	EMP_ID	|   START_DAY   |   LEAVE_MONTH |  LEAVE_YEAR   |   NO_OF_DAYS  |  Leave_Type  |	STATUS");
            printf("\n");
            printf("\n");

            /* Displaying all the pending requests*/
            while((row = mysql_fetch_row(read))){
            	count1++;
                num = mysql_num_fields(read);
                for(i = 0; i < num; i++){
                    printf("%s ", row[i]);
                    printf("	|	");
                }
                printf("\n");
            }
            if (count1==0){
            	printf("No pending requests found");
            	return "No pending requests found";
			}
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(connect8));
    }
    return "\n\n\nRequests displayed successfully";
}

/**
*
* \fn char* decision_leave_request(int req_id,int k)
* \brief Allows the admin to take can action on pending leave requests
*
* The admin can take an action on pending leave requests by specifying the request id
*
* @param[in] int req_id refers to the unique request id of the leave request
* @param[in] int k refers to the choice of decision Approval/Rejection
*
* \return string message confirmation for the decision taken
*
*/

char* decision_leave_request(int req_id,int k){
    /* Validating for incorrect input for decision to be taken*/
    if(k<1 || k>2){
        printf("\nInvalid input for k\n");
        return "Invalid input for k";
    }
    printf("\n");

    /* Admin input to approve or reject the leave request */
    switch(k){
        case 1:{
            MYSQL_RES *read=NULL;
            MYSQL_RES *res=NULL;
            MYSQL_ROW row=NULL;
            int remaining=0;
            int balance_ML=0;
            int balance_PL=0;
            char stmt[1500];

            /* Retrieving the data for leave request from database */
            char qry_decision[]={"select * from leave_request where request_id='%d'"};
            connect9=mysql_init(NULL);
            mysql_real_connect(connect9, "localhost", "root", "1234","payroll", port10, NULL, 0);

            if(connect9){
                int n=sprintf(stmt,qry_decision,req_id);
                mysql_query(connect9,stmt);
                read = mysql_store_result(connect9);
                row = mysql_fetch_row(read);
                if (mysql_query(connect9,stmt)){
                    printf(" Error: %s\n", mysql_error(connect9));
                    printf("Failed to execute query.");
                }else{
                    /* if no such leave request exists */
                    if(row==NULL){
                        printf("\nNo such pending leave request exists\n");
                        return "No such pending leave request exists";
                    }else{
                        int no_of_days=atoi(row[5]);
                        int year=atoi(row[4]);
                        int emp_id=atoi(row[1]);
                        char leave_type[20];
                        char leave_status[20];
                        strcpy(leave_type,row[6]);
                        strcpy(leave_status,row[7]);
                        printf("\n");

                        /* retrieving the leaves balance for the employee
                        * if admin decided to approve the request
                        * and there is not enough leave balance
                        * employee will get leave without pay
                        */

                        if(strcasecmp(leave_status,"Approved")==0){
                            printf("\nRequest is already approved\n");
                            return "Request is already approved";
                        }else{

                            char qry_retrieve_leaves[]={"select * from leave_details where Emp_id='%d' and Leave_year='%d'"};
                            connect10=mysql_init(NULL);
                            mysql_real_connect(connect10, "localhost", "root", "1234","payroll", port10, NULL, 0);

                            if(connect10){
                                int n=sprintf(stmt,qry_retrieve_leaves,emp_id,year);
                                mysql_query(connect10,stmt);
                                read = mysql_store_result(connect10);
                                if (mysql_query(connect10,stmt)){
                                    printf(" Error: %s\n", mysql_error(connect10));
                                    printf("Failed to execute query.");
                                }else{
                                    row = mysql_fetch_row(read);
                                    if(row==NULL){
                                    	printf("No leaves assigned for the leave request year in the database");
                                    	return "No leaves assigned for the leave request year in the database";
									}else{
									    balance_ML=atoi(row[5]);
                                        balance_PL=atoi(row[6]);
									}
                                }
                                printf("\n");
                            }else{
                                printf("not connected");
                                printf("%s\n", mysql_error(connect8));
                            }

                            if(strcasecmp(leave_type,"ML")==0){
                                if(balance_ML>no_of_days){
                                    balance_ML=balance_ML-no_of_days;
                                }else{
                                    remaining=no_of_days-balance_ML;
                                    balance_ML=0;
                                    if(balance_PL>remaining){
                                        balance_PL=balance_PL-remaining;
                                    }else{
                                        balance_PL=0;
                                    }
                                }
                            }else if(strcasecmp(leave_type,"PL")==0){
                                if(balance_PL>no_of_days){
                                    balance_PL=balance_PL-no_of_days;
                                }else{
                                    balance_PL=0;
                                }
                            }

                            /* updating the leave balance for the employee after availing the leaves as per leave request*/
                            char qry_update_balance[]={"update leave_details set Balance_ML='%d',Balance_PL='%d' where Emp_id='%d'"};
                            connect2=mysql_init(NULL);
                            mysql_real_connect(connect2, "localhost", "root", "1234","payroll", port10, NULL, 0);
                            if(connect2){
                                int n=sprintf(stmt,qry_update_balance,balance_ML,balance_PL,emp_id);
                                if (mysql_query(connect2,stmt)){
                                    printf(" Error: %s\n", mysql_error(connect2));
                                    printf("Failed to execute query.");
                                }else{
                                    //"Leave balance updated successfully\n");
                                }
                            }

                            /* changes the status of the leave request to Approved*/
                            char status[20]="Approved";
                            char qry_status_update[]={"update leave_request set status='%s' where request_id='%d'"};
                            connect1=mysql_init(NULL);
                            mysql_real_connect(connect1, "localhost", "root", "1234","payroll", port10, NULL, 0);
                            if(connect1){
                                int n=sprintf(stmt,qry_status_update,status,req_id);
                                if (mysql_query(connect1,stmt)){
                                    printf(" Error: %s\n", mysql_error(connect1));
                                    printf( "Failed to execute query.");
                                    break;
                                }else{
                                    printf("Leave request and Leave balance status updated successfully\n");
                                    return "Leave request and Leave balance status updated successfully";
                                }
                                break;
                            }
                            break;
                        }
                    }
                }
                break;
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(connect8));
            }
            break;
        }

        case 2:{
            MYSQL_RES *read=NULL;
            MYSQL_RES *res=NULL;
            MYSQL_ROW row=NULL;
            char stmt[1500];
            char leave_status[20];

            /* Retrieving the data for leave request from database */
            char qry_decision[]={"select * from leave_request where request_id='%d'"};
            connect9=mysql_init(NULL);
            mysql_real_connect(connect9, "localhost", "root", "1234","payroll", port10, NULL, 0);

            if(connect9){
                int n=sprintf(stmt,qry_decision,req_id);
                mysql_query(connect9,stmt);
                read = mysql_store_result(connect9);
                row = mysql_fetch_row(read);
                if (mysql_query(connect9,stmt)){
                    printf(" Error: %s\n", mysql_error(connect9));
                    printf("Failed to execute query.");
                }else{
                    /* if no such leave request exists */
                    if(row==NULL){
                        printf("\nNo such pending leave request exists\n");
                        return "No such pending leave request exists";
                    }else{
                        int emp_id=atoi(row[1]);
                        strcpy(leave_status,row[7]);

                        if(strcasecmp(leave_status,"Rejected")==0){
                            printf("\nRequest is already Rejected\n");
                            return "Request is already Rejected";
                        }else{
                            char status[20]="Rejected";
                            char qry_status_update[]={"update leave_request set status='%s' where request_id='%d'"};
                            connect1=mysql_init(NULL);
                            mysql_real_connect(connect1, "localhost", "root", "1234","payroll", port10, NULL, 0);
                            if(connect1){
                                int n=sprintf(stmt,qry_status_update,status,req_id);
                                if (mysql_query(connect1,stmt)){
                                    printf(" Error: %s\n", mysql_error(connect1));
                                    printf("Failed to execute query.");
                                }else{
                                    printf("\nRequest Status Updated successfully\n");
                                    return "Request Status Updated successfully";
                                }
                            }else{
                                printf("not connected");
                                printf("%s\n", mysql_error(connect8));
                            }
                        }
                        break;
                    }
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(connect9));
            }
            break;
        }
    }
}
