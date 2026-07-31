#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        RETCODE rc;
        SQLHENV env;
        SQLHDBC dbc;
        SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";
        SQLHSTMT stmt;

        SQLCHAR *sql = (SQLCHAR *)""
"SELECT DISTINCT "
"vun.CuttingPlanID, "
"nr.CreatedBy, "
"nr.CreatedOn, "
"nra.NestEngineTask, "
"nra.NestEngineWorkspace "
"FROM vw_userreport_nestingallocationitems AS vun "
"JOIN NestingResultAllocation AS nra ON nra.NestingResultAllocationID = vun.NestingResultAllocationID "
"JOIN NestingResult AS nr ON nr.NestResultID = nra.NestResultID "
"WHERE vun.CuttingPlanID = ? "
"AND nra.NestEngineTask IS NOT NULL "
"ORDER BY nr.CreatedOn ";

        SQLINTEGER cutting_plan;
        SQLLEN indicator[10];

        SQLCHAR created_by[50];
        SQL_TIMESTAMP_STRUCT created_on;
        SQLCHAR task[400];
        SQLCHAR workspace[400];

        int n_results = 0;

        if (argc == 2) {
                cutting_plan = strtol(argv[1], NULL, 10);
        } else {
                fprintf(stderr, "Enter cutting plan number: ");
                scanf("%d", &cutting_plan);
        }

        rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

        rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                           (SQLPOINTER) SQL_OV_ODBC3, 0);

        rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

        rc = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, NULL, 0, NULL,
                              SQL_DRIVER_COMPLETE);
        if (!SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Failed to connect!\n");
        }

        rc = SQLSetConnectAttr(dbc, SQL_ATTR_ACCESS_MODE,
                               (SQLPOINTER) SQL_MODE_READ_ONLY, SQL_NTS);

        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

        rc = SQLPrepare(stmt, sql, SQL_NTS);

        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
                         &cutting_plan, sizeof(cutting_plan), &indicator[0]);

        rc = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(rc)) {
                fprintf(stderr, "Execution failed.\n\n");
        }

        SQLBindCol(stmt, 2, SQL_C_CHAR,
                   created_by, sizeof(created_by), &indicator[1]);
        SQLBindCol(stmt, 3, SQL_C_TYPE_TIMESTAMP,
                   &created_on, sizeof(created_by), &indicator[2]);
        SQLBindCol(stmt, 4, SQL_C_CHAR,
                   task, sizeof(task), &indicator[3]);
        SQLBindCol(stmt, 5, SQL_C_CHAR,
                   workspace, sizeof(workspace), &indicator[4]);

        while (SQLFetch(stmt) == SQL_SUCCESS) {
                printf("\n(%d) Created by %s on %04d-%02d-%02d @ %02d:%02d\n",
                       cutting_plan, created_by,
                       created_on.year, created_on.month, created_on.day,
                       created_on.hour, created_on.minute);
                printf("Task: %s\n", task);
                printf("Workspace: %s\n", workspace);
                n_results++;
        }

        if (!n_results) {
                fprintf(stderr, "Nothing found.\n\n");
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLDisconnect(dbc);

        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);

        exit(EXIT_SUCCESS);
}

