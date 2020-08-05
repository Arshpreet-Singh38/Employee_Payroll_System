/**
* @file employee_personal_dtl_management.c
*
* It will display the details of the employee such as employee personal detail
* Employee feedback given by manager which help in increments or promotions
* Raised and View Grievances by the Admin,Manager and Employee and
* User have the option to be anonymous or not.
*
*/

/* Including basic libraries */
#include<stdio.h>
#include<windows.h>
#include<mysql.h>

/* including preprocessors */
#include "..\include\validation.h"
#include "..\include\employee_personal_dtl_management.h"
#include "..\include\employee.h"
#define YEAR 2020
#define MIN_YEAR 2018

/* Declaration of connection to MYSQL Database pointers and database port number */
MYSQL *conn3, *conn8, *conn9;
int port7=3306;

/**
* \brief Access the database and display the details of the employee.
*
* It will display the details of the employees.
*
* @param[in] char stmt: Query passed from emp_detail_mgmt to display the employee details
* and their rating given by the manager.
*
* \return Nothing because the data retrieved from the database is printing on the console screen.
*
*/

void emp_display(char stmt[]){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_FIELD *field;
    int num;
    conn3=mysql_init(NULL);
    mysql_real_connect(conn3, "localhost", "root", "1234","payroll", port7, NULL, 0);
    if(conn3){
        mysql_query(conn3,stmt);
        read = mysql_store_result(conn3);
        if (mysql_query(conn3,stmt)){
            printf(" Error: %s\n", mysql_error(conn3));
            printf("Failed to execute query.");
        }else{
            while (row = mysql_fetch_row(read)){
                int num_fields;
                num_fields = mysql_num_fields(read);
                int i;
                for(i = 0; i < num_fields; i++){
                    printf("|");
                    if (i == 0){
                        while(field = mysql_fetch_field(read)){
                            printf("%s|", field->name);
                        }
                        printf("\n");
                    }
                    printf(" %s", row[i] ? row[i] : "NULL");
                }
            }
            printf("\n");
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(conn3));
    }
}
/**
* \brief Access the database for their details to be modified by the employee
*
* The details of the employees can be modified by calling this function
* Use "emp_detail" table to fetch and modify the records
*
* @param[in] int emp_id: Employee ID of the user.
*
* \return User Type: User modified successfully: For positive case
*		   Not connected : Fail to establish the connection with database
*		   Failed to execute the query: Connection problem for execution of the query
*
*/

char* update_employee(int emp_id){
    conn3=mysql_init(NULL);
    int id;
    mysql_real_connect(conn3, "localhost", "root", "1234","payroll", port7, NULL, 0);
    struct employee{
        char address1[45];
        char address2[45];
        char phonenumber[20];
        char gender[10];
        char email[100];
    };

    struct employee emp;
    int i=0;
    char stmt[1500];
    printf("                Press 1 Address Line 1\n");
    printf("                Press 2 Address Line 2\n");
    printf("                Press 3 Phone Number\n");
    printf("                Press 4 Gender\n");
    printf("                Press 5 Email\n");
    printf("                Press 6 Exit\n");
    scanf("%d",&i);
    switch(i){
        case 1:{
            int x=0;
            do{
                printf("Enter address line 1\n");
                gets(emp.address1);
                x=notempty(emp.address1);
            }while(x==0);
            char qry[]={"update emp_details set address_l1='%s' where emp_id='%d'"};
            if(conn3){
                sprintf(stmt,qry,emp.address1,emp_id);
                if(mysql_query(conn3,stmt)){
                    printf(" Error: %s\n", mysql_error(conn3));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn3));
            }
            break;
        }
        case 2:{
            int x=0;
            do{
                printf("Enter address line 2\n");
                gets(emp.address2);
                x=notempty(emp.address2);
            }while(x==0);

            char qry[]={"update emp_details set address_l2='%s' where emp_id='%d'"};
            if(conn3){
                sprintf(stmt,qry,emp.address2,emp_id);
                if(mysql_query(conn3,stmt)){
                    printf(" Error: %s\n", mysql_error(conn3));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn3));
            }
            break;
        }
        case 3:{
            int x=0;
            do{
                printf("Please provide the phone number: \n");
                scanf("%s",emp.phonenumber);
                x=valid_phone(emp.phonenumber);
            }while(x==0);
            char qry[]={"update emp_details set phonen='%s' where emp_id='%d'"};

            if(conn3){
                sprintf(stmt,qry,emp.phonenumber,emp_id);
                if(mysql_query(conn3,stmt)){
                    printf(" Error: %s\n", mysql_error(conn3));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn3));
            }
            break;
        }
        case 4:{
            printf("Enter gender Male,Female or Others\n");
            scanf("%s",emp.gender);
            char qry[]={"update emp_details set gender='%s' where emp_id='%d'"};
            if(conn3){
                sprintf(stmt,qry,emp.gender,emp_id);
                if(mysql_query(conn3,stmt)){
                    printf(" Error: %s\n", mysql_error(conn3));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn3));
            }
            break;
        }
        case 5:{
            int x=0;
            do{
                printf("Please provide the email: \n");
                scanf("%s",emp.email);
                x=valid_email(emp.email);
            }while(x==0);

            char qry[]={"update emp_details set email='%s' where emp_id='%d'"};
            if(conn3){
                sprintf(stmt,qry,emp.email,emp_id);
                if(mysql_query(conn3,stmt)){
                    printf(" Error: %s\n", mysql_error(conn3));
                    return "Failed to execute query.";
                }else{
                    return "\n\n\n\nUser modified successfully\n\n\n\n";
                }
            }else{
                printf("not connected");
                printf("%s\n", mysql_error(conn3));
            }
            break;
        }
    }
}

