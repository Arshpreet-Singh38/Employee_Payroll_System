CREATE DATABASE `payroll`; 
use payroll;
drop TABLE if exists  `salary`;
CREATE TABLE `salary` (
 `emp_id` int(11) NOT NULL,
  `salary_type` varchar(8) NOT NULL,
  `salary` double DEFAULT NULL,
  `sal_year` varchar(5) NOT NULL,
 `designation` varchar(45) NOT NULL,
  PRIMARY KEY (`emp_id`,`sal_year`)
) ;

drop table if exists  `emp_details` ;
CREATE TABLE  `emp_details` (
  `emp_id` int(11) NOT NULL AUTO_INCREMENT,
  `name1` varchar(45) NOT NULL,
  `name2` varchar(45) NOT NULL,
  `department` varchar(45) DEFAULT NULL,
  `designation` varchar(45) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  `address_l1` varchar(45) DEFAULT NULL,
  `address_l2` varchar(45) DEFAULT NULL,
  `phonen` varchar(45) DEFAULT NULL,
  `gender` varchar(10) DEFAULT NULL,
  `email` varchar(60) DEFAULT NULL,
  `day` int(11) DEFAULT NULL,
  `month` int(11) DEFAULT NULL,
  `year` int(11) DEFAULT NULL,
  `emp_type` varchar(45) DEFAULT NULL,
  
  PRIMARY KEY (`emp_id`)
) ;


drop table if exists  `emp_perfor` ;
CREATE TABLE `emp_perfor` (
  `emp_id` int NOT NULL,
  `rating` int NOT NULL,
  `description` varchar(45) DEFAULT NULL,
  `year` int NOT NULL
);


drop table if exists  `grievances` ;
CREATE TABLE `grievances` (
  `response_number` int NOT NULL AUTO_INCREMENT,
  `emp_id` int DEFAULT NULL,
  `description` varchar(200) NOT NULL,
  PRIMARY KEY (`response_number`)
);



drop table if exists  `daily_attendance` ;
CREATE TABLE `daily_attendance` (
  `Attend_id` int(15) NOT NULL AUTO_INCREMENT,
  `emp_id` int(15) NOT NULL,
  `department` varchar(50) NOT NULL,
  `designation` varchar(50) NOT NULL,
  `attend_month` int(5) DEFAULT NULL,
  `attend_year` int(5) DEFAULT NULL,
  `a1` varchar(5) DEFAULT NULL,
  `a2` varchar(5) DEFAULT NULL,
  `a3` varchar(5) DEFAULT NULL,
  `a4` varchar(5) DEFAULT NULL,
  `a5` varchar(5) DEFAULT NULL,
  `a6` varchar(5) DEFAULT NULL,
  `a7` varchar(5) DEFAULT NULL,
  `a8` varchar(5) DEFAULT NULL,
  `a9` varchar(5) DEFAULT NULL,
  `a10` varchar(5) DEFAULT NULL,
  `a11` varchar(5) DEFAULT NULL,
  `a12` varchar(5) DEFAULT NULL,
  `a13` varchar(5) DEFAULT NULL,
  `a14` varchar(5) DEFAULT NULL,
  `a15` varchar(5) DEFAULT NULL,
  `a16` varchar(5) DEFAULT NULL,
  `a17` varchar(5) DEFAULT NULL,
  `a18` varchar(5) DEFAULT NULL,
  `a19` varchar(5) DEFAULT NULL,
  `a20` varchar(5) DEFAULT NULL,
  `a21` varchar(5) DEFAULT NULL,
  `a22` varchar(5) DEFAULT NULL,
  `a23` varchar(5) DEFAULT NULL,
  `a24` varchar(5) DEFAULT NULL,
  `a25` varchar(5) DEFAULT NULL,
  `a26` varchar(5) DEFAULT NULL,
  `a27` varchar(5) DEFAULT NULL,
  `a28` varchar(5) DEFAULT NULL,
  `a29` varchar(5) DEFAULT NULL,
  `a30` varchar(5) DEFAULT NULL,
  `a31` varchar(5) DEFAULT NULL,
  `days` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`Attend_id`)
);



