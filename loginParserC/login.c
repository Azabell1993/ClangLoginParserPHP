//
// Created by azabell on 24.02.18
//
#include </usr/include/mysql/mysql.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "connector.h"

#define CHOP(x) x[strlen(x) - 1] = '\0'
extern char *get_tokens(int token_select_num);

#define SQL_SELECT "SELECT DISTINCT        \
                    IFNULL(     \
                        CAST(       \
                            (       \
                                SELECT CASE     \
                                    WHEN COUNT(*) > 0 THEN 1        \
                                    ELSE 0      \
                                END     \
                                FROM (      \
                                    SELECT A1.USER_INFONUM      \
                                    FROM ADMIN AS A1        \
                                    INNER JOIN ADMIN_SECURITY AS S ON ADMIN.USERNAME = S.USERNAME       \
                                    WHERE ADMIN.USER_INFONUM = S.USER_SECURITY_INFO      \
                                    AND   ADMIN.USERNAME = S.USERNAME        \
                                ) A     \
                            ) AS CHAR       \
                        ), 0        \
                    ) AS LOGIN      \
                FROM ADMIN  \
                WHERE \
                    USERNAME = '%s' \
                    AND PASSWORD = '%s';"

#define SQL_LOGIN       "SELECT DISTINCT ad.USERNAME    \
                        FROM ADMIN ad                   \
                                WHERE USERNAME 		= '%s'   \
                        AND PASSWORD 	= '%s';"   //"

#define SQL_SHOW_DATE "select now()"

typedef struct LOGINAPI {
    struct USERSPACE {
        char *memberId;
        char *memberPw;
    } user;
    MYSQL       conn;
    MYSQL       *connection;
    MYSQL_RES   *sql_result;
    MYSQL_RES   *sql_result_chk;
    MYSQL_RES   *sql_result_date;

    MYSQL_ROW   sql_row_chk;
    MYSQL_ROW   sql_row_date;
    MYSQL_ROW   sql_row;

    char *query;
    char *query_chk;
    char *query_chk_date;

    int query_stat;
    int query_stat_chk;
    int query_stat_date;


} LOGINAPI;

