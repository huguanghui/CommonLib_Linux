#ifndef __OP_DB_H__
#define __OP_DB_H__

#include "mongoose.h"

void *db_open(const char *db_path);

void db_close(void **db_handle);

enum {API_OP_GET, API_OP_PUT, API_OP_DELETE};

void db_op(struct mg_connection *nc, const struct http_message *hm,
            const struct mg_str *key, void *db, int op);

#endif /* op_db.h */