#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main() {
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	SQLSMALLINT columns;
	SQLCHAR buf[5][64];
	SQLLEN indicator[5];
	int i;
	SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	ret = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, NULL, 0, NULL,
	                       SQL_DRIVER_COMPLETE);

	if (!SQL_SUCCEEDED(ret)) {
		fprintf(stderr, "Failed to connect.\n");

		SQLFreeHandle(SQL_HANDLE_DBC, dbc);
		SQLFreeHandle(SQL_HANDLE_ENV, env);

		return 0;
	}

	printf("Connected!\n");

	if (ret == SQL_SUCCESS_WITH_INFO) {
		printf("Driver reported diagnostics...\n");
	}

	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	SQLTables(stmt,
		  NULL, 0,
		  NULL, 0,
		  NULL, 0,
		  (SQLCHAR *) "TABLE", SQL_NTS);
	SQLNumResultCols(stmt, &columns);

	for (i = 0; i < columns; i++) {
		SQLBindCol(stmt, i + 1, SQL_C_CHAR, buf[i], sizeof(buf[i]),
		           &indicator[i]);
	}

	while (SQL_SUCCEEDED(SQLFetch(stmt))) {
		for (i = 0; i < columns; i++) {
			if (indicator[i] == SQL_NULL_DATA) {
				printf("  Column %u : NULL\n", i);
			} else {
				printf("  Column %u : %s\n", i, buf[i]);
			}
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	SQLDisconnect(dbc);

	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	SQLFreeHandle(SQL_HANDLE_ENV, env);

	return 0;
}

