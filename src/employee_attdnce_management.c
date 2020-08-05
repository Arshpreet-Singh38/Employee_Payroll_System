/**
* @file employee_attdnce_management.c
*
* Various attendance management functions for the user present in this file
*
*/

#include<stdio.h>
#include<windows.h>
#include<mysql.h>
#include "..\include\validation.h"
#include "..\include\attendance_display.h"
MYSQL *conn3;

int port5=3306;
/**
* \brief attendance change function takes 5 arguments.
*
* User can raise attendance request if he finds any problem
*
*
* @param[in] emp_id employee id who logged in
* @param[in] dd day of date
* @param[in] mm month of date
* @param[in] yy year of date
* @param[in] description to write tha problem
*
* \return invalid date, user not found, database error, request submitted
*
*/
char* attendance_change(int emp_id, int dd,int mm,int yy, char description[200]){
    int r;
    char date[15];
    r=validate_current_month(dd,mm,yy);
    if(r!=1){
        printf("\nInvalid Date\n");
        return "Invalid Date";
    }else if(strlen(description)>=150){
        printf("\n Description must not be greater than 150 characters\n");
        return "Invalid description";
    }else if(emp_id==0){
        printf("\nEmployee not exists\n");
        return "User not found";
    }else{
        conn3=mysql_init(NULL);
        mysql_real_connect(conn3, "localhost", "root", "1234","payroll", port5, NULL, 0);
        sprintf(date,"%d/%d/%d", dd,mm,yy);
        char stmt[1500];
        char qry[]={"insert into request_form (emp_id,date,descript,req_status) VALUES('%d','%s','%s','0')"};
        sprintf(stmt,qry,emp_id,date,description);
        if (mysql_query(conn3,stmt)){
            printf(" Error: %s\n", mysql_error(conn3));
            return "Database Error\n";
        }else{
            return "Request submitted";
        }
    }
}

/**
* \brief reuest_status takes 1 argument.
*
* User can check status of its request to change attendance
*
*
* @param[in] emp_id employee id who logged in
*
* \prints request status
*
*/
void reuest_status(int emp_id){

    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    int num,u;
    char stmt[1500];
    char qry_id[]={"select * from request_form where emp_id='%d'"};
    sprintf(stmt,qry_id,emp_id);
    if (mysql_query(conn3,stmt)){
        printf(" Error: %s\n", mysql_error(conn3));
        printf("Failed to execute query.");
    }else{
        int i=0;
        read = mysql_store_result(conn3);

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
    }
}


/**
* \brief attendance management takes 1 argument.
*
* User can see various attendance management options
*
*
* @param[in] emp_id employee id who logged in
*
* \ redirects user to the option selected by user
*
*/

int emp_attendance_mgmt(int emp_id){
    conn3=mysql_init(NULL);
    mysql_real_connect(conn3, "localhost", "root", "1234","payroll", port5, NULL, 0);
    if(!conn3){
        printf("Connection error");

    }else{
        printf("                Press 1 Display attendance \n");
        printf("                Press 2 Display attendance change request\n");
        printf("                Press 3 Request attendance change\n");
        printf("                Press 4 to Go Back...\n");
        int ch;
        scanf("%d",&ch);
        if(ch==1){
            int i=chk_emp_type(emp_id);
		    if(i==1){
                view_attendance(emp_id);
            }else if(i==2){
                hourly_view_attendance(emp_id);
            }
        }else if(ch==2){
            reuest_status(emp_id);
        }else if(ch==3){
            char decs11[50];
            int r,dd,mm,yy;
            char description[200];
            int x=1;
            printf("Enter the date on which you have problem (mm/dd/yyyy)\n");
            scanf("%d/%d/%d",&dd,&mm,&yy);
            do{
                printf("Explain your problem in 150 characters\n");
                gets(description);
                x=notempty(description);
            }while(x==0);
            do{
                if((strcmp(decs11,"Invalid Date")==0)){
                    printf("Enter the date on which you have problem (mm/dd/yyyy)\n");
                    scanf("%d/%d/%d",&dd,&mm,&yy);

                }
                if(strcmp(decs11,"Invalid description")==0){
                    do{
                        printf("\nExplain your problem in 150 characters\n");
                        gets(description);
                        x=notempty(description);
                    }while(x==0);
                }

                strcpy(decs11,attendance_change(emp_id,dd,mm,yy,description));
            }while(strcmp(decs11,"Request submitted")!=0);
            if(strcmp(decs11,"Database Error")==0){
                printf("Database Error");
            }else if(strcmp(decs11,"Request submitted")==0){
                printf("Request Submitted");
            }
        }else if(ch==4){
            return 1;
        }else{
            printf("wrong choice");
        }
    }
    return 0;
}

