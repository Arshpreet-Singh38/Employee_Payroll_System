/**
* @file attendance_display.c
*
* Perform the Admin duties for viewing the attendance from this file
*
*/

/* Including basic libraries */
#include<stdio.h>
#include<windows.h>
#include<mysql.h>

/* Include preprocessor for declaration of the functions. */
#include "..\include\validation.h"

/* Declaration of connection to MYSQL Database pointers and database port number */
MYSQL *conn10;
int port3=3306;

/**
 * \brief View the attendance for hourly employees
 *
 * Accessing the database to view the attendance from daily_attendance table for hourly employees
 * Admin can access for all users but employee and manager can access for own only.
 *
 * @param[in] int emp_id Integer value of the user Employee ID
 *
 * \return Nothing as the function is printing messages on the console screen
 *
 */

 void hourly_view_attendance(int emp_id)
 {
    int dd,mm,yy,r,num;
	MYSQL_RES *read=NULL;
	MYSQL_RES *res=NULL;
	MYSQL_ROW row=NULL;
	char stmt[2500];
	char qry_id[2000];
	conn10=mysql_init(NULL);
	mysql_real_connect(conn10, "localhost", "root", "1234","payroll", port3, NULL, 0);
	int days;
    do
	{
		printf("\nEnter the attendance month and year:Format(mm/yyyy)\n");
		scanf("%d/%d",&mm,&yy);
		r=validate_date(01,mm,yy);
	}while(r!=1);

    strcpy(qry_id,"select emp_details.name1,emp_details.name2, hourly_attendance.att_day,hourly_attendance.In_time,hourly_attendance.Out_time,hourly_attendance.days from hourly_attendance join emp_details on emp_details.emp_id=hourly_attendance.emp_id where emp_details.emp_id='%d' and hourly_attendance.att_month='%d'and att_year='%d'");

	//char qry_id[]={"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A%d from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'"};
    sprintf(stmt,qry_id,emp_id,mm,yy);

	if (mysql_query(conn10,stmt))
    {
        printf(" Error: %s\n", mysql_error(conn10));
        printf("Failed to execute query.");
    }
    else
    {
    	int i=0;
    	read = mysql_store_result(conn10);
        int count=mysql_num_rows(read);
        if(count>=1)
        {
            //printf("\nName |LName|Designation\n");
            while(row = mysql_fetch_row(read))
            {
                printf("|");
                printf("Name=%",row[0]);
                printf("    Last Name=%s",row[1]);
                printf("    Day=%s",row[2]);
                printf("    In_time=%s",row[3]);
                printf("    Out_time=%s",row[4]);
                printf("    Total Hours=%s",row[4]);
                printf("\n");
            }
        }
        else
        {
            printf("No record in the database");
        }

	}
 }


/**
 * \brief View the attendance for salaried employees
 *
 * Accessing the database to view the attendance from daily_attendance table for salaried employees
 * Admin can access for all users but employee and manager can access for own only.
 *
 * @param[in] int emp_id Integer value of the user Employee ID
 *
 * \return Nothing as the function is printing messages on the console screen
 *
 */
 
