/**
* @file employee_management.c
*
* Performs the functionality relating to management of the employees
*
*/

/* Including the libraries */
#include<stdio.h>
#include<windows.h>
#include<mysql.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

/* Including all dependencies */
#include "..\include\validation.h"
#include "..\include\employee_management.h"
#include "..\include\admin_attendance.h"
#include "..\include\salary_management.h"
#include "..\include\employee.h"

/* including preprocessors */
#define MAX_YEAR 2020
#define MIN_YEAR 2010
#define MAX_MONTH 12
#define MIN_MONTH 1

/* Declaration of connection to MYSQL Database pointers */
MYSQL *conn2, *oo1,*oo3,*conn8,*connect6,*conn7;
char query[1500];

/* Database connection port number*/
int port6=3306;

/**
*
* \fn void sal_inc()
* \brief Assigning increments in salaries of the employees
*
* Allows the admin to assign increments in the salary of employees
* at the end of year based on the rating given by their manager
*
*/

void sal_inc(){
    char stmt[1500];
    /* Initializing the pointers to access data from MYSQL database*/
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    int year;
    time_t now;
    time(&now);

    /* Accessing the system time */
    struct tm *local = localtime(&now);
    year = local->tm_year + 1900;
    int next_year;
    next_year=year+1;

    char qry_id2[]={"select * from salary where sal_year='%d'"};
    sprintf(stmt,qry_id2,next_year);
    if (mysql_query(conn2,stmt)){
        printf(" Error: %s\n", mysql_error(conn2));
        printf("Failed to execute query.");
    }else{
        read = mysql_store_result(conn2);
        row = mysql_fetch_row(read);
        int count=mysql_num_rows(read);
        if(count>=1){
            printf("Salary is already incremented");
        }else{
            char qry_id1[]={"select * from emp_perfor join salary on emp_perfor.emp_id=salary.emp_id where emp_perfor.year='%d'"};
            sprintf(stmt,qry_id1,year);
            if (mysql_query(conn2,stmt)){
                printf(" Error: %s\n", mysql_error(conn2));
                printf("Failed to execute query.");
            }else{
                read = mysql_store_result(conn2);
                while((row = mysql_fetch_row(read))){
                    char sal_type[5];
                    int emp_id=atoi(row[0]);
                    float salary=atof(row[6]);
                    int rating=atoi(row[1]);
                    strcpy(sal_type,row[5]);

                    /* For different rating values different increments assigned*/
                    if(rating==1){
                        salary=salary+(salary*((float)2/100));
                    }else if(rating==2){
                        salary=salary+(salary*(4/100));
                    }else if(rating==3){
                        salary=salary+(salary*((float)6/100));
                    }else if(rating==4){
                        salary=salary+(salary*((float)8/100));
                    }else if(rating==5){
                        salary=(salary+(salary*((float)10/100)));
                    }else{
                        printf("Rating is appropriate");
                    }

                    /* Inserting the updated salary values to the database */

                    char qry_id[]={"insert into salary values('%d','%s','%f','%d')"};
                    sprintf(stmt,qry_id,emp_id,sal_type,salary,next_year);
                    if (mysql_query(conn2,stmt)){
                        printf(" Error: %s\n", mysql_error(conn2));
                        printf("Failed to execute query.");
                    }else{
                        printf("%d employee incrmented with salary=%f on rating=%d\n",emp_id,salary,rating);
                    }
                }
            }
        }
    }
}

/**
*
* \fn char* update_salary(int emp_id)
* \brief To update the salary by the admin
*
* Allows the admin to update the salary of the employees and managers.
* Salaries can be incremented and also modified for calculated salary
*
* \return string message for salary updation status
*/

