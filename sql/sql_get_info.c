#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main() {
	SQLHENV env;
	SQLHDBC dbc;
	SQLRETURN ret;
	SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	ret = SQLDriverConnect(dbc, NULL, connection_str, SQL_NTS, NULL, 0, NULL,
	                       SQL_DRIVER_COMPLETE);

	if (SQL_SUCCEEDED(ret)) {
		SQLCHAR dbms_name[256], dbms_ver[256];
		SQLUINTEGER getdata_support;
		SQLUSMALLINT max_concur_act;

		printf("Connected!\n");

		SQLGetInfo(dbc, SQL_DBMS_NAME, &dbms_name,
		           sizeof(dbms_name), NULL);
		SQLGetInfo(dbc, SQL_DBMS_VER, &dbms_ver,
		           sizeof(dbms_ver), NULL);
		SQLGetInfo(dbc, SQL_GETDATA_EXTENSIONS, &getdata_support,
		           0, 0);
		SQLGetInfo(dbc, SQL_MAX_CONCURRENT_ACTIVITIES, &max_concur_act,
		           0, 0);

		printf("DBMS Name: %s\n", dbms_name);
		printf("DBMS Version: %s\n", dbms_ver);

		if (max_concur_act == 0) {
			printf("SQL_MAX_CONCURRENT_ACTIVITIES - no limit or undefined.\n");
		} else {
			printf("SQL_MAX_CONCURRENT_ACTIVITIES = %u\n", max_concur_act);
		}

		if (getdata_support & SQL_GD_ANY_ORDER) {
			printf("SQLGetData - columns can be retrieved in any order.\n");
		} else {
			printf("SQLGetData - columns must be retrieved in order.\n");
		}

		if (getdata_support & SQL_GD_ANY_COLUMN) {
			printf("SQLGetData - can retrieve columns before last bound one.\n");
		} else {
			printf("SQLGetData - columns must be retrieved after last bound one.\n");
		}

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