void view_attendance(int emp_id){
    int dd,mm,yy,r,num;
    MYSQL_RES *read=NULL;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;
    char stmt[2500];
    char qry_id[2000];
    conn10=mysql_init(NULL);
    mysql_real_connect(conn10, "localhost", "root", "1234","payroll", port3, NULL, 0);
    int days;
    do{
        printf("\nEnter the attendance month and year:Format(mm/yyyy)\n");
        scanf("%d/%d",&mm,&yy);
        r=validate_date(01,mm,yy);
    }while(r!=1);
    if(mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12){
        strcpy(qry_id,"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A1,daily_attendance.A2,daily_attendance.A3,daily_attendance.A4,daily_attendance.A5,daily_attendance.A6,daily_attendance.A7,daily_attendance.A8,daily_attendance.A9,daily_attendance.A10,daily_attendance.A11,daily_attendance.A12,daily_attendance.A13,daily_attendance.A14,daily_attendance.A15,daily_attendance.A16,daily_attendance.A17,daily_attendance.A18,daily_attendance.A19,daily_attendance.A20,daily_attendance.A21,daily_attendance.A22,daily_attendance.A23,daily_attendance.A24,daily_attendance.A25,daily_attendance.A26,daily_attendance.A27,daily_attendance.A28,daily_attendance.A29,daily_attendance.A30,daily_attendance.A31 from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'");
    }else if(mm==4 || mm==6 || mm==9 || mm==11){
        days=30;
        strcpy(qry_id,"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A1,daily_attendance.A2,daily_attendance.A3,daily_attendance.A4,daily_attendance.A5,daily_attendance.A6,daily_attendance.A7,daily_attendance.A8,daily_attendance.A9,daily_attendance.A10,daily_attendance.A11,daily_attendance.A12,daily_attendance.A13,daily_attendance.A14,daily_attendance.A15,daily_attendance.A16,daily_attendance.A17,daily_attendance.A18,daily_attendance.A19,daily_attendance.A20,daily_attendance.A21,daily_attendance.A22,daily_attendance.A23,daily_attendance.A24,daily_attendance.A25,daily_attendance.A26,daily_attendance.A27,daily_attendance.A28,daily_attendance.A29,daily_attendance.A30 from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'");
    }else if(mm==2){
        if(yy%400==0 ||(yy%4==0 && yy%100!=0)){
            strcpy(qry_id,"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A1,daily_attendance.A2,daily_attendance.A3,daily_attendance.A4,daily_attendance.A5,daily_attendance.A6,daily_attendance.A7,daily_attendance.A8,daily_attendance.A9,daily_attendance.A10,daily_attendance.A11,daily_attendance.A12,daily_attendance.A13,daily_attendance.A14,daily_attendance.A15,daily_attendance.A16,daily_attendance.A17,daily_attendance.A18,daily_attendance.A19,daily_attendance.A20,daily_attendance.A21,daily_attendance.A22,daily_attendance.A23,daily_attendance.A24,daily_attendance.A25,daily_attendance.A26,daily_attendance.A27,daily_attendance.A28,daily_attendance.A29 from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'");
            days=29;
        }else{
            strcpy(qry_id,"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A1,daily_attendance.A2,daily_attendance.A3,daily_attendance.A4,daily_attendance.A5,daily_attendance.A6,daily_attendance.A7,daily_attendance.A8,daily_attendance.A9,daily_attendance.A10,daily_attendance.A11,daily_attendance.A12,daily_attendance.A13,daily_attendance.A14,daily_attendance.A15,daily_attendance.A16,daily_attendance.A17,daily_attendance.A18,daily_attendance.A19,daily_attendance.A20,daily_attendance.A21,daily_attendance.A22,daily_attendance.A23,daily_attendance.A24,daily_attendance.A25,daily_attendance.A26,daily_attendance.A27,daily_attendance.A28 from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'");
            days=28;
        }
    }

    //char qry_id[]={"select emp_details.name1,emp_details.name2, daily_attendance.designation,daily_attendance.A%d from daily_attendance join emp_details on emp_details.emp_id=daily_attendance.emp_id where emp_details.emp_id='%d' and daily_attendance.attend_month='%d'and attend_year='%d'"};
    sprintf(stmt,qry_id,emp_id,mm,yy);

    if (mysql_query(conn10,stmt)){
        printf(" Error: %s\n", mysql_error(conn10));
        printf("Failed to execute query.");
    }else{
        int i=0;
        read = mysql_store_result(conn10);

        //printf("\nName |LName|Designation\n");
        row = mysql_fetch_row(read);
        num = mysql_num_fields(read);
        printf("|");
        printf("Name=%s\n",row[0]);
        printf("Last Name=%s\n",row[1]);
        printf("Designation=%s\n",row[2]);
        for(i = 3; i < num; i++){
            printf("Date %d=%s\n",(i-2),row[i]);
        }
        printf("\n--------------------------------------------------------------------------------\n");
    }
}

