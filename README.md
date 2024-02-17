# loginParserC

C Parser and MySQL Login Integration Example
=============

This repository contains an example implementation of a C parser and MySQL login integration. The program demonstrates how to connect to a MySQL database, authenticate users, and perform queries using C programming language.

Prerequisites
-------------
Before you begin, ensure you have the following requirements in place:

MySQL Server installed and running.
GCC compiler for compiling C code.
MySQL development libraries for linking the MySQL client.

Setup
-------------
Clone the repository to your local machine

bash
-------------
git clone https://github.com/Azabell1993/loginParserC.git  
cd ClangLoginParser  

Usage
-------------
Update the connect.json file with your MySQL database connection details:

json
-------------
```  
{
  "db_host": "localhost",
  "db_user": "your_db_user",
  "db_pass": "your_db_password",
  "db_name": "your_db_name"
}
```  

Makefile  
------------
```  
CC = gcc
CFLAGS = -I/usr/include/mysql -g -fPIC -std=c99
LDFLAGS = -L/usr/lib/mysql -lmysqlclient

SOURCES = login.c
EXECUTABLE = loginSecurityLib
CONNECTOR_SRC = connector.h
CONNECTOR_LIB = connector.so
LIBCONNECTOR = libconnector.so

# 기본 타겟
all: $(EXECUTABLE) connector

# 기존 C 프로그램 빌드
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# connector.so 생성
connector: $(CONNECTOR_SRC)
	$(CC) -shared -o $(CONNECTOR_LIB) $(CONNECTOR_SRC) $(CFLAGS) $(LDFLAGS)
	cp $(CONNECTOR_LIB) $(LIBCONNECTOR)

# 정리
clean:
	rm -f $(EXECUTABLE) $(CONNECTOR_LIB) $(LIBCONNECTOR) 
```  

Run the compiled executable:
-------------
```  
./loginSecurithLib
```  

Follow the prompts to enter a username and password for authentication.
  
  
## Structure
-------------
- connector.h: Header file containing JSON parsing and database connection functions.
- login.c: Main program file containing the login and MySQL interaction logic.
- connect.json: JSON configuration file for MySQL connection details.
- Makefile: Makefile for compiling the program.
- PHP Script: PHP script for web-based user authentication.
   
## Notes
-------------
The program establishes a connection to the MySQL database using the details provided in connect.json.  
It performs user authentication and executes sample queries using MySQL.  
The connector.h header provides JSON parsing utilities to read the connect.json file.  

## MAC
-------------
```
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
```  

## Contributing
-------------
Contributions are welcome! If you find any issues or want to enhance the example, feel free to create a pull request.  

## License
-------------
This project is licensed under the MIT License - see the LICENSE file for details.  

## Build Example
-------------
```
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ sudo make
gcc -I/usr/include/mysql -g -fPIC -std=c99 -o loginSecurityLib login.c -L/usr/lib/mysql -lmysqlclient
gcc -shared -o connector.so connector.h -I/usr/include/mysql -g -fPIC -std=c99 -L/usr/lib/mysql -lmysqlclient
cp connector.so libconnector.so
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ./loginSecurityLib 
사용법: ./loginSecurityLib <username> <password>
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ./loginSecurityLib admin admin1234
=======================================
db_host localhost
db_user admin
db_pass admin
db_name admin

Connect Sucess!!
 !! RUN QUERY !!
=====================================================================================
	 SELECT DISTINCT                            IFNULL(                             CAST(                                   (                                       SELECT CASE                                         WHEN COUNT(*) > 0 THEN 1                                            ELSE 0                                      END                                     FROM (                                          SELECT A1.USER_INFONUM                                          FROM ADMIN AS A1                                            INNER JOIN ADMIN_SECURITY AS S ON ADMIN.USERNAME = S.USERNAME                                           WHERE ADMIN.USER_INFONUM = S.USER_SECURITY_INFO                                          AND   ADMIN.USERNAME = S.USERNAME                                        ) A                                 ) AS CHAR                               ), 0                            ) AS LOGIN                      FROM ADMIN                  WHERE                     USERNAME = 'admin'                     AND PASSWORD = 'admin1234'; 
=====================================================================================
 	 query_stat_chk : 0 
 !! RUN QUERY !!
=====================================================================================
	 SELECT DISTINCT ad.USERNAME                            FROM ADMIN ad                                                   WHERE USERNAME 		= 'admin'                           AND PASSWORD 	= 'admin1234'; 
=====================================================================================
 	 query_stat : 0
  !! RUN QUERY !!
=====================================================================================
	 select now() 
=====================================================================================
 	 query_stat_date : 0
==============================로그인 성공!!!===================================
회원 admin님 환영합니다. 
##############################################
로그인 시각 :   2024-02-18 02:09:35
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ./loginSecurityLib admin admin1234234
=======================================
db_host localhost
db_user admin
db_pass admin
db_name admin

Connect Sucess!!
 !! RUN QUERY !!
=====================================================================================
	 SELECT DISTINCT                            IFNULL(                             CAST(                                   (                                       SELECT CASE                                         WHEN COUNT(*) > 0 THEN 1                                            ELSE 0                                      END                                     FROM (                                          SELECT A1.USER_INFONUM                                          FROM ADMIN AS A1                                            INNER JOIN ADMIN_SECURITY AS S ON ADMIN.USERNAME = S.USERNAME                                           WHERE ADMIN.USER_INFONUM = S.USER_SECURITY_INFO                                          AND   ADMIN.USERNAME = S.USERNAME                                        ) A                                 ) AS CHAR                               ), 0                            ) AS LOGIN                      FROM ADMIN                  WHERE                     USERNAME = 'admin'                     AND PASSWORD = 'admin1234234'; 
=====================================================================================
 	 query_stat_chk : 0 
 !! RUN QUERY !!
=====================================================================================
	 SELECT DISTINCT ad.USERNAME                            FROM ADMIN ad                                                   WHERE USERNAME 		= 'admin'                           AND PASSWORD 	= 'admin1234234'; 
=====================================================================================
 	 query_stat : 0
  !! RUN QUERY !!
=====================================================================================
	 select now() 
=====================================================================================
 	 query_stat_date : 0
로그인 실패!!
```  

