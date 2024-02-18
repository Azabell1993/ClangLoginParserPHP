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

## Setup
Clone the repository to your local machine

## bash
-------------
$ sudo git clone https://github.com/Azabell1993/loginParserC.git  
$ cd ClangLoginParser  

## Usage
-------------
Update the connect.json file with your MySQL database connection details:

## json
-------------
```  
{
  "db_host": "localhost",
  "db_user": "your_db_user",
  "db_pass": "your_db_password",
  "db_name": "your_db_name"
}
```  

## Makefile  
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
- connector.h: Header file containing JSON parsing and database connection functions.
- login.c: Main program file containing the login and MySQL interaction logic.
- connect.json: JSON configuration file for MySQL connection details.
- Makefile: Makefile for compiling the program.
- PHP Script: PHP script for web-based user authentication.
   
## Notes
The program establishes a connection to the MySQL database using the details provided in connect.json.  
It performs user authentication and executes sample queries using MySQL.  
The connector.h header provides JSON parsing utilities to read the connect.json file.  

## MAC
```
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
```  

## Contributing
Contributions are welcome! If you find any issues or want to enhance the example, feel free to create a pull request.  

## License
This project is licensed under the MIT License - see the LICENSE file for details.  

## Build Example

WARNING :
-----------------------
```
// Uncomment the line below if you want to test in the console
// If you wish to test the functionality in the console environment,
// please comment out the following line before running 'make':
// sha256(password, hashedPassword); // Hash the user's password using SHA-256
```  
  

```
----------------------------------------------------------------------------------------------------------------------------------
-- The warnings about 'SHA256_Init', 'SHA256_Update', and 'SHA256_Final' being deprecated since OpenSSL 3.0 can be safely ignored.
----------------------------------------------------------------------------------------------------------------------------------
azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ make
gcc -I/usr/include/mysql -g -fPIC -std=c99 -o loginSecurityLib login.c -L/usr/lib/mysql -lmysqlclient -lcrypto
login.c: In function ‘sha256’:
login.c:85:5: warning: ‘SHA256_Init’ is deprecated: Since OpenSSL 3.0 [-Wdeprecated-declarations]
   85 |     SHA256_Init(&sha256);
      |     ^~~~~~~~~~~
In file included from login.c:5:
/usr/include/openssl/sha.h:73:27: note: declared here
   73 | OSSL_DEPRECATEDIN_3_0 int SHA256_Init(SHA256_CTX *c);
      |                           ^~~~~~~~~~~
login.c:86:5: warning: ‘SHA256_Update’ is deprecated: Since OpenSSL 3.0 [-Wdeprecated-declarations]
   86 |     SHA256_Update(&sha256, string, strlen(string));
      |     ^~~~~~~~~~~~~
In file included from login.c:5:
/usr/include/openssl/sha.h:74:27: note: declared here
   74 | OSSL_DEPRECATEDIN_3_0 int SHA256_Update(SHA256_CTX *c,
      |                           ^~~~~~~~~~~~~
login.c:87:5: warning: ‘SHA256_Final’ is deprecated: Since OpenSSL 3.0 [-Wdeprecated-declarations]
   87 |     SHA256_Final(hash, &sha256);
      |     ^~~~~~~~~~~~
In file included from login.c:5:
/usr/include/openssl/sha.h:76:27: note: declared here
   76 | OSSL_DEPRECATEDIN_3_0 int SHA256_Final(unsigned char *md, SHA256_CTX *c);
      |                           ^~~~~~~~~~~~
gcc -shared -o connector.so connector.h -I/usr/include/mysql -g -fPIC -std=c99 -L/usr/lib/mysql -lmysqlclient 
cp connector.so libconnector.so


azabell@azabell-kernelhost:/var/www/cloud/loginParserC$ ./loginSecurityLib admin 'admin20081!@#'
=======================================
db_host localhost
db_user admin
db_pass admin
db_name admin

Connect Sucess!!
 !! RUN QUERY !!
=====================================================================================
	 SELECT DISTINCT                            IFNULL(                             CAST(                                   (                                       SELECT CASE                                         WHEN COUNT(*) > 0 THEN 1                                            ELSE 0                                      END                                     FROM (                                          SELECT A1.USER_INFONUM                                          FROM ADMIN AS A1                                            INNER JOIN ADMIN_SECURITY AS S ON ADMIN.USERNAME = S.USERNAME                                           WHERE ADMIN.USER_INFONUM = S.USER_SECURITY_INFO                                          AND   ADMIN.USERNAME = S.USERNAME                                        ) A                                 ) AS CHAR                               ), 0                            ) AS LOGIN                      FROM ADMIN                  WHERE                     USERNAME = 'admin'                     AND PASSWORD = '${hash password value}'; 
=====================================================================================
 	 query_stat_chk : 0 
 !! RUN QUERY !!
Username: admin
Hashed Password: '${hash password value}'; 
=====================================================================================
	 SELECT DISTINCT ad.USERNAME                            FROM ADMIN ad                                                   WHERE USERNAME 		= 'admin'                           AND PASSWORD 	= '${hash password value}'; 
=====================================================================================
 	 query_stat : 0
  !! RUN QUERY !!
=====================================================================================
	 select now() 
=====================================================================================
 	 query_stat_date : 0
==============================Login Successful!!!===================================
Member admin, welcome. 
##############################################
Login time:  2024-02-18 14:31:09
SELECT DISTINCT                            IFNULL(                             CAST(                                   (                                       SELECT CASE                                         WHEN COUNT(*) > 0 THEN 1                                            ELSE 0                                      END                                     FROM (                                          SELECT A1.USER_INFONUM                                          FROM ADMIN AS A1                                            INNER JOIN ADMIN_SECURITY AS S ON ADMIN.USERNAME = S.USERNAME                                           WHERE ADMIN.USER_INFONUM = S.USER_SECURITY_INFO                                          AND   ADMIN.USERNAME = S.USERNAME                                        ) A                                 ) AS CHAR                               ), 0                            ) AS LOGIN                      FROM ADMIN                  WHERE                     USERNAME = 'admin'                     AND PASSWORD = '${hash password value}'; 

```  

