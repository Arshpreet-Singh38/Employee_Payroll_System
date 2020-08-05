/**
* \file main.c
*
* Contains the main function from which different functions
* located in other files are called to perform different
* tasks assigned for admin,employee and manager based on the
* user inputs.
*
*/


#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<mysql.h>
#include<string.h>
#include <termios.h>
#include <unistd.h>

/* Including all dependencies */
#include "..\include\employee_management.h"
#include "..\include\employee.h"
#include "..\include\manager_dept_management.h"
#include "..\include\employee_personal_dtl_management.h"
#include "..\include\employee.h"
#include "..\include\validation.h"
#include "..\include\attendance_display.h"

/* Declaration of connection to MYSQL Database pointers */
MYSQL *oo,*conn,*conn4;

/* Initializing the pointers to access data from MYSQL database*/
MYSQL_RES *read1=NULL;
MYSQL_RES *res=NULL;
MYSQL_ROW row=NULL;

/* Database connection port number*/
int port8=3306;


/**
*
* \fn char* login(int id, char pwd[25])
* \brief Checks the login credentials from database to assign the user type: Admin,Employee,Manager.
*
* Accesses the login_details table from database and matches
* login credentials provided by the user corresponding to the
* data entry in the database
*
* @param[in] id Unique login id for the user
* @param[in] pwd Password for the user
*
* \return User_Type: admin,employee or manager for successful login
*		   Wrong username/password for incorrect login credentials
*		   User deactivated if the user is deactivated in database
*
*/

char* login(int id, char pwd[25]){
    char stmt[1500];

    /* SQL query to access login_details table from database */
    char qry[]={"select * from login_details where emp_id='%d'and password='%s'"};

    oo=mysql_init(NULL);
    mysql_real_connect(oo, "localhost", "root", "1234","payroll", port8, NULL, 0);

    /* checks for the database connectivity */
    if(oo){
        int n=sprintf(stmt,qry,id,pwd);
        mysql_query(oo,stmt);
        read1=mysql_store_result(oo);
        row = mysql_fetch_row(read1);
        if(row==NULL){
            return "\nWrong username or password\n\n\n\n\n";
        }else{
            if(strcmp("I",row[3])==0){
                return "\nUser is deactivated\n\n\n\n\n\n";
            }else{
                printf("\nSuccessfully logged in \n\n\n\n\n\n\n\n\n");
                return row[2];
            }
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(oo));
    }
}




/**
* \brief The main function which distributes various tasks to other functions.
*
* Expects a user type from the login function
* Based on the functionalities provided for the user type
* and corresponding user inputs calls the other functions.
*
*/

