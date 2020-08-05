/**
* @file employee.c
*
* Perform the functions who is login with employee crendentials
*
*/

/* Including basic libraries */
#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<mysql.h>
#include<string.h>
#include <termios.h>
#include <unistd.h>

/* Including all dependencies */
#define MAX_YEAR 2020
#define MIN_YEAR 2010
#define MAX_MONTH 12
#define MIN_MONTH 1
#include "..\include\employee_personal_dtl_management.h"
#include "..\include\employee.h"
#include "..\include\admin_attendance.h"
#include "..\include\employee_management.h"
#include "..\include\validation.h"

/* Declaration of connection to MYSQL Database pointers and database port number */
MYSQL *conn2, *oo,*conn,*conn4;
int port4=3306;

/**
* \brief Get or scan the value from the console screen
*
* Function can be called for any type of user-> Admin, Manager, Employee who want to scan the values from console screeen
*
* @param [void] No Arguments will be passed
*
* \return User_Type: Integer value 'ch' for all case
*
*/

int getch(void){
    struct termios oldt,newt;
    int   ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

/**
* \brief Display salary with the Employee ID
*
* It will display the salary for the employee ID
*
* @param[in] int emp_id Integer value is passed of the user.
*
* \return User_Type: 0: Negative or Wrong value
*                    1: Positiveor Successful Value
*
*/

int display_salary(int emp_id,int option){
    conn2=mysql_init(NULL);
    int id;
    mysql_real_connect(conn2, "localhost", "root", "1234","payroll", port4, NULL, 0);
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    MYSQL_FIELD *field;
    int num;
    char stmt[1500];
    char qry[300];
    if(option==1){
        /* Accessing the salary_cal table to select emp_id */
        strcpy(qry,"select * from salary_cal where emp_id='%d'  order by year desc,month desc");
    }else if(option==2){
        /* Accessing the salary_cal table to select emp_id, year and month */
        strcpy(qry,"select * from salary_cal where emp_id='%d' and year='%d'and month='%d'");
    }else if(option==3){
        strcpy(qry,"select * from salary where emp_id='%d' and sal_year='%d'");
    }else if(option==4){
        return 1;
    }else{
        printf("Wrong Input");
        return 3;
    }

    if(conn2){
        int n;
        if(option==1){
            n = sprintf(stmt,qry,emp_id);
        }else if(option==2){
            int year,month;
            int x=0;
            do{
                printf("Enter the  year you wish to see: \n");
                scanf("%d",&year);
                if(year<=MAX_YEAR && year>=MIN_YEAR){
                    x=1;
                }else{
                    printf("INVALID year, enter again\n");
                }
            }while(x==0);
            x=0;
            do{
                printf("Enter the month you wish to see: \n");
                scanf("%d",&month);
                if(month<=MAX_MONTH && month>=MIN_MONTH){
                    x=1;
                }else{
                    printf("INVALID month, enter again\n");
                }
            }while(x==0);
            x=0;
            n = sprintf(stmt,qry,emp_id,year,month);
        }else if(option==3){
            int year;
            int x=0;
            do{
                printf("Enter the  year you wish to see: \n");
                scanf("%d",&year);
                if(year<=MAX_YEAR && year>=MIN_YEAR){
                    x=1;
                }else{
                    printf("INVALID year, enter again\n");
                }
            }while(x==0);

            sprintf(stmt,qry,emp_id,year);
        }

        mysql_query(conn2,stmt);
        read = mysql_store_result(conn2);
        if (mysql_query(conn2,stmt)){
            printf(" Error: %s\n", mysql_error(conn2));
            printf("Failed to execute query.");
            return 7;
        }else{
            row = mysql_fetch_row(read);
            if(row==NULL){
                printf("No details found");
                return 6;
            }
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
                    printf("  %s  ", row[i] ? row[i] : "NULL");
                }
                row = mysql_fetch_row(read);
            }
        }
    }
    return 0;
}