drop table if exists  `login_details` ;
CREATE TABLE `login_details` (
  `emp_id` int(11) NOT NULL,
  `password` varchar(45) NOT NULL,
  `emp_type` varchar(45) NOT NULL,
  `status` varchar(45) NOT NULL,
  `DOL` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`emp_id`)
) ;


drop table if exists  `hourly_attendance` ;
CREATE TABLE `hourly_attendance` (
  `att_id` int(11) NOT NULL AUTO_INCREMENT,
  `emp_id` varchar(8) DEFAULT NULL,
  `att_day` varchar(3) DEFAULT NULL,
  `att_month` varchar(5) DEFAULT NULL,
  `att_year` varchar(5) DEFAULT NULL,
  `In_time` varchar(20) DEFAULT NULL,
  `out_time` varchar(20) DEFAULT NULL,
  `days` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`att_id`)
);



drop table if exists  `leave_details` ;
CREATE TABLE `leave_details` (
  `Leave_id` int(15) NOT NULL AUTO_INCREMENT,
  `Emp_id` int(15) NOT NULL,
  `Medical_leave` varchar(2) NOT NULL,
  `Paid_leave` varchar(2) NOT NULL,
  `Leave_year` varchar(5) NOT NULL,
  `Balance_ML` varchar(2) DEFAULT NULL,
  `Balance_PL` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`Leave_id`)
);


drop table if exists  `request_form` ;
CREATE TABLE `request_form` (
  `Req_id` int(12) NOT NULL AUTO_INCREMENT,
  `emp_id` int(25) NOT NULL,
  `date` varchar(15) NOT NULL,
  `descript` varchar(200) DEFAULT NULL,
  `req_status` varchar(2) NOT NULL,
  PRIMARY KEY (`Req_id`)
);



drop table if exists  `employee_request` ;
CREATE TABLE `employee_request` (
  `request_id` int(11) NOT NULL AUTO_INCREMENT,
  `emp_id` int(11) NOT NULL,
  `department` varchar(20) NOT NULL,
  `designation` varchar(45) NOT NULL,
  `request_status` varchar(20) NOT NULL,
  PRIMARY KEY (`request_id`)
) ;



drop table if exists  `leave_request` ;
CREATE TABLE `leave_request` (
  `request_id` int(11) NOT NULL AUTO_INCREMENT,
  `emp_id` int(11) NOT NULL,
  `start_day` int(11) NOT NULL,
  `leave_month` int(11) NOT NULL,
  `leave_year` int(11) NOT NULL,
  `no_of_days` int(11) NOT NULL,
  `leave_type` varchar(45) NOT NULL,
  `status` varchar(45) NOT NULL,
  PRIMARY KEY (`request_id`,`emp_id`)
) ;


drop table if exists `salary_cal`;
CREATE TABLE `salary_cal` (
  `emp_id` int(11) NOT NULL,
  `salary_type` varchar(45) NOT NULL,
  `month` int(11) NOT NULL,
  `year` int(11) NOT NULL,
  `calculated_salary` float DEFAULT NULL,
  `deductions` float DEFAULT NULL,
  `net_pay` float DEFAULT NULL
) ;

insert into `login_details` values (1,"1234","admin","A",NULL);
insert into `login_details` values (2,"1234","employee","A",NULL);
insert into `login_details` values (3,"1234","employee","I",NULL);
insert into `login_details` values (4,"1234","manager","A",NULL);
insert into `login_details` values (5,"1234","employee","A",NULL);

INSERT INTO `leave_details` (`Emp_id`,`Medical_leave`,`Paid_leave`,`Leave_year`,`Balance_ML`,`Balance_PL`) VALUES(2,10,10,2009,8,7);


INSERT INTO `emp_details`
(`name1`,
`name2`,
`department`,
`designation`,
`age`,
`address_l1`,
`address_l2`,
`phonen`,
`gender`,
`email`,
`day`,
`month`,
`year`,
`emp_type`,
`emp_id`)
VALUES
('Jagdeep',
'Singh',
'HR',
'HR Admin',
'25',
'1315,2850 Cedarwood',
'Ottawa Canada',
'6477057364',
'Male',
'baidwanjagdeep94@gmail.com',
12,
11,
2018,
'Admin',
1);