Replace your-username and your-repo-name with your GitHub username and repository name respectively. Make sure to update the MySQL connection details in connect.json before running the program.

Feel free to customize this README to include any additional information or instructions specific to your project.


## PHP Integration for Web Authentication
-------------
To integrate this C program with a web interface for user authentication, use the following PHP script:  
  
```
<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // 사용자 입력 처리
    $username = $_POST['username']; // escapeshellarg 호출 전 원본 데이터 유지
    $password = $_POST['password']; // escapeshellarg 호출 전 원본 데이터 유지

    // 입력된 사용자 이름과 비밀번호 출력 (HTML escape 처리)
    echo "<h3>입력된 정보:</h3>";
    echo "아이디: " . htmlspecialchars($username) . "<br>";
    echo "비밀번호: " . htmlspecialchars($password) . "<br>";

    // escapeshellarg 함수를 사용하여 쉘 명령어 인자를 안전하게 처리
    $safe_username = escapeshellarg($username);
    $safe_password = escapeshellarg($password);

    // C 프로그램 실행 및 결과 출력
    $command = "/var/www/cloud/loginParserC/loginSecurityLib {$safe_username} {$safe_password} 2>&1";

    $output = shell_exec($command);
    
    // 로그인 성공 여부 확인 (예시)
    if (strpos($output, "로그인 성공!!!") !== false) {
        // 로그인 성공 시 localhost/ 로 리다이렉트
        header("Location: http://localhost/");
        exit();
    } else {
        // 로그인 실패 시 메시지 출력
        echo "<pre>로그인에 실패하셨습니다!</pre>";
        echo "<pre>$output</pre>";
    }
}
?>

<form method="post">
    아이디: <input type="text" name="username"><br>
    비밀번호: <input type="password" name="password"><br>
    <input type="submit" value="로그인">
</form>
```  

## make && make clean  
```
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ sudo make
gcc -I/usr/include/mysql -g -fPIC -std=c99 -o loginSecurityLib login.c -L/usr/lib/mysql -lmysqlclient
gcc -shared -o connector.so connector.h -I/usr/include/mysql -g -fPIC -std=c99 -L/usr/lib/mysql -lmysqlclient
cp connector.so libconnector.so
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ls
connect.json  connector.h  connector.so  createDb.txt  golangbuild.txt  libconnector.so  login.c  loginSecurityLib  Makefile  README.md
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ sudo make clean
rm -f loginSecurityLib connector.so libconnector.so 
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ls
connect.json  connector.h  createDb.txt  golangbuild.txt  login.c  Makefile  README.md
```  

## Notes
-------------
The program establishes a connection to the MySQL database using the details provided in connect.json. It performs user authentication and executes sample queries using MySQL. The connector.h header provides JSON parsing utilities to read the connect.json file.

Contributing
Contributions are welcome! If you find any issues or want to enhance the example, feel free to create a pull request.

License
This project is licensed under the MIT License - see the LICENSE file for details.

Build Example
[Include build and execution examples as previously provided]

Replace your_db_user, your_db_password, and your_db_name with your actual MySQL database details before running the program. Customize the README to include any additional information or instructions specific to your project.