int main(int argc, char *argv[]){
    int i,id;
    char ch;
    char pwd[25];
    char user_type[25];
    system("clear");
    printf("\n*********************************************************************\n");
    printf("                               Login                                   \n");
    printf("\n*********************************************************************\n");

    printf("                Press 1 LOGIN\n");
    printf("                Press 2 EXIT\n");
    scanf("%d",&i);
    switch(i){

        /* Expecting user id and password from the user for login*/

        case 1:{
            printf("\nEnter your employee id:");
            scanf("%d",&id);
            getchar();
            printf("\nEnter your Password:");
            int p=0;
            do{
                pwd[p]=getch();
                if((pwd[p]=='\b' ||pwd[p]==127)&& p>0){
                    p--;
                    printf("\b \b");
                    continue;
                }
                if(pwd[p]!='\r'){
                    printf("*");
                }
                if(pwd[p]=='\n'||pwd[p]==13){
                    p++;
                    break;
                }
                p++;
            }while(pwd[p-1]!='\r'||pwd[p-1]!=13);
            pwd[p-1]='\0';

            /* Calls the login function and stores the user type in user_type*/

            strcpy(user_type,login(id,pwd));
            char user_t[25];
            strcpy(user_t,user_type);
            printf("%s\n",strupr(user_t));
            break;
        }

        case 2:{
            printf("EXIT\n");
            break;
        }

        default:{
            printf("Wrong Input");
            break;
        }
    }


    /* Display the functionalities available corresponding to the user_type */


    /* If the user type is admin, the following functionality is available to the user */
    if(strcmp("admin",user_type)==0){
        int st=1;
        do{
            printf("                Press 1 Employee management\n");
            printf("                Press 2 Leave management\n");
            printf("                Press 3 Attendance management\n");
            printf("                Press 4 Salary management\n");
            printf("                Press 5 Department management\n");
            printf("                Press 6 Increment management\n");
            printf("                Press 7 Grievances redressal\n");
            printf("                Press 8 To change password\n");
            printf("                Press 9 To log out\n");
            scanf("%d",&i);
            st=emp_management(i,id);
            if(st==0){
                printf("Application connection Error");
                break;
            }
            if(st==2){
                printf("Successfully logged Out!");
                break;
            }
        }while(st==1);
    /* If the user type is employee the following functionality is available to the user */
    }else if(strcmp("employee",user_type)==0){
        int dcsn=0;
        do{
            dcsn=employee(id);
        }while(dcsn!=1);
    /* If the user type is manager,the following functionality is available to the user */
    }else if(strcmp("manager",user_type)==0){
        int dcsn=0;
        do{
            printf("\n                Press 1 Personal details management\n");
            printf("                Press 2 Attendance management\n");
            printf("                Press 3 Leave management\n");
            printf("                Press 4 Salary management\n");
            printf("                Press 5 Grievances redressal\n");
            printf("                Press 6 Department management\n");
            printf("                Press 7 Log Out!!\n");
            scanf("%d",&i);
            switch(i){
                case 1:{
                    int dcsn1=0;
                    do{
                        printf("                Press 1 Display personal details \n");
                        printf("                Press 2 Update details\n");
                        printf("                Press 3 Change password\n");
                        printf("                Press 4 Go back....\n");
                        int j=1;
                        scanf("%d",&j);
                        switch (j){
                            case 1:{
                                char stmt[1500];
                                char qry[] = {"select * from emp_details where emp_id = %d"};
                                int n = sprintf(stmt,qry,id);
                                emp_display(stmt);
                                break;
                            }
                            case 2:{
                                printf("%s",update_employee(id));
                                break;
                            }
                            case 3:{
                                change_password(id);
                                break;
                            }
                            case 4:{
                                dcsn1=1;
                            }
                            default:
                            break;
                        }
                    }while(dcsn1!=1);
                    break;
                }
                case 2:{
                    int choice;
                    int dcsn1=0;
                    do{
                        printf("                Press 1 Display attendance\n");
                        printf("                Press 2 Request attendance change\n");
                        printf("                Press 3 To Go back.....\n");
                        scanf("%d",&choice);
                        if(choice==1){
                            view_attendance(id);
                        }else if(choice==2){
                            reuest_status(id);
                        }else if(choice==3){
                            dcsn1=1;
                        }else{
                            printf("\nWrong Input");
                        }
                    }while(dcsn1!=1);
                    break;
                }
                case 3:{
                    int dcsn1=0;
                    do{
                        int i;
                        printf("                Press 1 Request leave\n");
                        printf("                Press 2 Display leaves\n");
                        printf("                Press 3 Go back..\n");
                        scanf("%d",&i);
                        switch(i){
                            case 1:{
                                int x=0;
                                int dd,mm,yy,r;
                                int no_of_days;
                                char leave_type[20];
                                do{
                                    printf("\nEnter the start date for the leave:Format(dd/mm/yyyy)");
                                    scanf("%d/%d/%d",&dd,&mm,&yy);
                                    r=validate_date(dd,mm,yy);
                                }while(r!=1);
                                printf("Provide the number of days for the leave,(including start date): ");
                                scanf("%d",&no_of_days);
                                do{
                                    printf("Please provide the leave type to avail: ? (SL),(PL),(LWP)");
                                    scanf("%s",leave_type);
                                    if (strcasecmp(leave_type,"SL")==0 || strcasecmp(leave_type,"PL")==0 || strcasecmp(leave_type,"LWP")==0){
                                        x=1;
                                        break;
                                    }
                                } while(x==0);
                                x=0;
                                printf("%s",leave_request(id,dd,mm,yy,no_of_days,leave_type));
                                break;
                            }
                            case 2:{
                                int dcsn1=1,year;
                                do{
                                    printf("Enter the leave year you wish to see: \n");
                                    scanf("%d",&year);
                                    dcsn1=display_leaves(id,year);
                                }while(dcsn1==2);
                                break;
                            }
                            case 3:{
                                dcsn1=1;
                                break;
                            }
                        }

                    }while(dcsn1!=1);
                    break;
                }
                case 4:{
                    int dcsn1=0;
                    do{
                        int option;
                        printf("\n                Press 1 if you want to see all pay slips\n");
                        printf("                Press 2 if you want to see pay slips for particular pay period\n");
                        printf("                Press 3 To display hourly/monthly salary\n");
                        printf("                Press 4 Go back...\n");
                        scanf("%d",&option);
                        dcsn1=display_salary(id,option);

                    }while(dcsn1!=1);
                    break;
                }
                case 5:{
                    int dcsn1=0;
                    do{
                        int choice;
                        printf("\n		Press 1 Raise Grievance\n");
                        printf("		Press 2 View Grievances\n");
                        printf("		Press 3 To Go back....\n");
                        scanf("%d",&choice);
                        if (choice==1){
                            printf("%s",raise_grievances(id));
                        }else if(choice==2){
                            int g_choice;
                            printf("		Press 1 View Grievances by employee id\n");
                            printf("		Press 2 View all Grievances\n");
                            scanf("%d",&g_choice);
                            int gri_choice;
                            if (choice == 1){
                                printf("Enter the employee id to see their grievances: \n");
                                scanf("%d",&gri_choice);
                            }
                            view_raised_grievances(g_choice,gri_choice);
                        }else if(choice==3){
                            dcsn1=1;
                        }else{
                            printf("\t\t Wrong Choice Entered.\n");
                        }
                    }while(dcsn1!=1);
                    break;
                }
                case 6:{
                    int dcsn1=0;
                    do{
                        int j=0;
                        printf("\n                Press 1 Request Employee\n");
                        printf("                Press 2 Rate employee\n");
                        printf("                Press 3 Display employee rating \n");
                        printf("                Press 4 To Go back... \n");
                        scanf("%d",&j);
                        switch(j){
                            case 1:{
                                char stmt[1500];
                                char dept[10];
                                char designation[15];
                                conn4=mysql_init(NULL);
                                mysql_real_connect(conn4, "localhost", "root", "1234","payroll", port8, NULL, 0);
                                char qry_dep[]={"select department from emp_details where emp_id='%d'"};
                                if(conn4){
                                    int n=sprintf(stmt,qry_dep,id);
                                    mysql_query(conn4,stmt);
                                    read1=mysql_store_result(conn4);
                                    row=mysql_fetch_row(read1);
                                    strcpy(dept,row[0]);
                                    printf("At which position/designation is the employee required: \n");
                                    scanf("%s",designation);
                                    employee_request(id,dept,designation);
                                }else{
                                    printf("not connected");
                                    printf("%s\n", mysql_error(oo));
                                }
                                break;
                            }
                            case 2:{
                                char description[200];
                                int x=0,year=0,rate=0,e_id=0;
                                printf("Enter the Employee ID for rating: ");
                                scanf("%d",&e_id);

                                printf("Enter the rating of the employee for %d : ",e_id);
                                scanf("%d",&rate);

                                getchar();
                                do{
                                    printf("Feedback of the employee under 200 characters\n");
                                    gets(description);
                                    x=notempty(description);
                                }while(x==0);

                                printf("Enter the year for rating\n");
                                scanf("%d",&year);

                                printf("%s",employee_rating(e_id,rate,description,year));
                                break;
                            }
                            case 3:{
                                char stmt[1500];
                                char qry[] = {"select * from emp_perfor where emp_id = %d"};
                                int n = sprintf(stmt,qry,id);
                                emp_display(stmt);
                                break;
                            }
                            case 4:{
                                dcsn1=1;
                            }
                            default:{
                                printf("\nWrong Input\n");
                                break;
                            }
                        }
                    }while(dcsn1!=1);
                    break;
                }
                case 7:{
                    printf("Thank You! Have a nice Day!");
                    dcsn=1;
                    break;
                }
            }
        }while(dcsn!=1);
    }else{
        printf("Have a Good Day!");
    }
    return 0;
}
