#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
        RETCODE rc;
        SQLCHAR allocation[48];
        SQLCHAR classification[64];
        SQLCHAR connection_str[] = "DSN=STRUMIS;UID=strumis_user;PWD=Strumis_6;";
        SQLCHAR size[12];
        SQLCHAR tracking_numbers[10];
        SQLFLOAT usable_area;
        SQLHDBC dbc;
        SQLHENV env;
        SQLHSTMT stmt;
        SQLINTEGER quantity;
        SQLLEN indicator[9];
        SQLSMALLINT n_cols;
        char mill_cert[24];
        char name[24];
        char name_last[24];
        char yard_reference[16];

        char query[] = "SELECT\n\
        ii.YardReference,\n\
        ii.Quantity,\n\
        i.Name,\n\
        FORMAT(ii.Length / 25.4, 'F0') + 'x' + FORMAT(ii.Width / 25.4, 'F0') AS Size,\n\
        (((ii.Length / 25.4) - 15) * (ii.Width / 25.4) / 72) AS UsableArea,\n\
        iigp.TrackingNumbers,\n\
        iigp.Allocations,\n\
        iip.CEMark,\n\
        iigp.Classifications\n\
FROM dbo.ItemInventory ii\n\
JOIN dbo.Item i\n\
        ON i.ItemID = ii.ItemID\n\
LEFT JOIN dbo.ItemInventoryGroupProperty iigp\n\
        ON ii.ItemInventoryID = iigp.ItemInventoryID\n\
LEFT JOIN dbo.ItemInventoryProperty iip\n\
        ON ii.ItemInventoryPropertyID = iip.ItemInventoryPropertyID\n\
WHERE ii.FacilityID = 3  -- In Berlin\n\
        AND iigp.Allocations NOT LIKE ''\n\
        AND i.Name LIKE '%%PLT%%'\n\
        AND i.Name NOT LIKE '%%A588%%'\n\
        AND i.Name NOT LIKE '%%GA%%'\n\
        AND i.Name NOT LIKE '1/8\"%%'\n\
        AND i.Name NOT LIKE '3/16\"%%'\n\
        AND iigp.Classifications IN ('OK', 'Processed')\n\
ORDER BY\n\
        i.Name,\n\
        iigp.TrackingNumbers,\n\
        SUBSTRING(iigp.Allocations, 0, 13)";


        rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

        rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                           (SQLPOINTER) SQL_OV_ODBC3, 0);

        rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

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

        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

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
                rc = SQLBindCol(stmt, 7, SQL_C_CHAR, allocation,
                                sizeof(allocation), &indicator[6]);
                rc = SQLBindCol(stmt, 8, SQL_C_CHAR, mill_cert,
                                sizeof(mill_cert), &indicator[7]);
                rc = SQLBindCol(stmt, 9, SQL_C_CHAR, classification,
                                sizeof(classification), &indicator[8]);

                while (SQL_SUCCEEDED(SQLFetch(stmt))) {
                        if (indicator[0] == SQL_NULL_DATA) {
                                snprintf(yard_reference, 2, " ");
                        }
                        if (indicator[7] == SQL_NULL_DATA) {
                                snprintf(mill_cert, 2, " ");
                        }

                        if (strcmp(name, name_last) != 0) {
                                printf("%s\n", name);
                                snprintf(name_last, 24, name);
                        }

                        printf("  (%2d)%7s || %-9.9s || %11.11s || %.42s\n", quantity, size, tracking_numbers, yard_reference, allocation);
                }
        } else {
                fprintf(stderr, "No results.\n");
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLDisconnect(dbc);

        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);

        exit(EXIT_SUCCESS);
}

