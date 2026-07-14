#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main() {
	SQLHENV env;
	SQLHDBC dbc;
	SQLRETURN ret;
	SQLCHAR out_str[1024];
	SQLSMALLINT out_str_len;
	SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	ret = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, out_str,
	                       sizeof(out_str), &out_str_len,
	                       SQL_DRIVER_COMPLETE);

	if (SQL_SUCCEEDED(ret)) {
		printf("Connected!\n");
		printf("Returned connection string was:\n\t%s\n", out_str);

		if (ret == SQL_SUCCESS_WITH_INFO) {
			printf("Driver reported diagnostics...\n");
		}

		SQLDisconnect(dbc);
	} else {
		fprintf(stderr, "Failed to connect.\n");
	}

	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	SQLFreeHandle(SQL_HANDLE_ENV, env);

	return 0;
}