char* update_salary(int emp_id){
    MYSQL_RES *read=NULL;
    MYSQL_ROW row=NULL;
    conn2 = mysql_init(NULL);
    char stmt[300];
    int sal_year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    sal_year = local->tm_year + 1900;
    mysql_real_connect(conn2, "localhost", "root", "1234","payroll", port6, NULL, 0);
    if(!conn2){
        return "Connection error";
    }else{
        int option;
        float salary;
        float  deductions;
        float net_pay;
        printf("To increment the salary of employee press 1. \nTo make changes to calculated salary press 2\n");
        scanf("%d",&option);
        if(option==1){
            char qry1[]={"select salary_type from salary where emp_id=%d and sal_year='%d'"};
            sprintf(stmt,qry1,emp_id,sal_year);
            if (mysql_query(conn2,stmt)){
                printf("		Error: %s\n", mysql_error(conn2));
                return ("		Failed to execute query.");
            }else{
                read = mysql_store_result(conn2);
                row = mysql_fetch_row(read);
                if(row==NULL){
                    return("		No data found for this id\n");
                }
                if(strcasecmp(row[0],"salaried")==0){
                    printf("%d is a salaried employee, enter salary per cheque i.e. half month salary\n ",emp_id);
                }else if(strcasecmp(row[0],"hourly")==0){
                    printf("%d is a hourly employee, enter  per hour salary\n ",emp_id);
                }
                scanf("%f",&salary);
                char qry2[]={"update salary set salary='%f' where emp_id='%d' and sal_year='%d'"};
                sprintf(stmt,qry2,salary,emp_id,sal_year);

                if (mysql_query(conn2,stmt)){
                    printf("		Error: %s\n", mysql_error(conn8));
                    return ("		Failed to execute query.");
                }else{
                    return ("\n\n Salary updated. \n");
                }
            }
        }else if(option==2){
            int year,month,week;
            int x=0;
            do{
                printf("Enter the  year you wish to update: \n");
                scanf("%d",&year);
                if(year<=MAX_YEAR && year>=MIN_YEAR){
                    x=1;
                }
                else{
                    printf("INVALID year, enter again\n");
                }
            }while(x==0);
            x=0;
            do{
                printf("Enter the month you wish to update: \n");
                scanf("%d",&month);
                if(month<=MAX_MONTH && month>=MIN_MONTH){
                    x=1;
                }
                else{
                    printf("INVALID month, enter again\n");
                }
            }while(x==0);
            do{
                printf("Enter salary you wish to update\n");
                scanf("%f",&salary);
                printf("Enter deductions for employee\n");
                scanf("%f",&deductions);
                net_pay=salary-deductions;
                if(net_pay<=0){
                    printf("Invalid try again");
                }
            }while(net_pay<=0);
            char qry[200];
            char qry2[]={"select salary_type from salary_cal where emp_id='%d' and year='%d'and month='%d'"};
            sprintf(stmt,qry2,emp_id,year,month,week);
            if (mysql_query(conn2,stmt)){
                printf("		Error: %s\n", mysql_error(conn2));
                return ("		Failed to execute query.");
            }else{
                read = mysql_store_result(conn2);
                row = mysql_fetch_row(read);
                if(row==NULL){
                    return("	No data found to update\n");
                }
                strcpy(qry,"update salary_cal set calculated_salary='%f',deductions='%f' ,net_pay='%f' where emp_id='%d' and year='%d'and month='%d'");
                int n = sprintf(stmt,qry,salary,deductions,net_pay,emp_id,year,month);
                if (mysql_query(conn2,stmt)){
                    printf("		Error: %s\n", mysql_error(conn2));
                    return ("		Failed to execute query.");
                }else{
                    return ("\n\n Salary updated. \n");
                }
            }
        }
    }
}

/**
* \fn void new_leave_detail()
* \brief Assigning the employees with leaves for a specific year.
*
* Allows the admin to assign leaves to an employee in
* a particular year.
*
*/

void new_leave_detail(){
    int t=0;
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    int paid_leave,med_leave;
    int emp_id,yr;
    char stmt[1500];
    do{
        t=0;
        printf("Enter the employee Id:\n");
        scanf("%d",&emp_id);
        char qry_id1[]={"select * from emp_details where emp_id=%d"};
        sprintf(stmt,qry_id1,emp_id);
        if (mysql_query(conn2,stmt)){
            printf(" Error: %s\n", mysql_error(conn2));
            printf("Failed to execute query.");
        }else{
            read = mysql_store_result(conn2);
            row = mysql_fetch_row(read);
            if(row==NULL){
                printf("No Such Employee ID found\n");
                t=0;
            }else{
                t=1;
            }
        }
    }while(t!=1);
    do{
        t=0;
        printf("Enter the year for leaves\n");
        scanf("%d",&yr);
        if(yr>2000 &&yr<9999){
            t=1;
        }
        else{
            t=0;
            printf("Wrong year\n");
        }
    }while(t!=1);
    char qry_id[]={"select * from leave_details where leave_year='%d' and emp_id=%d"};
    sprintf(stmt,qry_id,yr,emp_id);
    if (mysql_query(conn2,stmt)){
        printf(" Error: %s\n", mysql_error(conn2));
        printf("Failed to execute query.");
    }
    else{
        read = mysql_store_result(conn2);
        row = mysql_fetch_row(read);
        if(row==NULL){
            printf("Enter the paid leaves\n");
            scanf("%d",&paid_leave);
            printf("Enter the medical leaves\n");
            scanf("%d",&med_leave);
            char leave_qry[]={"insert into leave_details (emp_id,medical_leave,paid_leave,leave_year,balance_ML,balance_PL) values('%d','%d','%d','%d','%d','%d')"};
            sprintf(stmt,leave_qry,emp_id,med_leave,paid_leave,yr,med_leave,paid_leave);
            if (mysql_query(conn2,stmt)){
                printf(" Error: %s\n", mysql_error(conn2));
                printf("Failed to execute query.");
            }else{
                printf("\nRecord entered successfully\n");
            }
        }else{
            printf("Leaves already have in the databas... do you want to update with new leaves");
        }
    }
}

/**
* \fn void employee_reuests()
* \brief View and take decision on employee requests for attendance errors
*
* Allows the admin to view and take decision on requests raised by employees for
* attendance discrepancy
*
*/

