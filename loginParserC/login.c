//
// Created by azabell on 24.02.18
//
#include </usr/include/mysql/mysql.h>
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

int main(int argc, char *argv[])
{
    LOGINAPI login;
    login.connection       =   NULL;
 
    // argc는 인자의 개수, argv는 인자의 배열
    if (argc < 3) {
        printf("사용법: %s <username> <password>\n", argv[0]);
        return 1;
    }
    char* username = argv[1];
    char* password = argv[2];
    // 이후 로직에서 username과 password 사용

    login.user.memberId     = username;
    login.user.memberPw     = password;

    login.query            =   malloc(sizeof(char*)*500);
    login.query_chk        =   malloc(sizeof(char*)*2000);
    login.query_chk_date   =   malloc(sizeof(char*)*800);

    printf("=======================================\n");
    printf("db_host %s\n", get_tokens(0));
    printf("db_user %s\n", get_tokens(1));
    printf("db_pass %s\n", get_tokens(2));
    printf("db_name %s\n", get_tokens(3));


    mysql_init(&login.conn);

    // DB 연결
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

    sprintf(login.query_chk, SQL_SELECT, username, password);

    login.query_stat_chk = mysql_query(login.connection, login.query_chk);

    if (login.query_stat_chk != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&login.conn));
        printf("로그인에 실패하셨습니다!\n");
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
        printf("==============================로그인 성공!!!===================================\n");
        printf("회원 "); printf("%2s", login.sql_row[0]); printf("님 환영합니다. \n");
        printf("##############################################\n");
        printf("로그인 시각 :  %20s\n", login.sql_row_date[0]);
        return 1;
    } else if( ((login.sql_row_chk=mysql_fetch_row(login.sql_result)) == NULL) ) {
        printf("로그인 실패!!\n");
        return (0);
    }

    mysql_free_result(login.sql_result);
    mysql_free_result(login.sql_result);
    mysql_free_result(login.sql_result_chk);
    mysql_free_result(login.sql_result_date);


    // DB 연결 닫기
    mysql_close(login.connection);

    return 0;
}