INSERT INTO `emp_details`
(`name1`,
`name2`,
`department`,
`designation`,
`age`,
`address_l1`,
`address_l2`,
`phonen`,
`gender`,
`email`,
`day`,
`month`,
`year`,
`emp_type`,
`emp_id`)
VALUES
('Varinder',
'Singh',
'IT',
'Manager',
'27',
'1315,2850 Cedarwood',
'Ottawa Canada',
'6477057462',
'Male',
'Varinder94@gmail.com',
12,
11,
2018,
'Manager',
4);


INSERT INTO `emp_details`
(`name1`,
`name2`,
`department`,
`designation`,
`age`,
`address_l1`,
`address_l2`,
`phonen`,
`gender`,
`email`,
`day`,
`month`,
`year`,
`emp_type`,
`emp_id`)
VALUES
('Arshpreet',
'Singh',
'IT',
'Developer',
'22',
'1315,2850 Cedarwood',
'Ottawa Canada',
'6477057462',
'Male',
'arshpreet94@gmail.com',
12,
11,
2018,
'Employee',
3);


INSERT INTO `emp_details`
(`name1`,
`name2`,
`department`,
`designation`,
`age`,
`address_l1`,
`address_l2`,
`phonen`,
`gender`,
`email`,
`day`,
`month`,
`year`,
`emp_type`,
`emp_id`)
VALUES
('Puneet',
'Singh',
'IT',
'Developer',
'23',
'1315,2850 Cedarwood',
'Ottawa Canada',
'6477057362',
'Male',
'pkmultani94@gmail.com',
12,
11,
2018,
'Employee',
2);

INSERT INTO `emp_details`
(`name1`,
`name2`,
`department`,
`designation`,
`age`,
`address_l1`,
`address_l2`,
`phonen`,
`gender`,
`email`,
`day`,
`month`,
`year`,
`emp_type`,
`emp_id`)
VALUES
('Amitav',
'Shaw',
'IT',
'Developer',
'28',
'1315,3 Meadowlands',
'Ottawa Canada',
'6132641524',
'Male',
'amitavshaw4@gmail.com',
12,
11,
2018,
'Employee',
5);

INSERT INTO `salary`
(`emp_id`,
`salary_type`,
`salary`,
`sal_year`,
`designation`)
VALUES
(1,
"Salaried",
2000,
2020,"Admin");


INSERT INTO `salary`
(`emp_id`,
`salary_type`,
`salary`,
`sal_year`,
`designation`)
VALUES
(2,
"Hourly",
45,
2020,"Developer");

INSERT INTO `salary`
(`emp_id`,
`salary_type`,
`salary`,
`sal_year`,
`designation`)
VALUES
(3,
"Hourly",
15,
2019,"Developer");

INSERT INTO `salary`
(`emp_id`,
`salary_type`,
`salary`,
`sal_year`,
`designation`)
VALUES
(4,
"Salaried",
1500,
2020,"Manager");

INSERT INTO `salary_cal`
(`emp_id`,
`salary_type`,
`month`,
`year`,
`calculated_salary`,
`deductions`,
`net_pay`)
VALUES
(2,
"Hourly",
2,
2020,
1000,
200,
800);

INSERT INTO `emp_perfor`
(`emp_id`,
`rating`,
`description`,
`year`)
VALUES
(1,
3,
"Good Employee",
2020
);

INSERT INTO `emp_perfor`
(`emp_id`,
`rating`,
`description`,
`year`)
VALUES
(2,
4,
"Excellent work and jolly nature",
2020
);

INSERT INTO `grievances`
(`emp_id`,
`description`
)
VALUES
(1,
"Company laptop lock out"
);

INSERT INTO `grievances`
(`emp_id`,
`description`
)
VALUES
(2,
"Unable to access my company email account."
);