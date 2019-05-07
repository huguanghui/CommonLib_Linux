#include <stdio.h>

#include "sqlite3.h"

static const char *s_db_path = "/tmp/test.db";

int main(int argc, char const *argv[])
{
    sqlite3 *db = NULL;

    if (sqlite3_open_v2(s_db_path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | 
                SQLITE_OPEN_FULLMUTEX, NULL) == SQLITE_OK) {
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS kv(key PRIMARY KEY, val)", 0, 0, 0);
    }

    // 添加参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";
        char value[16] = "abc";

        if (sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO kv VALUES(?, ?);", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, value, sizeof(value), SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    // 获取参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";
        char value[16] = "abc";
        const char *data = NULL;

        if (sqlite3_prepare_v2(db, "SELECT val FROM kv WHERE key = ?;", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            int result = sqlite3_step(stmt);
            data = sqlite3_column_text(stmt, 0);
            if ((result == SQLITE_OK || result == SQLITE_ROW) && data != NULL)
            {
                printf("Result[%d] Data[%s]\n", result, data);
            }
            sqlite3_finalize(stmt);
        }        
    }

    // 修改参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";
        char value[16] = "abcdefg";

        if (sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO kv VALUES(?, ?);", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, value, sizeof(value), SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    // 获取参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";
        char value[16] = "abc";
        const char *data = NULL;

        if (sqlite3_prepare_v2(db, "SELECT val FROM kv WHERE key = ?;", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            int result = sqlite3_step(stmt);
            data = sqlite3_column_text(stmt, 0);
            if ((result == SQLITE_OK || result == SQLITE_ROW) && data != NULL)
            {
                printf("Result[%d] Data[%s]\n", result, data);
            }
            sqlite3_finalize(stmt);
        }        
    }

    // 删除参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";

        if (sqlite3_prepare_v2(db, "DELETE FROM kv WHERE key = ?;", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            int result = sqlite3_step(stmt);
            if (result == SQLITE_OK || result == SQLITE_ROW || result == SQLITE_DONE)
            {
                printf("DELETE OK![%s]\n", key);
            }
            printf("Result[%d]\n", result);
            sqlite3_finalize(stmt);
        }
    }
    // 获取参数
    {
        sqlite3_stmt *stmt = NULL;
        char key[16] = "net";
        char value[16] = "abc";
        const char *data = NULL;

        if (sqlite3_prepare_v2(db, "SELECT val FROM kv WHERE key = ?;", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, key, sizeof(key), SQLITE_STATIC);
            int result = sqlite3_step(stmt);
            data = sqlite3_column_text(stmt, 0);
            if ((result == SQLITE_OK || result == SQLITE_ROW) && data != NULL)
            {
                printf("Result[%d] Data[%s]\n", result, data);
            }
            else
            {
                printf("Get Key[%s] Data Failed! Result[%d]\n", key, result);
            }
            sqlite3_finalize(stmt);
        }        
    }

    sqlite3_close(db);

    return 0;
}