void employee_reuests(){

    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    int num,u;
    char stmt[1500];
    char qry_id[]={"select * from request_form where req_status='0'"};
    if (mysql_query(conn2,qry_id)){
        printf(" Error: %s\n", mysql_error(conn2));
        printf("Failed to execute query.");
    }else{
        int i=0;
        read = mysql_store_result(conn2);
        printf("\n--------------------------------------------------------------------------------\n");
        while(row = mysql_fetch_row(read)){
            num = mysql_num_fields(read);
            printf("|");
            for(i = 0; i < num; i++){
                if(i==4){
                    if(atoi(row[i])==0){
                        printf("-Request Pending-");
                    }else if(atoi(row[i])==1){
                        printf("-Rejected-");
                    }else if(atoi(row[i])==2){
                        printf("-Accepted-");
                    }
                }else{
                    printf("%s|", row[i]);
                }
            }
            printf("\n--------------------------------------------------------------------------------\n");
        }
        printf("\n");
        int st,ch,st1;
        printf("Press 1 to update the status of an employee\n");
        printf("Press 2 to exit\n");
        scanf("%d",&st);
        if(st==1){
            do{
                int req_id;
                printf("Enter the request id to process\n");
                scanf("%d",&req_id);
                printf("Press 1 to Reject\n");
                printf("Press 2 to Accept\n");
                scanf("%d",&st1);
                if(st1==1){
                    char qry[]={"update request_form set req_status='%d' where req_id='%d'"};
                    sprintf(stmt,qry,st1,req_id);
                    //printf("%s",stmt);
                    if(mysql_query(conn2,stmt)){
                        printf("\n Database error please cotact to administrator");
                    }else{
                        printf("\n Request has processed");
                    }
                }else if(st1==2){
                    char qry[]={"update request_form set req_status='%d' where req_id='%d';"};
                    sprintf(stmt,qry,2,req_id);
                    if(mysql_query(conn2,stmt)){
                        printf("\n Database error please contact to administrator");
                    }else{
                        printf("\n Request has processed");
                    }
                }else{
                    printf("You cannot update the status of request with this value");
                }
                printf("\nDo you want to continue? Press 1 to continue else press any key");
                scanf("%d",&u);
            }while(u==1);
        }else if(st==2){
            printf("thanx");
        }else{
            printf("wrong choice");
        }
    }
}

/**
*
* \fn char* add_employee(char dept[],char desig[],int check,int request_id)
* \brief Allows the admin to add a new employee in the database.
*
* Allows the admin to add a new employee in emp_details and login_details in the database.
* This function is used either to add employee initially or in response to employee request by the manager.
* If responding to employee request by manager the input parameters will have auto-assigned values
* If adding employees initially the input parameter will have default values of "empty" for char type parameters
* and '0' for int type parameters and will be overwritten by user input
*
* @param[in] char dept[]   "empty" when adding employee initially
*						     department of the manager who rose employee request
* @param[in] char desig[]  "empty" when adding employee initially
*						     designation as requested by the manager who rose employee request
* @param[in] int check:     0 for adding employee initially
*						   	 1  for adding employee corresponding to employee request
* @param[in] int request_id unique request id corresponding to employee request raised
*
* \return string message for add employee status
*
*/