/**
* \brief Raise grievances or complaints aginst others by Admin,Manager and EMployee
*
* It will raise a grievance or complaints against each other by Admin, Manager and Employee.
* User want to be anonymous, then emp_id is not inserted in the database.
* Description can be provided upto 200 characters for the grievances redressal.
*
* @param[in] int emp_id: Employee ID of the user.
*
* \return User Type: Grivance Raised: For positive case
*		   No Record found: For not finding the employee in the database
*		   Not connected or Failed to execute the query: For failure with the database
*
*/

char* raise_grievances(int e_id){
    conn8 = mysql_init(NULL);
    mysql_real_connect(conn8, "localhost", "root", "1234","payroll", port7, NULL, 0);

    if(!conn8){
        printf("Connection error");
        return "0";
    }else{
        char stmt[1500];
        char description[200];
        char choice[10];
        int x=0;
        getchar();

        do{
            printf("Redress the problem in 200 characters\n");
            gets(description);
            x=notempty(description);
        }while(x==0);

        printf("Do you want to anonymous? y/n: ");
        scanf("%s",choice);

        /**
        * Query: Insert into "grievances" table according to auto-generated response number
        * It will add emp_id in the database by asking the permission from the user
        */

        if ( 0 == strcasecmp(choice,"y")){
            char qry[]={"insert into grievances (response_number,emp_id,description) VALUES(NULL,NULL,'%s')"};
            sprintf(stmt,qry,description);
            if (mysql_query(conn8,stmt)){
                printf(" Error: %s\n", mysql_error(conn8));
                return "Failed to execute query.";
            }else{
                return "\n\n\n Grievance Raised. \n\n\n\n";
            }
        }else if (0 == strcasecmp(choice,"n")){
            char qry[]={"insert into grievances (response_number,emp_id,description) VALUES(NULL, %d,'%s')"};
            sprintf(stmt,qry,e_id,description);

            if (mysql_query(conn8,stmt)){
                printf(" Error: %s\n", mysql_error(conn8));
                return "Failed to execute query.";
            }else{
                return "\n\n\n Grievance Raised. \n\n\n\n";
            }
        }else{
            printf("Wrong choice.");
            return "1";
        }
    }
}

/**
* \brief Access the database to display the raised grievances by the users.
*
* It will display the raised grievances by the users. Admin,Manager,Employee have the option
* to view the grievances by employee id or see all raised grievances.
*
* @param [in] int choice Intger value input from the user
*			   int gri_chocie Integer value input as the ID of the user
*
* \return User Type: Not connected : Fail to establish the connection with database
*		 			  Failed to execute the query: Connection problem for execution of the query
*		              Employee ID not found or No Record with the query: For not finding the employee in the database
*				      Grievance Found for the employee ID or Grievance Found: For positive case
*
*/