/**
* \brief Change password of the user
*
* Change the password when user called this function
*
* @param[in] int emp_id Integer value for Employee ID
* 			  char new_pass[45] Array of characters of length 45
* 			  char confirm_pass[45] Array of characters of length 45
* 			  char old_passs[45] Array of characters of length 45
*
* \return string message change password status
*
*/

char* change_pass(int emp_id,char new_pass[45], char confirm_pass[45],char old_pass[45]){
    MYSQL_RES *read1=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    oo=mysql_init(NULL);
    mysql_real_connect(oo, "localhost", "root", "1234","payroll", port4, NULL, 0);
    char stmt[1500];
    char qry[]={"select password from login_details where emp_id='%d'"};
    if(oo){
        sprintf(stmt,qry,emp_id);
        if (mysql_query(oo,stmt)){
            printf(" Error: %s\n", mysql_error(oo));
            return "Failed to execute query.";
        }else{
            read1=mysql_store_result(oo);
            row = mysql_fetch_row(read1);
            if(row==NULL){
                return "\nWrong username or password\n\n\n\n\n";
            }else{
                if(strcmp(old_pass,row[0])==0){
                    char qry[]={"update login_details set password='%s' where emp_id='%d'"};
                    printf( "\nOld Password confirmed\n\n\n");
                    if(strcmp(new_pass,confirm_pass)==0){
                        sprintf(stmt,qry,new_pass,emp_id);
                        if (mysql_query(oo,stmt)){
                            printf(" Error: %s\n", mysql_error(oo));
                            return "Failed to execute query.\n";
                        }else{
                            return "Password updated\n";
                        }
                    }else{
                        return "re-enter new password \n";
                    }
                }else{
                    return "\nOld password wrong\n\n\n";
                }
            }
        }
    }else{
        printf("not connected");
        printf("%s\n", mysql_error(oo));
        return "Error\n";
    }
}

/**
* \brief Change the password of the user
*
* Change the password of the user
*
* @param[in] int id Integer value given by the user
*
* \return Nothing because the data retrieved from the database is printing on the console screen.
*
*/

void change_password(int id){
    char old_pass[40],new_pass[40],confirm_pass[40];
    getch();
    printf("\nEnter old password\n");
    int p=0;
    do{
        old_pass[p]=getch();
        if((old_pass[p]=='\b' ||old_pass[p]==127)&& p>0){
            p--;
            printf("\b \b");
            continue;
        }
        if(old_pass[p]!='\r'){
            printf("*");
        }
        if(old_pass[p]=='\n'||old_pass[p]==13){
            p++;
            break;
        }
        p++;
    }while(old_pass[p-1]!='\r'||old_pass[p-1]!=13);
    old_pass[p-1]='\0';
    printf("\nEnter new password\n");
    p=0;
    do{
        new_pass[p]=getch();
        if((new_pass[p]=='\b'||new_pass[p]==127)&& p>0){
            p--;
            printf("\b \b");
            continue;
        }
        if(new_pass[p]!='\r'){
            printf("*");
        }
        if(new_pass[p]=='\n'||new_pass[p]==13){
            p++;
            break;
        }
        p++;
    }while(new_pass[p-1]!='\r'||new_pass[p-1]!=13);
    new_pass[p-1]='\0';
    printf("\nConfirm new password\n");
    p=0;
    do{
        confirm_pass[p]=getch();
        if((confirm_pass[p]=='\b'||confirm_pass[p]==127)&& p>0){
            p--;
            printf("\b \b");
            continue;
        }
        if(confirm_pass[p]!='\r'){
            printf("*");
        }
        if(confirm_pass[p]=='\n'||confirm_pass[p]==13){
            p++;
            break;
        }
        p++;
    }while(confirm_pass[p-1]!='\r'||confirm_pass[p-1]!=13);
    confirm_pass[p-1]='\0';
    printf("%s\n",change_pass(id,new_pass,confirm_pass,old_pass));
}