Replace your-username and your-repo-name with your GitHub username and repository name respectively. Make sure to update the MySQL connection details in connect.json before running the program.

Feel free to customize this README to include any additional information or instructions specific to your project.


## PHP Integration for Web Authentication
To integrate this C program with a web interface for user authentication, use the following PHP script:  
  
```
<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    // Use the hashed password instead of the original password
    $hashedPassword = $_POST['hashedPassword'];
    
    // Display the entered username and hashed password (with HTML escape)
    echo "<h3>Entered Information:</h3>";
    echo "Username: " . htmlspecialchars($username) . "<br>";
    echo "Hashed Password: " . htmlspecialchars($hashedPassword) . "<br>";
    
    $safe_username = escapeshellarg($username);
    
    // Use the hashed password when calling the C program
    $command = "/var/www/cloud/loginParserC/loginSecurityLib {$safe_username} {$hashedPassword} 2>&1";
    $output = shell_exec($command);
    
    /**
     * 
     * Notice )
     * 
     * The mechanism for checking login success between a C program and a PHP script involves comparing the output string of the C program with a condition in PHP. 
     * When the login is successful in the C program, it prints a string like "==============================Login Successful!!!===================================\n". 
     * Then, the PHP script executes this C program using the shell_exec() function and stores its output in the $output variable.
     * The PHP script examines the contents of this $output variable to check if it contains a specific string. In this case, the presence of the "Login Successful!!!" 
     * string is used to determine the success of the login. If this string exists within $output, it is considered a successful login, 
     * and the user is redirected to the homepage of localhost. Conversely, if this string does not exist, a login failure message is displayed to the user.
     * Through this process, the integration between the login verification logic of the C program and the PHP web interface is implemented, 
     * allowing users to be redirected to the homepage of localhost upon successful login.
     */
    // Check for login success
    if (strpos($output, "Login Successful!!!") !== false) { // "Login Successful!!!"
        header("Location: http://localhost/");
        exit();
    } else {
        echo "<pre>Login Failed!</pre>";
        echo "<pre>$output</pre>";
    }
}
?>

<form method="post" onsubmit="return hashPassword();">
    Username: <input type="text" name="username"><br>
    Password: <input type="password" id="password"><br>
    <!-- Add a hidden field to store the hashed password -->
    <input type="hidden" name="hashedPassword" id="hashedPassword">
    <input type="submit" value="Login">
</form>

<script src="https://cdnjs.cloudflare.com/ajax/libs/crypto-js/4.0.0/crypto-js.min.js"></script>
<script>
function hashPassword() {
    var password = document.getElementById('password').value;
    var hashedPassword = CryptoJS.SHA256(password).toString();
    
    // Set the hashed password in the hidden field
    document.getElementById('hashedPassword').value = hashedPassword;
    
    // Clear the original password field
    document.getElementById('password').value = '';
    return true; // Continue with form submission
}
</script>
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

## Console Test  
```
    // Uncomment the line below if you want to test in the console
    // console에서 테스트를 해보고 싶다면 아래 줄의 주석을 해제한다.
    // sha256(password, hashedPassword);
```  
  
## Notes
The program establishes a connection to the MySQL database using the details provided in connect.json. It performs user authentication and executes sample queries using MySQL. The connector.h header provides JSON parsing utilities to read the connect.json file.

Contributing
Contributions are welcome! If you find any issues or want to enhance the example, feel free to create a pull request.

License
This project is licensed under the MIT License - see the LICENSE file for details.

Build Example
[Include build and execution examples as previously provided]

Replace your_db_user, your_db_password, and your_db_name with your actual MySQL database details before running the program. Customize the README to include any additional information or instructions specific to your project.