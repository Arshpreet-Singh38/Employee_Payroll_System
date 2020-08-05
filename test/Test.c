/**
* @file test.c
*
* @brief Implementation of unit test cases.
*
* @author Arshpreet Singh -
* @author Jagdeep Singh -
* @author Puneet Singh -
* @author Varinder Singh Dhanda -
*/
#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<mysql.h>
#include<string.h>
#include "..\include\employee_management.h"
#include "..\include\employee.h"
#include "..\include\manager_dept_management.h"
#include "..\include\employee_personal_dtl_management.h"
#include "..\include\salary_management.h"

MYSQL *conn;
MYSQL_RES *read1=NULL;
MYSQL_RES *res=NULL;
MYSQL_ROW row=NULL;
#define ASSERT_TEST(x)                        \
{                                         \
    if (x)                                \
    {                                     \
        fprintf(fp,"\n\nPASSED\n\n\n");               \
    }                                     \
    else                                  \
    {                                     \
        fprintf(fp,"\n\nFAILED\n\n\n"); \
    }                                     \
}


char* login(int id, char pwd[25])
{
    char stmt[1500];
    char qry[]={"select * from login_details where emp_id='%d'and password='%s'"};
    conn=mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "root", "1234","payroll", 3306, NULL, 0);
    if(conn)
    {
        
        int n=sprintf(stmt,qry,id,pwd);
        mysql_query(conn,stmt);
        read1=mysql_store_result(conn);
        row = mysql_fetch_row(read1);
        if(row==NULL)
        {
            return "\nWrong username or password\n\n\n\n\n";
            
        }
        else
        {
            if(strcmp("I",row[3])==0)
            {
                
                return "\nUser is deactivated\n\n\n\n\n\n";
            }
            else
            {
                printf("\nSuccessfully logged in \n");
                return row[2];
                
            }
        }
    }
    else
    {
        printf("not connected");
        printf("%s\n", mysql_error(conn));
    }
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("./test/testresults.txt","w");
    if(fp==NULL)
    {
        printf("Unable to open file\n");
    }
    else
    {
        fprintf(fp,"*********************************TEST 1 for Login**********************************************\n");
        fprintf(fp,"***********************************TEST 1A*****************************************************\n");
        fprintf(fp,"We are first going to login using correct credentials for Admin i.e. Userid=1 and password=1234\n");
        fprintf(fp,"Expected result is it should return string admin\n ");
        ASSERT_TEST(strcmp(login(1,"1234"),"admin")==0);
        fprintf(fp,"***********************************TEST 1B****************************************************\n");
        fprintf(fp,"We are going to login using incorrect password Userid=1 and password=1233\n");
        fprintf(fp,"Expected result is it should return string Wrong username or password\n ");
        ASSERT_TEST(strcmp(login(1,"1233"),"\nWrong username or password\n\n\n\n\n")==0);
        fprintf(fp,"***********************************TEST 1C****************************************************\n");
        fprintf(fp,"We are going to login using incorrect username Userid=999 and password=1233\n");
        fprintf(fp,"Expected result is it should return string Wrong username or password\n ");
        ASSERT_TEST(strcmp(login(999,"1233"),"\nWrong username or password\n\n\n\n\n")==0);
        fprintf(fp,"***********************************TEST 1D****************************************************\n");
        fprintf(fp,"We are going to login using Inactive user credentials i.e UserId=3 and Password=1234\n");
        fprintf(fp,"Expected result is it should return string User is deactivated\n ");
        ASSERT_TEST(strcmp(login(3,"1234"),"\nUser is deactivated\n\n\n\n\n\n")==0);
        
        fprintf(fp,"*******************************TEST 2 for change_password*****************************************\n");
        fprintf(fp,"***********************************TEST 2A*****************************************************\n");
        fprintf(fp,"Change_password accepts 4 parameters i.e. userid,old password , new password , confirm new password\n");
        fprintf(fp,"In first case we are going to input correct credentials, it should return string Password updated\n");
        ASSERT_TEST(strcmp(change_pass(1,"1233","1233","1234"),"Password updated\n")==0);
        change_pass(1,"1234","1234","1233");
        fprintf(fp,"***********************************TEST 2B*****************************************************\n");
        fprintf(fp,"In second case we are going to input incorrect old password, it should return string Old password wrong\n");
        ASSERT_TEST(strcmp(change_pass(1,"1222","1222","12"),"\nOld password wrong\n\n\n")==0);
        fprintf(fp,"***********************************TEST 2C*****************************************************\n");
        fprintf(fp,"In second case we are going to input incorrect new password, it should return string re-enter new password\n");
        ASSERT_TEST(strcmp(change_pass(1,"1232","1224","1234"),"re-enter new password \n")==0);
        
        fprintf(fp,"\n\n\n\n*********************************TEST 3 for attendance change request**********************************************\n");
        fprintf(fp,"***********************************TEST 3A*****************************************************\n");
        fprintf(fp,"We are first going to raise a request with wrong date 123/123/1243 and a random description\n");
        fprintf(fp,"Expected result is it should return Invalid date format\n ");
        ASSERT_TEST(strcmp("Invalid Date",attendance_change(2,123,123,1243,"abcd"))==0);
        fprintf(fp,"\n\n***********************************TEST 3B*****************************************************\n");
        fprintf(fp,"Function call with a description with more than 150 character\n");
        fprintf(fp,"Expected result is it should return Description must not be greater than 150 characters 'C is a procedural programming language. It was initially developed by Dennis Ritchie in the year 1972. It was mainly developed as a system programming language to write an operating system' \n ");
        ASSERT_TEST(strcmp("Invalid description",attendance_change(2,01,04,2020,"C is a procedural programming language. It was initially developed by Dennis Ritchie in the year 1972. It was mainly developed as a system programming language to write an operating system"))==0);
        fprintf(fp,"\n\n***********************************TEST 3C*****************************************************\n");
        fprintf(fp,"Function call without employee id \n");
        fprintf(fp,"Expected result is it should return Employee not exists\n ");
        ASSERT_TEST(strcmp("User not found",attendance_change(0,01,04,2020,"abcd"))==0);
        fprintf(fp,"\n\n***********************************TEST 3D*****************************************************\n");
        fprintf(fp,"Function call with a correct parameters empID=2, date= 17/04/2020 and description='change Absent with ML'\n");
        fprintf(fp,"Expected result is it should return Request submitted\n ");
        ASSERT_TEST(strcmp("Request submitted",attendance_change(2,17,04,2020,"change Absent with ML"))==0);
        
        fprintf(fp,"\n\n\n\n*********************************TEST 4 for employee deactivation**********************************************\n");
        fprintf(fp,"***********************************TEST 4A*****************************************************\n");
        fprintf(fp,"We are first going to raise a request with a wrong emp_id to deactivate\n");
        fprintf(fp,"Expected result is it should return Employee does not exists\n ");
        ASSERT_TEST(strcmp("No employee exists",deactivate(92344,1))==0);
        fprintf(fp,"***********************************TEST 4B*****************************************************\n");
        fprintf(fp,"Now we raise a request to deactivate the same logged in user id\n");
        fprintf(fp,"Expected result is it should return: Same logged in user can not deactivate himself\n ");
        ASSERT_TEST(strcmp("Same logged in user",deactivate(1,1))==0);
        fprintf(fp,"***********************************TEST 4C*****************************************************\n");
        fprintf(fp,"Now we raise a request to deactivate the already deactivated account\n");
        fprintf(fp,"Expected result is it should return: User already deactivated\n ");
        ASSERT_TEST(strcmp("Already de-activated",deactivate(3,1))==0);
        fprintf(fp,"***********************************TEST 4D*****************************************************\n");
        fprintf(fp,"Now we raise a request to deactivate right employee\n");
        fprintf(fp,"Expected result is it should return: Employee deactivated\n ");
        ASSERT_TEST(strcmp("Employee deactivated",deactivate(2,1))==0);
        
        fprintf(fp,"\n\n\n\n*********************************TEST 5 for employee display leaves**********************************************\n");
        fprintf(fp,"***********************************TEST 5A*****************************************************\n");
        fprintf(fp,"We are first going to raise a display leave request with a wrong year\n");
        fprintf(fp,"Expected result is it should return Wrong year chosen\n ");
        ASSERT_TEST(display_leaves(2,22)==2);
        fprintf(fp,"***********************************TEST 5B*****************************************************\n");
        fprintf(fp,"Now if the data is not available in the data base \n");
        fprintf(fp,"Expected result is it should return: No data found \n ");
        ASSERT_TEST(display_leaves(555,2020)==3);
        fprintf(fp,"***********************************TEST 5C*****************************************************\n");
        fprintf(fp,"Now we show the data available in the database...\n");
        fprintf(fp,"Expected result is it should return: table show the data of leave\n ");
        ASSERT_TEST(display_leaves(2,2009)==1);
        fprintf(fp,"***********************************TEST 5D*****************************************************\n");
        fprintf(fp,"Now if the the employee does not exist \n");
        fprintf(fp,"Expected result is it should return: No data found \n ");
        ASSERT_TEST(display_leaves(1234,2020)==3);
        
        fprintf(fp,"\n\n\n\n*********************************TEST 6 for  Display Salary**********************************************\n");
        fprintf(fp,"***********************************TEST 6A*****************************************************\n");
        fprintf(fp,"We are first going to raise a display salary request with wrong option\n");
        fprintf(fp,"Expected result is it should return error\n ");
        ASSERT_TEST(display_salary(2,8)==3);
        fprintf(fp,"***********************************TEST 6B*****************************************************\n");
        fprintf(fp,"Now if the employee id is wrong\n");
        fprintf(fp,"Expected result is it should return error \n ");
        ASSERT_TEST(display_salary(100,1)==6);
        fprintf(fp,"***********************************TEST 6C*****************************************************\n");
        fprintf(fp,"Now we show the data available in the database...\n");
        fprintf(fp,"Expected result is it should return: table show the data of salary\n ");
        ASSERT_TEST(display_salary(2,1)==0);
        
        fprintf(fp,"\n\n\n*****  TEST 7 Function: char* leave_request(int emp_id,int dd,int mm,int yy,int no_of_days,char leave_type[]) ****\n");
        fprintf(fp,"*********************  TEST 7A  **************************************************\n");
        fprintf(fp,"A leave request will be raised with all valid input parameters except the date which is invalid: 123/123/124\n");
        fprintf(fp,"Expected result is that it should return an Invalid date message \n ");
        ASSERT_TEST(strcmp("Invalid Date",leave_request(2,123,123,124,4,"ML"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 7B  **************************************************\n");
        fprintf(fp,"A leave request will be raised with all valid input parameters except the leave type which is invalid: ABCD \n");
        fprintf(fp,"Expected result is it should return Invalid Leave Type \n ");
        ASSERT_TEST(strcmp("Invalid Leave Type",leave_request(2,11,12,2006,4,"ABCD"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 7C  ***************************************************\n");
        fprintf(fp,"A leave request will be raised with employee id that does not exist in the database\n");
        fprintf(fp,"Expected result is it should return User does not exist \n ");
        ASSERT_TEST(strcmp("User does not exist",leave_request(61,11,12,2006,4,"ML"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 7D  **************************************************\n");
        fprintf(fp,"A leave request will be raised with leave_type length greater than 10\n");
        fprintf(fp,"Expected result is that it should return Invalid string length for leave_type message\n ");
        ASSERT_TEST(strcmp("Invalid string length for leave_type",leave_request(2,11,12,2006,4,"ABCDEFGHIJKL"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 7E  **************************************************\n");
        fprintf(fp,"\n A leave request will be raised with all the right parameters and therefore will be raised successfully\n");
        fprintf(fp,"Expected result is it should return a string message Request raised successfully \n ");
        ASSERT_TEST(strcmp("Request raised successfully",leave_request(2,11,12,2009,4,"ML"))==0);
        
        fprintf(fp,"\n\n\n*****  TEST 8 Function: char* employee_request(int emp_id,char dept[],char designation[]) ****\n");
        fprintf(fp,"*********************  TEST 8A  **************************************************\n");
        fprintf(fp,"The employee request function is called to raise employee request but emp_id of the manager who raises request doesn't exist in the database\n");
        fprintf(fp,"Expected result is that it should return User does not exist message \n ");
        ASSERT_TEST(strcmp("User does not exist",employee_request(61,"IT","Technician"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 8B  **************************************************\n");
        fprintf(fp,"The employee request function is called to raise emp request but the department input to function is not the one assigned to the manager \n");
        fprintf(fp,"Expected result is it should return Data Mismatch message as the department does not match the department assigned to the manager\n ");
        ASSERT_TEST(strcmp("Data Mismatch",employee_request(4,"Physics","Technician"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 8C  ***************************************************\n");
        fprintf(fp,"The employee request function is called to raise emp request but the department length is greater than 10\n");
        fprintf(fp,"Expected result is it should return Department string length invalid \n ");
        ASSERT_TEST(strcmp("Department string length invalid",employee_request(4,"Mechanical Engineer","Technician"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 8D  ***************************************************\n");
        fprintf(fp,"The employee request function is called to raise emp request but the designation length is greater than 15\n");
        fprintf(fp,"Expected result is it should return Designation string length invalid \n ");
        ASSERT_TEST(strcmp("Designation string length invalid",employee_request(4,"IT","abcdefghijklmnopqrst"))==0);
        
        fprintf(fp,"\n\n*****************  TEST 8E  **************************************************\n");
        fprintf(fp,"The employee request function is called to raise emp request with right parameters\n");
        fprintf(fp,"Expected result is that it should return Request raised successfully\n ");
        ASSERT_TEST(strcmp("Request raised successfully",employee_request(4,"IT","Technician"))==0);
        
        fprintf(fp,"\n\n\n*****  TEST 9 Function: char* decision_leave_request(int req_id,int k) ****\n");
        fprintf(fp,"*********************  TEST 9A  **************************************************\n");
        fprintf(fp,"The decision leave request function is called but no leave request exists with the input request id\n");
        fprintf(fp,"Expected result is that it should return No such pending leave request exists message \n ");
        ASSERT_TEST(strcmp("No such pending leave request exists",decision_leave_request(75,1))==0);
        
        fprintf(fp,"*********************  TEST 9B  **************************************************\n");
        fprintf(fp,"The decision leave request function is called but decision input: k is invalid as k can be only 1 or 2\n");
        fprintf(fp,"Expected result is that it should return Invalid input for k message \n ");
        ASSERT_TEST(strcmp("Invalid input for k",decision_leave_request(1,4))==0);
        
        fprintf(fp,"*********************  TEST 9C  **************************************************\n");
        fprintf(fp,"The decision leave request function is called with right parameters to approve a leave request id\n");
        fprintf(fp,"Expected result is that it should return Leave request and Leave balance status updated successfully message \n ");
        ASSERT_TEST(strcmp("Leave request and Leave balance status updated successfully",decision_leave_request(1,1))==0);
        
        fprintf(fp,"*********************  TEST 9D  **************************************************\n");
        fprintf(fp,"The decision leave request function is called to approve req id which is already approved\n");
        fprintf(fp,"Expected result is that it should return Request is already approved message \n ");
        ASSERT_TEST(strcmp("Request is already approved",decision_leave_request(1,1))==0);
        
        fprintf(fp,"*********************  TEST 9E  **************************************************\n");
        fprintf(fp,"The decision leave request function is called with right parameters to reject a leave request id\n");
        fprintf(fp,"Expected result is that it should return Request Status Updated successfully message \n ");
        ASSERT_TEST(strcmp("Request Status Updated successfully",decision_leave_request(1,2))==0);
        
        fprintf(fp,"\n\n\n*****  TEST 10 Function: char* employee_rating(int emp_id,int rate, char description[200],int year) ****\n");
        fprintf(fp,"*********************  TEST 10A  **************************************************\n");
        fprintf(fp,"The employee rating function is called with employee id '17' but it is not exist in the database \n");
        fprintf(fp,"Expected result is that it should return Employee ID not found message \n ");
        ASSERT_TEST(strcmp("Employee ID not found",employee_rating(20,1,"Good employee",2020))==0);
        
        fprintf(fp,"*********************  TEST 10B  **************************************************\n");
        fprintf(fp,"The employee rating function is called with rating '10' but the range is from 1 to 5 \n");
        fprintf(fp,"Expected result is that it should return 'Rating does not exceed 5' message \n");
        ASSERT_TEST(strcmp("Rating does not exceed 5",employee_rating(1,10,"Good employee",2020))==0);
        
        fprintf(fp,"*********************  TEST 10C  **************************************************\n");
        fprintf(fp,"The employee rating function is called with rating '0' but the range is from 1 to 5 \n");
        fprintf(fp,"Expected result is that it should return 'Rating canot be 0' message \n");
        ASSERT_TEST(strcmp("Rating cannot be 0",employee_rating(1,0,"Good employee",2020))==0);
        
        fprintf(fp,"*********************  TEST 10D  **************************************************\n");
        fprintf(fp,"The employee rating function is called with year '200' but the range is from 2018 to 2020 \n");
        fprintf(fp,"Expected result is that it should return 'INVALID YEAR' message \n ");
        ASSERT_TEST(strcmp("INVALID year",employee_rating(1,3,"Good employee",200))==0);
        
        fprintf(fp,"*********************  TEST 10E  **************************************************\n");
        fprintf(fp,"The employee rating function is called for the existing employee \n");
        fprintf(fp,"Expected result is that it should return 'Rating Updated' message \n ");
        ASSERT_TEST(strcmp("Rating Updated",employee_rating(2,1,"Good employee",2020))==0);
        
        fprintf(fp,"\n\n\n*****  TEST 11 Function: char* view_raised_grievances(int choice,int gri_choice) ****\n");
        fprintf(fp,"*********************  TEST 11A  **************************************************\n");
        fprintf(fp,"The view raised grievances function is called when no grievance is added in the table 'grievances' \n");
        fprintf(fp,"Expected result is that it should return 'No Record with the query' message \n ");
        ASSERT_TEST(strcmp("No Record with the query",view_raised_grievances(1,3))==0);
        
        fprintf(fp,"*********************  TEST 11B  **************************************************\n");
        fprintf(fp,"The view raised grievances function is called when grievance is in the table 'grievances' \n");
        fprintf(fp,"Expected result is that it should return 'Grievance Found for the employee ID' message \n ");
        ASSERT_TEST(strcmp("Grievance Found for the employee ID",view_raised_grievances(1,2))==0);
        
        fprintf(fp,"*********************  TEST 11C  **************************************************\n");
        fprintf(fp,"The view raised grievances function is called with right parameters \n");
        fprintf(fp,"Expected result is that it should return 'Grievance Found' message \n ");
        ASSERT_TEST(strcmp("Grievance Found",view_raised_grievances(2,1))==0);
        
        fprintf(fp,"*********************  TEST 11D  **************************************************\n");
        fprintf(fp,"The view raised grievances function is called when employee ID is invalid \n");
        fprintf(fp,"Expected result is that it should return 'Employee ID not found' message \n ");
        ASSERT_TEST(strcmp("Employee ID not found",view_raised_grievances(1,100))==0);
        
        fprintf(fp,"*********************  TEST 11E  **************************************************\n");
        fprintf(fp,"The view raised grievances function is called with wrong choice number \n");
        fprintf(fp,"Expected result is that it should return 'Wrong choice' message \n ");
        ASSERT_TEST(strcmp("Wrong choice",view_raised_grievances(4,1))==0);
        
        fprintf(fp,"\n\n\n*****  TEST 12 Function: char* add_salary(int e_id,char sal_type[20], float salary,char desig[50]) ****\n");
        fprintf(fp,"*********************  TEST 12A  **************************************************\n");
        fprintf(fp,"The add salary function is called but with wrong employee ID \n");
        fprintf(fp,"Expected result is that it should return 'Employee ID not found' message \n ");
        ASSERT_TEST(strcmp("Employee ID not found",add_salary(100,"Salaried",110.01,"developer"))==0);
        
        fprintf(fp,"*********************  TEST 12B  **************************************************\n");
        fprintf(fp,"The add salary function is called with right parameters \n");
        fprintf(fp,"Expected result is that it should return 'Salary added successfully' message \n ");
        ASSERT_TEST(strcmp("Salary added successfully",add_salary(5,"Salaried",110.01,"developer"))==0);
        
        fprintf(fp,"*********************  TEST 12C  **************************************************\n");
        fprintf(fp,"The add salary function is called but with wrong salary_type \n");
        fprintf(fp,"Expected result is that it should return 'Wrong salary_type' message \n ");
        ASSERT_TEST(strcmp("Wrong salary_type",add_salary(1,"Active",110.01,"developer"))==0);
    }
}