char* add_employee(char dept[],char desig[],int check, int request_id){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    struct employee{
        int id;
        char f_name[50];
        char l_name[50];
        char dep[50];
        char des[50];
        int age;
        char address1[45];
        char address2[45];
        char phonenumber[20];
        char gender[10];
        char email[100];
        float salary;
        char sal_type[10];
        int day;
        int month;
        int year;
    };
    struct employee emp;
    int x=0;
    char ch;
    int num;
    getchar();
    do{
        printf("Enter the first name of employee\n");
        gets(emp.f_name);
        x=notempty(emp.f_name);
    }while(x==0);
    x=0;
    do{
        printf("Enter the Last name of employee\n");
        gets(emp.l_name);
        x=notempty(emp.l_name);
    }while(x==0);
    x=0;

    if(check==1){
        strcpy(emp.dep,dept);
        strcpy(emp.des,desig);
    }else{
        do{
            printf("Enter the department of employee\n");
            gets(emp.dep);
            x=notempty(emp.dep);
        }while(x==0);
        x=0;

        do{
            printf("Enter the designation of employee\n");
            gets(emp.des);
            x=notempty(emp.des);
        }while(x==0);
        x=0;
    }
    printf("Enter the age of employee\n");
    scanf("%d",&emp.age);
    getchar();
    do{
        printf("Enter house/apartment number and street\n");
        gets(emp.address1);
        x=notempty(emp.address1);
    }while(x==0);
    x=0;

    do{
        printf("\nEnter city and country\n");
        gets(emp.address2);
        x=notempty(emp.address2);
    }while(x==0);
    x=0;
    do{
        printf("\nEnter gender Male , Female or Others\n");
        scanf("%s",emp.gender);
        if (strcasecmp(emp.gender,"Male")==0 || strcasecmp(emp.gender,"Female")==0 || strcasecmp(emp.gender,"Female")==0){
            x=1;
            break;
        }
    } while(x==0);
    x=0;
    do{
        printf("Enter email\n");
        scanf("%s",emp.email);
        x=valid_email(emp.email);
    }while(x==0);
    x=0;
    
    printf("\nEnter the Salary: \n");
    scanf("%.2f",&emp.salary);
    getchar();
    
	do{
    	printf("Enter the Salary type (Hourly/Salaried)? :\n");
        gets(emp.sal_type);
        if (strcasecmp(emp.sal_type,"Hourly")==0 || strcasecmp(emp.sal_type,"Salaried")==0){
            x=1;
            break;
        }
    } while(x==0);
    x=0;
    
	char desi[50];
	char salary_type[10];
	float sal;
    strcpy(desi,emp.des);
    strcpy(salary_type,emp.sal_type);
    sal = emp.salary;
    
    do{
        printf("Enter Phone number\n");
        scanf("%s",emp.phonenumber);
        x=valid_phone(emp.phonenumber);
    } while(x==0);
    x=0;

    getchar();
    do{
        printf("Enter the date of joining\n");
        printf("   Enter day of joining\n");
        scanf("%d",&emp.day);
        printf("   Enter month of joining\n");
        scanf("%d",&emp.month);
        printf("   Enter year of joining\n");
        scanf("%d",&emp.year);
        x=datevalid(emp.day,emp.month,emp.year);
    } while(x==0);

    /* inserting in emp_details table */
    char stmt[1500];
    conn8=mysql_init(NULL);
    mysql_real_connect(conn8, "localhost", "root", "1234","payroll", port6, NULL, 0);
    char qry[]={"insert into emp_details (name1,name2,department,designation,age,address_l1,address_l2,phonen,gender,email,day,month,year,emp_type) VALUES('%s','%s','%s','%s','%d','%s','%s','%s','%s','%s','%d','%d','%d','%s')"};
    if(conn8){
        sprintf(stmt,qry,emp.f_name,emp.l_name,emp.dep,emp.des,emp.age,emp.address1,emp.address2,emp.phonenumber,emp.gender,emp.email,emp.day,emp.month,emp.year,emp.sal_type);
        if (mysql_query(conn8,stmt)){
            printf(" Error: %s\n", mysql_error(conn8));
            return "Failed to execute query.";
        }else{
            conn7=mysql_init(NULL);
            mysql_real_connect(conn7, "localhost", "root", "1234","payroll", port6, NULL, 0);
            char qry2[]={"select last_insert_id()"};
            if(conn7){
                printf( "\n\n\nuser details added successfully\n\n\n\n\n");
            }else{
                printf(" Error: %s\n", mysql_error(conn7));
                return "Failed to execute query.";
            }
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(conn8));
    }
    /* inserting in the login details table */
    int emp_id;
    char qry2[]={"select max(emp_id) as id from emp_details"};
    if(conn8){
        int n=sprintf(stmt,qry2);
        mysql_query(conn8,stmt);
        read = mysql_store_result(conn8);
        if (mysql_query(conn8,stmt)){
            printf(" Error: %s\n", mysql_error(conn8));
            printf("Failed to execute query.");
        }else{
            int i=0;
            row = mysql_fetch_row(read);
            emp_id=atoi(row[0]);
            printf("%d\n",emp_id);
            mysql_free_result(read);
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(conn8));
    }
    oo1=mysql_init(NULL);
    mysql_real_connect(oo1, "localhost", "root", "1234","payroll", port6, NULL, 0);
    char password []="1234";
    char employee_type[30];
    printf("Enter the employee type -Admin,Employee or Manager\n");
    scanf("%s",employee_type);
    char qry3[]={"insert into login_details (emp_id,password,emp_type,status) VALUES('%d','%s','%s','%s')"};
    if(oo1){
        sprintf(stmt,qry3,emp_id,password,employee_type,"A");
        if (mysql_query(oo1,stmt)){
            printf(" Error: %s\n", mysql_error(oo1));
            return "Failed to execute query.";
        }else{
            if(check=1){
                oo3=mysql_init(NULL);
                mysql_real_connect(oo3, "localhost", "root", "1234","payroll", port6, NULL, 0);
                char request_status[]="Completed";
                char qry_update_request_status[]={"update employee_request set request_status='%s' where request_id='%d'"};
                if(oo3){
                    sprintf(stmt,qry_update_request_status,request_status,request_id);
                    if (mysql_query(oo3,stmt)){
                        printf(" Error: %s\n", mysql_error(oo3));
                        return "Failed to execute query.";
                    }else{
                        printf("Status updated successfully");
                    }
                }else{
                    printf("not connected");
                    printf("%s\n", mysql_error(oo3));
                }
            }
            
            printf("%s",add_salary(emp_id,salary_type,sal,desi));
            return "User Inserted";
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(oo1));
    }
}

