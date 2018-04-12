#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Attr_s Attr_t;
struct Attr_s {
    Attr_t *next;
    char *name;
    char *value;
};

typedef struct Tuple_s Tuple_t;
struct Tuple_s {
    Tuple_t *next;
    int n_attrs;
    Attr_t *attr;
};

#define HASHSIZE 109
typedef struct Table_s Table_t;
struct Table_s {
    Table_t *next;
    char *name;
    char *schema;
    Tuple_t *ht[HASHSIZE];
};

typedef struct Database_s Database_t;
struct Database_s {
    char *name;
    Table_t *tbl;
};

Database_t *db_create(char *name);
void db_destroy(Database_t *db);
void db_addtable(Database_t *db, char *name, char *schema);
Database_t *db_load(const char *name);
bool db_dump(Database_t *db);
bool db_insert(Database_t *db, const char *table_name, const char *row);
void db_delete(Database_t *db, const char *table_name, const char *where);
Table_t *db_lookup(Table_t *tbl, const char *where);
Table_t *db_select(Table_t *tbl, const char *cond);
Table_t *db_gettable(Database_t *db, const char *table_name);
Table_t *db_project(Table_t *tbl, char *cols);
Table_t *db_join(Table_t *R, Table_t *S, const char *name, const char *schema);
void table_print(Table_t *tbl);

void table_destroy(Table_t *tbl);

void write_to_file(Database_t *db);

Database_t *read_from_file();
