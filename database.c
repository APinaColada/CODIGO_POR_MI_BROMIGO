#include "database.h"

#define STREQ(s1, s2) (strcmp((s1), (s2)) == 0)

static int tuple_hash(Tuple_t *tpl);

Database_t *db_create(char *name) {
    struct Database_s *db = calloc(1, sizeof(Database_t));
    db->name = strdup(name);
    return db;
}

void db_addtable(Database_t *db, char *name, char *schema) {
    Table_t *table = calloc(1, sizeof(Table_t));
    table->name = strdup(name);
    table->schema = strdup(schema);

    Table_t *tbl = db->tbl;
    if (tbl == NULL) {
        // if there is nothing in the database
        db->tbl = table;
    } else {
        // append new table
        while (tbl->next != NULL) {
            tbl = tbl->next;
        }
        tbl->next = table;
    }
}

Tuple_t *tuple_create(const char *schema, const char *row) {
    Tuple_t *tuple = calloc(1, sizeof(Tuple_t));
    char *schemacp = strdup(schema);
    char *schemabrkp;
    char *rowcp = strdup(row);
    char *rowbrkp;
    char *value = strtok_r(rowcp, "|", &rowbrkp);
    char *col = strtok_r(schemacp, "|", &schemabrkp);

    while ((value != NULL) && (col != NULL)) {
        Attr_t *attr = calloc(1, sizeof(Attr_t));
        attr->name = strdup(col);
        attr->value = strdup(value);

        value = strtok_r(NULL, "|", &rowbrkp);
        col = strtok_r(NULL, "|", &schemabrkp);

        tuple->n_attrs++;
        if (tuple->attr == NULL) {
            tuple->attr = attr;
        } else {
            Attr_t *ap = tuple->attr;
            while (ap->next != NULL) {
                ap = ap->next;
            }
            ap->next = attr;
        }
    }
    free(schemacp);
    free(rowcp);
    return tuple;
}

void tuple_destroy(Tuple_t *tuple) {
    if (tuple != NULL) {
    Attr_t *attr = tuple->attr;

    while (attr != NULL) {
        Attr_t *next = attr->next;
        free(attr->name);
        free(attr->value);
        free(attr);
        attr = next;
    }
    free(tuple);
    }
}

void table_destroy(Table_t *tbl) {
    if (tbl != NULL) {
       free(tbl->name);
       free(tbl->schema);
       for (int i = 0; i < HASHSIZE; i++) {
           Tuple_t *tpl = tbl->ht[i];
           while (tpl) {
               Tuple_t *next = tpl->next;
               tuple_destroy(tpl);
               tpl = next;
           }
       }
       free(tbl);
    }
}

void db_destroy(Database_t *db) {
    if (db != NULL) {
        Table_t *tbl = db->tbl;

        free(db->name);
        while (tbl) {
            Table_t *next = tbl->next;
            table_destroy(tbl);
            tbl = next;
        }
        free(db);
    }
}
    

bool db_insert(Database_t *db, const char *name, const char *row) {
    Table_t *tbl = db_gettable(db, name);

    if (tbl != NULL) {
        Tuple_t *tuple = tuple_create(tbl->schema, row);
        int h = tuple_hash(tuple);
        if (tbl->ht[h] == NULL) {
            tbl->ht[h] = tuple;
        } else {
            Tuple_t *tpl = tbl->ht[h];
            while (tpl->next != NULL)
                tpl = tpl->next;
            tpl->next = tuple;
        }
        return true;
    }
    return false;
}

void tuple_print(FILE *fp, Tuple_t *tuple) {
    char *sep = "";
    for (Attr_t *attr = tuple->attr; attr != NULL; attr = attr->next) {
        fprintf(fp, "%s%s", sep, attr->value);
        sep = "|";
    }
    fprintf(fp, "\n");
}

void table_print(Table_t *tbl) {
    int n = printf("%s\n", tbl->schema);
    for (int i = 0; i < n; i++)
        printf("=");
    printf("\n");
    for (int i = 0; i < HASHSIZE; i++) {
        for (Tuple_t *tpl = tbl->ht[i]; tpl != NULL; tpl = tpl->next)
            tuple_print(stdout, tpl);
    }
}

Table_t *db_gettable(Database_t *db, const char *name) {
    for (Table_t *tbl = db->tbl; tbl != NULL; tbl = tbl->next) {
        if (STREQ(tbl->name, name)) return tbl;
    }
    return NULL;
}

// hash on primary key - first attribute in tuple
// string hash from K&R Section 6.6 "Lookup Table"
static int tuple_hash(Tuple_t *tpl) {
    int h = 0;

    for (char *s = tpl->attr->value; *s != '\0'; s++)
        h = *s + 31 * h;
    return h % HASHSIZE;
}

/*
void db_destroy(Database_t *db);

Database_t *db_load(const char *name);
bool db_dump(Database_t *db);

void db_delete(Database_t *db, const char *filter, const char *tblname);
Result_t *db_lookup(Database_t *db, const char *tuple, const char *tblname);
Table_t *db_select(Table_t *tbl, const char *cond);
Table_t *db_gettable(Database_t *db, const char *name);
Table_t *db_project(Table_t *tbl, char *attrs);
Table_t *db_join(Table_t *R, Table_t *S);

void table_print(Table_t *tbl);
void table_destroy(Table_t *tbl);

void print_result(Result_t *res);

void result_destroy(Result_t *res);
*/