// Function to convert a password to a SHA-256 hash
// 패스워드를 SHA-256 해시로 변환하는 함수
void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65]) {
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

// Function to hash the user's password with SHA-256
// 사용자의 패스워드를 SHA-256으로 해싱하는 함수
void sha256(const char *string, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    sha256_hash_string(hash, outputBuffer);
}

// Logic to remove single quotes from argv[2]
// argv[2]에서 단일 인용부호를 제거하는 로직
char* processArgument(char* arg) {
    size_t len = strlen(arg);
    // In case it's wrapped in single quotes
    if (arg[0] == '\'' && arg[len - 1] == '\'') {   // Remove the last single quote
        arg[len - 1] = '\0'; // Skip the first single quote and return
        return arg + 1;
    }
    return arg; // Return as is if there are no single quotes
}

int main(int argc, char *argv[])
{
    LOGINAPI login;
    login.connection       =   NULL;
 
    // argc is the number of arguments, argv is the array of arguments
    // argc는 인자의 개수, argv는 인자의 배열
    if (argc < 3) {
        printf("사용법: %s <username> <password>\n", argv[0]);
        return 1;
    }
    char* username = argv[1];
    char* password = argv[2];
    char* hashedPassword = processArgument(argv[2]); // 해시된 패스워드 처리


    // WARNING:
    // Uncomment the line below if you want to test in the console
    // If you wish to test the functionality in the console environment,
    // please comment out the following line before running 'make':
    // sha256(password, hashedPassword); // Hash the user's password using SHA-256

    // ********************************************************************************
    // Uncomment the line below if you want to test in the console
    // console에서 테스트를 해보고 싶다면 아래 줄의 주석을 해제한다.
   sha256(password, hashedPassword); // 사용자의 패스워드를 SHA-256으로 해싱
    // ********************************************************************************

    // Use username and password in the following logic
    // 이후 로직에서 username과 password 사용
    login.user.memberId     = username;
    login.user.memberPw     = hashedPassword;

    login.query            =   malloc(sizeof(char*)*500);
    login.query_chk         =   malloc(sizeof(char*)*2000);
    login.query_chk_date    =   malloc(sizeof(char*)*800);

    printf("=======================================\n");
    printf("db_host %s\n", get_tokens(0));
    printf("db_user %s\n", get_tokens(1));
    printf("db_pass %s\n", get_tokens(2));
    printf("db_name %s\n", get_tokens(3));

    mysql_init(&login.conn);
    
    // DB connection
    login.connection = mysql_real_connect(&login.conn
            , get_tokens(0)
            , get_tokens(1)
            , get_tokens(2)
            , get_tokens(3)
            , 3306,(char *)NULL, 0);
    if(login.connection==NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&login.conn));
        return -1;
    } else {
        printf("\nConnect Sucess!!\n");
    }

    //sprintf(login.query_chk, SQL_SELECT, username, password);
    sprintf(login.query_chk, SQL_SELECT, username, hashedPassword);

    login.query_stat_chk = mysql_query(login.connection, login.query_chk);

    if (login.query_stat_chk != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&login.conn));
        printf("ou have failed to login!!\n");
        return (0);
    } else {
        printf(" !! RUN QUERY !!\n");
        printf("=====================================================================================\n");
        printf("\t %s \n", login.query_chk);
        printf("=====================================================================================\n");
        login.sql_result_chk=mysql_store_result(login.connection);
        printf(" \t query_stat_chk : %d \n", login.query_stat_chk);
        if(login.query_stat_chk == 0) {
            sprintf(login.query, SQL_LOGIN, login.user.memberId, login.user.memberPw);
            login.query_stat = mysql_query(login.connection, login.query);
            printf(" !! RUN QUERY !!\n");
            printf("Username: %s\n", username);
            printf("Hashed Password: %s\n", hashedPassword);
            printf("=====================================================================================\n");
            printf("\t %s \n", login.query);
            printf("=====================================================================================\n");
            login.sql_result = mysql_store_result(login.connection);
            printf(" \t query_stat : %d\n ", login.query_stat);

            // date
            sprintf(login.query_chk_date, SQL_SHOW_DATE);
            login.query_stat_date = mysql_query(login.connection, login.query_chk_date);
            login.sql_result_date = mysql_store_result(login.connection);
            printf(" !! RUN QUERY !!\n");
            printf("=====================================================================================\n");
            printf("\t %s \n", login.query_chk_date);
            printf("=====================================================================================\n");
            printf(" \t query_stat_date : %d\n", login.query_stat_date);
        }
    }

    if(
            ((login.sql_row=mysql_fetch_row(login.sql_result)) != NULL)
            && ((login.sql_row_date=mysql_fetch_row(login.sql_result_date)) != NULL)
            )
    {
        printf("==============================Login Successful!!!===================================\n");
        printf("Member "); printf("%2s", login.sql_row[0]); printf(", welcome. \n");
        printf("##############################################\n");
        printf("Login time: %20s\n", login.sql_row_date[0]);

        printf("%s\n", login.query_chk);

        return 1;
    } else if( ((login.sql_row_chk=mysql_fetch_row(login.sql_result)) == NULL) ) {
        printf("Login Failed!!\n");
        return (0);
    }


    if (login.sql_result_chk != NULL) {
        mysql_free_result(login.sql_result_chk);
    }
    if (login.sql_result_date != NULL) {
        mysql_free_result(login.sql_result_date);
    }
    if (login.sql_result != NULL) {
        mysql_free_result(login.sql_result); // Remove duplicate calls
    }

    // Close DB connection
    mysql_close(login.connection);

    return 0;
}
