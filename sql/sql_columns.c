#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
        SQLHENV env;
        SQLHDBC dbc;
        SQLHSTMT stmt;
        SQLRETURN rc;
        SQLSMALLINT columns;
        SQLCHAR buf[64][64];
        SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";

        /* Might want error handling... */
        rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

        /* Might want error handling... */
        rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                           (SQLPOINTER*) SQL_OV_ODBC3, 0);

        /* Might want error handling... */
        rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

        rc = SQLSetConnectAttr(dbc, SQL_ATTR_ACCESS_MODE, 
                               (SQLPOINTER) SQL_MODE_READ_ONLY, SQL_NTS);
        if (SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Set to read only mode.\n");
        }

        rc = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, NULL, 0, NULL,
                              SQL_DRIVER_COMPLETE);
        if (SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Connected!\n\n");

                /* Might want error handling... */
                SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

                /*
                rc = SQLColumns(stmt, 
                                NULL, 0,
                                NULL, 0,
                                (SQLCHAR*)"ItemInventoryShape", SQL_NTS,
                                NULL, 0);
                */
                rc = SQLColumns(stmt, 
                                NULL, 0,
                                NULL, 0,
                                NULL, 0,
                                (SQLCHAR*)"ItemInventoryShapeID", SQL_NTS);

                SQLNumResultCols(stmt, &columns);
                SQLBindCol(stmt, 3, SQL_C_CHAR, buf[0], sizeof(buf[0]), NULL);
                SQLBindCol(stmt, 4, SQL_C_CHAR, buf[1], sizeof(buf[1]), NULL);
                SQLBindCol(stmt, 6, SQL_C_CHAR, buf[2], sizeof(buf[2]), NULL);

                while (SQL_SUCCEEDED(SQLFetch(stmt))) {
                        printf("%s - %s (%s)\n", buf[0], buf[1], buf[2]);
                }

                SQLFreeHandle(SQL_HANDLE_STMT, stmt);
                SQLDisconnect(dbc);
        }

        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);

        return 0;
}