char* view_raised_grievances(int choice, int gri_choice){
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_FIELD *field;

    conn8 = mysql_init(NULL);
    mysql_real_connect(conn8, "localhost", "root", "1234","payroll", 3306, NULL, 0);

    if(!conn8){
        printf("Connection error");
        printf("%s\n", mysql_error(conn8));
    }else{
        char stmt[1500];
        char qry1[]={"select * from emp_details where emp_id=%d"};
        sprintf(stmt,qry1,gri_choice);

        if (mysql_query(conn8,stmt)){
            printf("Error: %s\n", mysql_error(conn8));
            return ("Failed to execute query");
        }else{
            read = mysql_store_result(conn8);
            int num_rows = mysql_num_rows(read);
            if(num_rows<=0){
                printf("Employee ID not found\n");
                return ("Employee ID not found");
            }
        }
        if (choice==1){
            char qry[]={"select response_number, description from grievances where emp_id = %d"};
            sprintf(stmt,qry,gri_choice);
            if (mysql_query(conn8,stmt)){
                printf(" Error: %s\n", mysql_error(conn8));
                printf("Failed to execute query.");
            }else{
                read = mysql_store_result(conn8);
                int count_rows = mysql_num_rows(read);
                if (count_rows>0){
                    while (row = mysql_fetch_row(read)){
                        int num_fields;
                        num_fields = mysql_num_fields(read);
                        int i;
                        for(i = 0; i < num_fields; i++){
                            printf("|");
                            if (i == 0){
                                while(field = mysql_fetch_field(read)){
                                    printf("%s|", field->name);
                                }
                                printf("\n");
                            }
                            printf(" %s", row[i] ? row[i] : "NULL");
                        }
                    }
                    printf("Grievance Found for the employee ID\n");
                    return "Grievance Found for the employee ID";
                }else{
                    printf("No Record with the query");
                    return "No Record with the query";
                }
            }
        }else if (choice==2){
            char qry[]={"select response_number, description from grievances"};
            sprintf(stmt,qry);
            if (mysql_query(conn8,stmt)){
                printf(" Error: %s\n", mysql_error(conn8));
                printf("Failed to execute query.");
            }else{
                read = mysql_store_result(conn8);
                int count_rows = mysql_num_rows(read);
                if (count_rows>0){
                    while (row = mysql_fetch_row(read)){
                        int num_fields;
                        num_fields = mysql_num_fields(read);
                        int i;
                        for(i = 0; i < num_fields; i++){
                            printf("|");
                            if (i == 0){
                                while(field = mysql_fetch_field(read)){
                                    printf("%s|", field->name);
                                }
                                printf("\n");
                            }
                            printf(" %s", row[i] ? row[i] : "NULL");
                        }
                    }
                    printf("Grievance Found");
                    return("Grievance Found");
                }else{
                    printf("No Record with the query");
                    return "No Record with the query";
                }
            }
        }else{
            printf("Wrong choice");
            return ("Wrong choice");
        }
    }
}

/**
* \brief Manager can rate the employees using this function
*
* Employees will get the rating by their Manager for the increment/promotions
* The rating is from 1 to 5 and cannot accept others value than 1,2,3,4 and 5
* Employee ID is also validated that it is active or inactive.
* Description can be entered upto 200 character, which signifies qualities of the employee
* Year of the rating is also given and validated
* The values are inserted in emp_perfor function
*
* @param [in] int e_id Integer input value from the user
*			   int rate Integer input value as rate for the employee
*  		   char description [200] Array of character of length 200
*			   int year Integer input value as year of the rating
*
* \return User Type: Not Establish connection: For error
*		 			  Not connected : Fail to establish the connection with database
* 					  Failed to execute the query: Connection problem for execution of the query
* 			   		  Rating Added or Rating Updated: Positive case
* 					  INVALID year, Rating cannot exceed 5, Rating can not be 0: Negative Case
*
*/

