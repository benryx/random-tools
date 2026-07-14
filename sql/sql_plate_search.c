#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUERY_LENGTH 1280
#define QUERY_FILENAME "/home/benry/code/c/sql/query.sql"

int main(int argc, char *argv[]) {
        RETCODE rc;
        SQLCHAR classification[64];
        SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";
        SQLCHAR size[12];
        SQLCHAR tracking_numbers[64];
        SQLFLOAT usable_area;
        SQLHDBC dbc;
        SQLHENV env;
        SQLHSTMT stmt;
        SQLINTEGER quantity;
        SQLLEN indicator[9];
        SQLSMALLINT allocated;
        SQLSMALLINT allocated_last = 1;
        SQLSMALLINT n_cols;
        char mill_cert[32];
        char name[24];
        char name_last[24];
        char query[QUERY_LENGTH + 1];
        char yard_reference[16];

        FILE *fp;
        char *query_format;
        int c;
        int i;

        if ((query_format = malloc((QUERY_LENGTH + 1) * sizeof(char))) == NULL) {
                printf("Failed to allocate space for query.\n");
                exit(EXIT_FAILURE);
        }

        if ((fp = fopen(QUERY_FILENAME, "r")) == NULL) {
                printf("Can't open %s for reading.\n", QUERY_FILENAME);
                exit(EXIT_FAILURE);
        }

        for (i = 0; (c = getc(fp)) != EOF && i < QUERY_LENGTH; i++) {
                query_format[i] = c;
        }
        query_format[i] = '\0';
        fclose(fp);

        /* Need error handling */
        rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

        /* Need error handling */
        rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                           (SQLPOINTER) SQL_OV_ODBC3, 0);

        /* Need error handling */
        rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

        /* Need error handling */
        rc = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, NULL, 0, NULL,
                              SQL_DRIVER_COMPLETE);

        if (SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Connected!\n");
        }

        rc = SQLSetConnectAttr(dbc, SQL_ATTR_ACCESS_MODE,
                               (SQLPOINTER) SQL_MODE_READ_ONLY, SQL_NTS);
        if (SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Set to read-only mode.\n");
        }

        /* Need error handling */
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

        if (argc > 1) {
                sprintf(query, query_format, argv[1]);
        } else {
                sprintf(query, query_format, "%");
        }
        free(query_format);

        rc = SQLExecDirect(stmt, (SQLCHAR*)query, SQL_NTS);
        if (SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Executed query!\n\n");
        } else {
                fprintf(stderr, "Failed to execute query.\n");
        }

        rc = SQLNumResultCols(stmt, &n_cols);
        if (n_cols) {
                rc = SQLBindCol(stmt, 1, SQL_C_CHAR, yard_reference,
                                sizeof(yard_reference), &indicator[0]);
                rc = SQLBindCol(stmt, 2, SQL_C_LONG, &quantity,
                                0, &indicator[1]);
                rc = SQLBindCol(stmt, 3, SQL_C_CHAR, name,
                                sizeof(name), &indicator[2]);
                rc = SQLBindCol(stmt, 4, SQL_C_CHAR, size,
                                sizeof(size), &indicator[3]);
                rc = SQLBindCol(stmt, 5, SQL_C_DOUBLE, &usable_area,
                                0, &indicator[4]);
                rc = SQLBindCol(stmt, 6, SQL_C_CHAR, tracking_numbers,
                                sizeof(tracking_numbers), &indicator[5]);
                rc = SQLBindCol(stmt, 7, SQL_C_SHORT, &allocated,
                                0, &indicator[6]);
                rc = SQLBindCol(stmt, 8, SQL_C_CHAR, mill_cert,
                                sizeof(mill_cert), &indicator[7]);
                rc = SQLBindCol(stmt, 9, SQL_C_CHAR, classification,
                                sizeof(classification), &indicator[8]);

                while (SQL_SUCCEEDED(SQLFetch(stmt))) {
                        if (allocated == 0 && allocated_last == 1) {
                                /*printf("Free...\n");*/
                                allocated_last = allocated;
                        } else if (allocated == 1 && allocated_last == 0) {
                                printf("\nFreeable?...\n");
                                allocated_last = allocated;

                                /* Clear name_last so that it prints name */
                                sprintf(name_last, " ");
                        }
                        if (strcmp(name, name_last) != 0) {
                                printf("%s\n", name);
                                sprintf(name_last, name);
                        }

                        if (indicator[0] == SQL_NULL_DATA) {
                                sprintf(yard_reference, " ");
                        }
                        if (indicator[7] == SQL_NULL_DATA) {
                                sprintf(mill_cert, " ");
                        }

                        printf("  (%2d)%7s || %5.1f "
                               "|| %-9.9s || %-11.11s "
                               "|| %-22.22s || %s\n",
                               quantity, size, usable_area,
                               tracking_numbers, yard_reference,
                               mill_cert, classification);
                }
        } else {
                fprintf(stderr, "No results.\n");
        }


        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLDisconnect(dbc);

        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);

        return 0;
}