/**
* \fn char* deactivate (int emp_id, int login_id)
* \brief Allows the admin to deactivate an existing employee.
*
* Allows the admin to deactivate any existing employee.
* The data entry in login_details is updated to "I":Inactive for the employee.
*
* @param[in] emp_id employee id of the employee who has to be deactivated
* @param[in] login_id login id of the employee who has to be deactivated
*
*  \return string message for deactivate employee status
*
*/

char* deactivate (int emp_id, int login_id){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    char status[20]="0" ;
    conn2=mysql_init(NULL);
    mysql_real_connect(conn2, "localhost", "root", "1234","payroll", port6, NULL, 0);
    snprintf(query,1500,"select emp_id,status FROM login_details where emp_id = '%d'",emp_id);
    /* send SQL query */
    if (mysql_query(conn2, query)){
        printf("Failed to execute quesry. Error: %s\n", mysql_error(conn2));
        return "Database Error";
    }else{
        res = mysql_store_result(conn2);
        row = mysql_fetch_row(res);
        if (row == NULL){
            return "No employee exists";
        }else{
            //strcpy(status,row[0]);
            if(strcmp("A",row[1])){
                return "Already de-activated";
            }else{
                if(atoi(row[0])!=login_id){
                    int dd,mm,yy;
                    int r;
                    /*do{
                        printf("\nEnter the date of leave of an employee:Format(dd/mm/yyyy)");
                        scanf("%d/%d/%d",&dd,&mm,&yy);
                        r=validate_date(dd,mm,yy);
                    }while(r!=1);*/
                    dd=01;
                    mm=01;
                    yy=2020;
                    char date[15];
                    sprintf(date,"%d/%d/%d", dd,mm,yy);
                    char qry[]="update  login_details set status='I', DOL='%s' where emp_id='%d'";
                    sprintf(query,qry,date,emp_id) ;
                    if (mysql_query(conn2, query)){
                        printf("Failed to execute query. Error: %s\n", mysql_error(conn2));
                    }else{
                        return "Employee deactivated";
                    }
                }else{
                    printf("Sorry! You can not deactivate the logged in account\n");
                    return "Same logged in user";
                }
            }
        }
    }
}

/**
* \fn void emp_display_details(char stmt[])
* \brief Allows to display the employee details
*
* Allows to display the employee details depending on input SQL query
*
* @param[in] char stmt[] SQL query that holds the input to display employee details
*
*/

void emp_display_details(char stmt[]){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_FIELD *field;
    int num;

    if(conn2){
        mysql_query(conn2,stmt);
        read = mysql_store_result(conn2);
        if (mysql_query(conn2,stmt)){
            printf(" Error: %s\n", mysql_error(conn2));
            printf("Failed to execute query.");
        }else{
            while ((row = mysql_fetch_row(read))){
                int num_fields;
                num_fields = mysql_num_fields(read);
                int i;
                for(i = 0; i < num_fields; i++){
                    printf(" |");
                    if (i == 0){
                        while(field = mysql_fetch_field(read)){
                            printf("%s |", field->name);
                        }
                        printf("\n");
                    }
                    printf("%s  ", row[i] ? row[i] : "NULL");
                }
            }
            printf("\n");
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(conn2));
    }
    return;
}

/**
*
* \fn char* modify_employee(int emp_id)
* \brief Allows the admin to modify employee details.
*
* Allows the admin to modify existing employee details.
* Depending on admin input different fields can be modified.
*
* @param[in] emp_id employee id whose details to be modified by the admin
*
* \return string message for modify employee status
*
*/