/**
* \brief Basically call the function from this main function
*
* It is the main function where other functions are called to perform the actions accordingly
*
* @param[in] int emp_id: Employee ID for the user
*
* \return Nothing because every functions is handled internally
*
*/

int employee(int emp_id){
    int i;
    printf("\n                Press 1 Personal details management\n");
    printf("                Press 2 Attendance management\n");
    printf("                Press 3 Leave management\n");
    printf("                Press 4 Salary management\n");
    printf("                Press 5 Grievances redressal\n");
    printf("                Press 6 to Log Out!!\n");
    scanf("%d",&i);
    switch(i){
        case 1:{
            int dcsn=0;
            do{
                dcsn=emp_detail_mgmt(emp_id);
            }while(dcsn!=1);
            break;
        }
        case 2:{
            int dcsn=0;
            do{
                dcsn=emp_attendance_mgmt(emp_id);
            }while(dcsn!=1);
            break;
        }
        case 3:{
            int dcsn=0;
            do{
                int i;
                printf("\n                Press 1 Request leave\n");
                printf("                Press 2 Display leaves\n");
                printf("                Press 3 To Go Back...\n");
                scanf("%d",&i);
                switch(i){
                    case 1:{
                        int x=0;
                        int dd,mm,yy,r;
                        int no_of_days;
                        char leave_type[20];
                        char validation_res[45];
                        do{
                            printf("\nEnter the start date for the leave:Format(dd/mm/yyyy)");
                            scanf("%d/%d/%d",&dd,&mm,&yy);

                            printf("Provide the number of days for the leave,(including start date): ");
                            scanf("%d",&no_of_days);

                            printf("Please provide the leave type to avail: ? (ML),(PL),(LWP)");
                            scanf("%s",leave_type);
                            strcpy(validation_res,leave_request(emp_id,dd,mm,yy,no_of_days,leave_type));
                        }while(strcmp(validation_res,"Invalid Date")==0 || strcmp(validation_res,"Invalid leave type")==0);

                        if(strcmp(validation_res,"Request raised successfully")==0){
                            dcsn=1;
                        }else if(strcmp(validation_res,"Database connection error")==0){
                            dcsn=1;
                        }
                        break;
                    }
                    case 2:{
                        int year1,dcsn1=1;
                        do{
                            printf("Enter the leave year you wish to see: \n");
                            scanf("%d",&year1);
                            dcsn1=display_leaves(emp_id,year1);
                        }while(dcsn1==2);
                        break;
                    }
                    case 3:{
                        dcsn=1;
                        break;
                    }
                }
            }while(dcsn!=1);
            break;
        }
        case 4:{
            int dcsn=0;
            do{
                int option;
                printf("\n                Press 1 if you want to see all pay slips\n");
                printf("                Press 2 if you want to see pay slips for particular pay period\n");
                printf("                Press 3 To display hourly/monthly salary\n");
                printf("                Press 4 Go back...\n");
                scanf("%d",&option);
                dcsn=display_salary(emp_id,option);
            }while(dcsn!=1);
            break;
        }
        case 5:{
            int dcsn=0;
            do{
                int choice;
                printf("		Press 1 Raise Grievance\n");
                printf("		Press 2 View Grievances\n");
                printf("		Press 3 Go back....\n");
                scanf("%d",&choice);
                if (choice==1){
                    printf("%s",raise_grievances(emp_id));
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
                }
                else if(choice==3){
                    dcsn=1;
                }
                else{
                    printf("\t\t Wrong Choice Entered.\n");
                }
                break;
            }while(dcsn!=1);
            break;
        }
        case 6:{
            printf("\n Thank you! Have Good Day....");
            return 1;
        }
        default:{
            printf("wrong input");
            break;
        }
    }
    return 0;
}