char* employee_rating(int e_id,int rate,char description[200],int year){
    MYSQL_RES *read=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_RES *read1=NULL;
    MYSQL_ROW rows=NULL;

    conn8 = mysql_init(NULL);
    mysql_real_connect(conn8, "localhost", "root", "1234","payroll", port7, NULL, 0);

    if(!conn8){
        printf("Connection error\n");
        return "Not Establish connection";
    }else{
        char stmt[1500];
        char qry1[]={"select * from emp_details where emp_id=%d"};
        sprintf(stmt,qry1,e_id);

        if (mysql_query(conn8,stmt)){
            printf("Error: %s\n", mysql_error(conn8));
            return ("Failed to execute query");
        }else{
            read = mysql_store_result(conn8);
            int num_rows = mysql_num_rows(read);
            if(num_rows<=0){
                printf("Employee ID not found\n");
                return ("Employee ID not found");
            }else{
                if(rate <= 0){
                    printf("Rating cannot be 0\n");
                    return("Rating cannot be 0");
                }else if (rate >0 && rate <=5){
                    rate = rate;
                }else{
                    printf ("Rating does not exceed 5\n");
                    return ("Rating does not exceed 5");
                }

                if(!(year>=MIN_YEAR && year<=YEAR)){
                    printf ("INVALID year\n");
                    return ("INVALID year");
                }
            }
        }
        conn9 = mysql_init(NULL);
        mysql_real_connect(conn9, "localhost", "root", "1234","payroll", port7, NULL, 0);

        if(!conn9){
            printf("Connection error\n");
            return "Not Establish connection";
        }else{
            char qry4[]={"select ep.emp_id,ep.rating,ep.description,ep.year,ed.emp_id from emp_perfor ep inner join emp_details ed on (ep.emp_id=ed.emp_id)"};
            sprintf(stmt,qry4);

            if (mysql_query(conn9,stmt)){
                printf("Error: %s\n", mysql_error(conn9));
                return ("Failed to execute query");
            }else{
                read1 = mysql_store_result(conn9);
                rows = mysql_fetch_row(read);
                int num_rows = mysql_num_rows(read);
                if(num_rows<0){
                    char qry2[]={"insert into emp_perfor (emp_id,rating,description,year) VALUES('%d','%d','%s','%d')"};
                    sprintf(stmt,qry2,rate,description,e_id,year);

                    if (mysql_query(conn9,stmt)){
                        printf("Error: %s\n", mysql_error(conn9));
                        return ("Failed to execute query");
                    }else{
                        printf ("\nRating Added");
                        return ("Rating Added");
                    }
                }else{
                    printf("\t Already in database so going to update it.\n");
                    char qry2[]={"update emp_perfor set rating = '%d',description ='%s',year='%d' where emp_id= '%d'"};
                    sprintf(stmt,qry2,rate,description,year,e_id);

                    if (mysql_query(conn8,stmt)){
                        printf("Error: %s\n", mysql_error(conn8));
                        return ("Failed to execute query");
                    }else{
                        printf ("\nRating Updated");
                        return ("Rating Updated");
                    }
                }
            }
        }
    }
}

/**
* \brief Create connection and basically calls the function from here
*
* It will initialize the connection and calls the other function from this function.
*
* @param[in] int emp_id: Employee ID for the user.
*
* \return Nothing because every functions is handled internally.
*
*/

int emp_detail_mgmt(int emp_id){
    conn3=mysql_init(NULL);
    int id;
    mysql_real_connect(conn3, "localhost", "root", "1234","payroll", port7, NULL, 0);
    int i;
    printf("                Press 1 Display employee details \n");
    printf("                Press 2 Update employee details\n");
    printf("                Press 3 View employee feedback\n");
    printf("                Press 4 Change password\n");
    printf("                Press 5 Log out\n");
    scanf("%d",&i);
    switch (i){

        /**
        * Case 1: Query is passed from here to display the employee details
        * Case2: Update function is called with emp_id as @param [in]
        * Case3: Query is passed from here to view the feedback of the employee
        * Case4: Change Password function is called with emp_id as @param [in]
        */

        case 1:{
            char stmt[1500];
            char qry[] = {"select * from emp_details where emp_id = %d"};
            int n = sprintf(stmt,qry,emp_id);
            emp_display(stmt);
            break;
        }
        case 2:
        printf("%s",update_employee(emp_id));
        break;
        case 3:{
            char stmt[1500];
            char qry[] = {"select * from emp_perfor where emp_id = %d"};
            int n = sprintf(stmt,qry,emp_id);
            emp_display(stmt);
            break;
        }
        case 4:{
            change_password(emp_id);
            break;
        }
        case 5:{
            return 1;
        }
        default:{
            printf("\nInvalid Input\n");
        }
        break;
    }
    return 0;
}