char* modify_employee(int emp_id){
    struct employee{
        int id;
        char f_name[50];
        char l_name[50];
        int age;
        char address1[45];
        char address2[45];
        char phonenumber[20];
        char gender[10];
        char email[100];
        char status[10];
        int day;
        int month;
        int year;
    };

    struct employee emp;
    int i=0;
    int x=0;
    char stmt[1500];
    getchar();
    printf("                Press 1 First Name\n");
    printf("                Press 2 Last Name\n");
    printf("                Press 3 Age\n");
    printf("                Press 4 Address Line 1\n");
    printf("                Press 5 Address Line 2\n");
    printf("                Press 6 Phone Number\n");
    printf("                Press 7 Gender\n");
    printf("                Press 8 Email\n");
    printf("                Press 9 Day of joining\n");
    printf("                Press 10 Month of joining\n");
    printf("                Press 11 Year of joining\n");
    printf("                Press 12 Status\n");
    printf("                Press 13 Exit\n");
    scanf("%d",&i);
    switch(i){
        case 1:{

            getchar();
            do{
                printf("Please provide the first name: \n");
                gets(emp.f_name);
                x=notempty(emp.f_name);
            }while(x==0);
            x=0;
            char qry[]={"update emp_details set name1='%s' where emp_id='%d'"};

            if(conn2){
                sprintf(stmt,qry,emp.f_name,emp_id);

                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 2:{
            getchar();
            do{
                printf("Please provide the last name: \n");
                gets(emp.l_name);
                x=notempty(emp.l_name);
            }while(x==0);
            x=0;
            char qry[]={"update emp_details set name2='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.l_name,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 3:{
            printf("Please provide the age: \n");
            scanf("%d",&emp.age);
            getchar();
            char qry[]={"update emp_details set age='%d' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.age,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 4:{
            int x=0;
            getchar();
            do{
                printf("Enter address line 1\n");
                gets(emp.address1);
                x=notempty(emp.address1);
            }while(x==0);

            char qry[]={"update emp_details set address_l1='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.address1,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 5:{
            int x=0;
            getchar();
            do{
                printf("Enter address line 2\n");
                gets(emp.address2);
                x=notempty(emp.address2);
            }while(x==0);

            char qry[]={"update emp_details set address_l2='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.address2,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 6:{
            int x=0;
            do{
                printf("Please provide the phone number: \n");
                scanf("%s",emp.phonenumber);
                getchar();
                x=valid_phone(emp.phonenumber);
            }while(x==0);

            char qry[]={"update emp_details set phonen='%s' where emp_id='%d'"};

            if(conn2){
                sprintf(stmt,qry,emp.phonenumber,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 7:{
            do{
                printf("\nEnter gender Male , Female or Others\n");
                scanf("%s",emp.gender);
                if (strcasecmp(emp.gender,"Male")==0 || strcasecmp(emp.gender,"Female")==0 || strcasecmp(emp.gender,"Female")==0){
                    x=1;
                    break;
                }
            } while(x==0);
            x=0;
            char qry[]={"update emp_details set gender='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.gender,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 8:{
            int x=0;
            do{
                printf("Please provide the email: \n");
                scanf("%s",emp.email);
                getchar();
                x=valid_email(emp.email);
            }while(x==0);
            char qry[]={"update emp_details set email='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.email,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 9:{
            printf("Please provide the day of joining: \n");
            scanf("%d",&emp.day);
            getchar();
            char qry[]={"update emp_details set day='%d' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.day,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 10:{
            printf("Please provide the month of joining: \n");
            scanf("%d",&emp.month);
            getchar();
            char qry[]={"update emp_details set month='%d' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.month,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 11:{
            printf("Please provide the year: \n");
            scanf("%d",&emp.year);
            getchar();
            char qry[]={"update emp_details set year='%d' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.year,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
        case 12:{
            int x=0;
            do{
    			printf("Enter the Salary type (Hourly/Salaried)? :\n");
        		gets(emp.status);
        		if (strcasecmp(emp.status,"Hourly")==0 || strcasecmp(emp.status,"Salaried")==0){
            		x=1;
            		break;
        		}
    		}while(x==0);
    		x=0;
    		
            char qry[]={"update emp_details set emp_type='%s' where emp_id='%d'"};
            if(conn2){
                sprintf(stmt,qry,emp.status,emp_id);
                if(mysql_query(conn2,stmt)){
                    printf(" Error: %s\n", mysql_error(conn2));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn2));
            }
            break;
        }
    }
    return NULL;
}

/**
* \fn int display_leaves(int emp_id,int year)
* \brief Display the leave details of the user (Admin,Employee,Manager).
*
* Display the leave details: including the total leaves assigned
* and remaining leaves for the year.
* Admin can view leave details of all employees as well
* employee and manager can view leaves for themseleves.
*
* @param[in] emp_id employee id whose leave details to display
* @param[in] year the year for which leaves to see
*
* return int corresponding to display leave status
*
*/

int display_leaves(int emp_id,int year){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_FIELD *field;
    int num;
    char stmt[1500];
    connect6=mysql_init(NULL);
    if(year<1900 || year>9999){
        printf("Invalid year\n");
        return 2;
    }
    mysql_real_connect(connect6,"localhost", "root", "1234","payroll", port6, NULL, 0);
    char qry[]={"select * from leave_details where emp_id='%d' and Leave_year='%d'"};
    if(connect6){
        int n = sprintf(stmt,qry,emp_id,year);
        mysql_query(connect6,stmt);
        read = mysql_store_result(connect6);
        if (mysql_query(connect6,stmt)){
            printf(" Error: %s\n", mysql_error(connect6));
            printf("Failed to execute query.");
            return 0;
        }else{
            row = mysql_fetch_row(read);
            if(row==NULL){
                printf("No data found for employee id %d ",emp_id);
                return 3;
            }else{
                while (row){
                    int num_fields;
                    num_fields = mysql_num_fields(read);
                    int i;
                    for(i = 0; i < num_fields; i++){
                        printf(" |");
                        if (i == 0){
                            while(field = mysql_fetch_field(read)){
                                printf("%s |", field->name);
                            }
                            printf("\n");
                        }
                        printf("    %s    ", row[i] ? row[i] : "NULL");
                    }
                    row = mysql_fetch_row(read);
                }
                return 1;
            }
        }
    }else{
        return 0;
    }
}

/*
* \fn int emp_management(int i,int emp_id)
* \brief The function to access different functionalities for admin.
*
* Access to various management functionalities for an admin
* Based on the admin input different functions will be called
*
* @param[in] i refers to the functionality chosen by the admin to perform based on switch statement
* @param[in] emp_id employee id of the admin who has logged in
*
* return 0 for connection failure
* 	      1 for successful execution
*/

int emp_management(int i,int emp_id){
    /* initializing connection to the database */
    conn2=mysql_init(NULL);
    int id;
    mysql_real_connect(conn2, "localhost", "root", "1234","payroll", port6, NULL, 0);

    /* checking database connectivity */
    if(!conn2){
        printf("Connection error");
        return 0;
    }else{
        /* accessing the functionality related to employee management by the admin */
        switch(i){
            case 1:{
                int dcsn=0;
                do{
                    printf("\n\n                Press 1 Add Employee\n");
                    printf("                Press 2 Modify Employee\n");
                    printf("                Press 3 Deactivate Employee\n");
                    printf("                Press 4 Display Employee\n");
                    printf("                Press 5 go back\n");
                    int k;
                    scanf("%d",&k);
                    switch(k){
                        case 1:{
                            printf("\n\n%s\n",add_employee("empty","empty",0,0));
                            break;
                        }
                        case 2:{
                            printf("Please specify the employee id: ");
                            scanf("%d",&id);
                            printf("%s",modify_employee(id));
                            break;
                        }
                        case 3:{
                            printf("Please specify the employee id: ");
                            scanf("%d",&id);
                            printf("%s",deactivate(id,emp_id));
                            break;
                        }
                        case 4:{
                            int j;
                            int emp_id;
                            char emp_depart_display[50];
                            char emp_desig_display[50];
                            printf("\n                Press 1 Display with Employee ID.");
                            printf("\n                Press 2 Display with Departments");
                            printf("\n                Press 3 Display with Designations");
                            printf("\n                Press 4 Display all Employees.\n");
                            scanf("%d",&j);
                            if (j == 1){
                                char stmt[1500];
                                printf("Enter the Employee ID: ");
                                scanf("%d",&emp_id);
                                char qry[] = {"select * from emp_details where emp_id = %d"};
                                int n = sprintf(stmt,qry,emp_id);
                                emp_display_details(stmt);
                            }else if (j == 2){
                                char stmt[1500];
                                printf("\nEnter the department: ");
                                scanf("%s",emp_depart_display);
                                char qry[] = {"select * from emp_details where department= '%s'"};
                                int n = sprintf(stmt,qry,emp_depart_display);
                                emp_display_details(stmt);
                            }else if (j == 3){
                                char stmt[1500];
                                printf("\nEnter the designation: ");
                                scanf("%s",emp_desig_display);
                                char qry[] = {"select * from emp_details where designation = '%s'"};
                                int n = sprintf(stmt,qry,emp_desig_display);
                                emp_display_details(stmt);
                            }else if (j == 4){
                                char stmt[1500];
                                printf("\nDisplaying all employees \n");
                                char qry[] = {"select * from emp_details"};
                                int n = sprintf(stmt,qry);
                                emp_display_details(stmt);
                            }else{
                                printf("Entered Incorrect number");
                                break;
                            }
                            break;
                        }
                        case 5:{
                            dcsn=1;
                            break;
                        }
                    }
                }while(dcsn!=1);
                return 1;
            }
            /* accessing the functionality related to leave management by the admin */
            case 2:{
                int dcsn=0;
                do{
                    printf("\n                Press 1 Add new leave details\n");
                    printf("                Press 2 View pending requests for attendance discrepancy\n");
                    printf("                Press 3 Display leaves\n");
                    printf("                Press 4 View pending requests for leave requests\n");
                    printf("                Press 5 Decision on leave requests\n");
                    printf("                Press 6 Go Back\n");
                    int ch;
                    scanf("%d",&ch);
                    if(ch==1){
                        new_leave_detail();
                    }else if(ch==2){
                        employee_reuests();
                    }else if(ch==3){
                        int id;
                        printf("Please provide the employee id: ");
                        scanf("%d",&id);
                        int dcsn1=1,year1;
                        do{
                            printf("Enter the leave year you wish to see: \n");
                            scanf("%d",&year1);
                            dcsn1=display_leaves(id,year1);
                        }while(dcsn1==2);
                    }else if(ch==4){
                        char status[20];
                        strcpy(status,"Pending");
                        view_pending_leave_requests(status);
                    }else if(ch==5){
                        int req_id,k;
                        printf("Please provide the request id to be addressed: ");
                        scanf("%d",&req_id);
                        printf("-------------------------Press 1: To Approve----------------------\n");
                        printf("-------------------------Press 2: To Reject----------------------\n");
                        scanf("%d",&k);
                        decision_leave_request(req_id,k);
                    }else if(ch==6){
                        dcsn=1;
                    }else{
                        printf("\nWrong choice\n");
                    }
                }while(dcsn!=1);
                return 1;
            }
            /* accessing the functionality related to attendance management by the admin */
            case 3:{
                int dcsn;
                do{
                    printf("\n                Press 1 create attendance month\n");
                    printf("                Press 2 to add daily attendance attendance\n");
                    printf("                Press 3 Display attendance\n");
                    printf("                Press 4 Update attendance\n");
                    printf("                Press 5 Go back...\n");
                    int ch;
                    scanf("%d",&ch);
                    if(ch==5){
                        dcsn=1;
                    }else{
                        dcsn=0;
                        attend_mgmt(ch);
                    }
                }while(dcsn!=1);
                return 1;
            }

            /* accessing the functionality related to salary management by the admin */
            case 4:{
                int dcsn=0;
                do{
                    printf("\n              Press 1 Compute salary\n");
                    printf("                Press 2 Display salary\n");
                    printf("                Press 3 Update salary\n");
                    printf("                Press 4 Go Back....\n");
                    int i;
                    scanf("%d",&i);
                    switch(i){
                        case 1:{
                        	emp_sal_mgmt();
                        	break;
                        }
                        case 2:{
                        	printf("Enter id of the employee\n");
                        	int e_id;
                        	scanf("%d",&e_id);
	                        int option;
    	                    printf("\n                Press 1 if you want to see all pay slips\n");
        	                printf("                Press 2 if you want to see pay slips for particular pay period\n");
            	            printf("                Press 3 To display Per-Hour/Per-Month salary\n");
                	        printf("                Press 4 Go back...\n");
                    	    scanf("%d",&option);
                        	int k=display_salary(e_id,option);
	                        break;
	                    }
                        case 3:{
	                        printf("Enter employee id of employee, you wish to change salary for: \n");
    	                    int i;
        	                scanf("%d",&i);
            	            printf("%s",update_salary(i));
                	        break;
                    	}
						case 4:{
	                        dcsn=1;
    	                    break;
    	                }
                        default:{
        	                printf("Wrong option\n");
            	            break;
            	        }
                	}
                }while(dcsn!=1);
                return 1;
            }

            /* accessing the functionality related to department management by the admin */
            case 5:{
                int dcsn=0;
                do{
                    int i=0;
                    printf("\n                Press 1 View pending requests to add employee\n");
                    printf("                Press 2 Add employee to the department\n");
                    printf("                Press 3 Go Back......\n");
                    scanf("%d",&i);
                    switch(i){
                        case 1:{
                            char status[10];
                            strcpy(status,"Pending");
                            printf("%s",view_pending_requests(status));
                            break;
                        }
                        case 2:{
                            int req_id;
                            printf("Please provide the request id to be addressed: ");
                            scanf("%d",&req_id);
                            printf("%s",add_employee_department(req_id));
                            break;
                        }
                        case 3:{
                            dcsn=1;
                            break;
                        }
                        default:{
                            printf("\nWrong choice\n");
                        }
                    }
                }while(dcsn!=1);
                return 1;
            }
            case 6:{
                int dcsn=0;
                do{
                    printf("                Press 1 Increment Salary\n");
                    printf("                Press 2 Go Back......\n");
                    int cho;
                    scanf("%d",&cho);
                    if(cho==1){
                        sal_inc();
                    }else if(cho==2){
                        dcsn=1;
                    }else{
                        printf("Wrong input");
                    }
                }while(dcsn!=1);
                return 1;
            }

            /* accessing the functionality related to Grievance & Redresaal management by the admin */
            case 7:{
                int dcsn=0;
                do{
                    int gri_choice;
                    printf("                Press 1 Grievances Readressal\n");
                    printf("                Press 2 Go back.....\n");
                    scanf("%d",&gri_choice);
                    if (1 == gri_choice){
                        int choice;
                        int dcsn1=0;
                        do{
                            printf("		Press 1 Raise Grievance\n");
                            printf("		Press 2 View Grievances\n");
                            printf("		Press 3 Go back...\n");
                            scanf("%d",&choice);
                            if (1 == choice){
                                printf("%s",raise_grievances(emp_id));
                            }else if(2 == choice){
                                int g_choice;
                                printf("		Press 1 View Grievances by employee id\n");
                                printf("		Press 2 View all Grievances\n");
                                scanf("%d",&g_choice);
                                if (choice == 1){
                                    int gri_choice;
                                    printf("Enter the employee id to see their grievances: \n");
                                    scanf("%d",&gri_choice);
                                }
                                view_raised_grievances(g_choice,gri_choice);
                            }else if(3 == choice){
                                dcsn1=1;
                            }else{
                                printf("\t\t Wrong Choice Entered.\n");
                            }
                        }while(dcsn1!=1);
                    }else if(2 == gri_choice){
                        dcsn=1;
                    }else{
                        printf("\t\t Wrong Choice entered.\n");
                    }
                }while(dcsn!=1);
                return 1;
            }

            case 8:{
                change_password(emp_id);
                return 1;
            }
            case 9:{
                return 2;
            }
            default:{
                printf("wrong input");
                return 1;
            }
        }
    }
}
