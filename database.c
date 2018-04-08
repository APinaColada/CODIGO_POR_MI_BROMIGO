#include "database.h"

Database_t *db_create(char *name) {
    struct Database_s *db = malloc(sizeof(struct Database_s));
    db->name = name;
    db->tbl = NULL;
    return db;
}

void db_addtable(Database_t *db, char *name, char *schema) {
    struct Table_s *tbl = malloc(sizeof(struct Table_s));
    tbl->name = strdup(name);
    tbl->schema = strdup(schema);
    tbl->next = NULL;
    tbl->size = 0;

    // traverses the table list until it finds an opening to place the new table
    // in
    Table_t *tbl = db->tbl;
    if (tbl == NULL) {
        // if there is nothing in the database
        db->tbl = tbl;
    } else {
        // if it needs to traverse the list
        while (tbl->next != NULL) {
            tbl = tbl->next;
        }
        tbl->next = tbl;
    }

    /*int n_attrs = 0;
    int i = 0;
    while (schema[i] != '\0') {
        n_attrs += 1;
        i++;
    }

    tbl->secondary = malloc(sizeof(int) * (n_attrs - 1));

    // creates the array of size 109 for each secondary index
    for (i = 0; i < (n_attrs - 1); i++) {
        tbl->secondary[i] = malloc(sizeof(int) * 109);
    }*/
}

Tuple_t *db_create_tuple(const char *row, Table_t *tbl) {
    Tuple_t *tpl = malloc(sizeof(Tuple_t));
    tpl->next = NULL;
    char* first,second;
    char *rowcp = strdup(row);
    char *token = strtok_r(rowcp, "|", &first);
    char *schema = tbl->schema;
    char *schemacp = strdup(schema);
    char *sch = strtok_r(schemacp, "|", &second);
    while ((token != NULL) && (sch != NULL)) {
        Attr_t *attr = malloc(sizeof(struct Attr_s));
        attr->name = sch;
        attr->type = STRING; // figure out way to differentiate type later...
        attr->sval = token;
        attr->next = NULL;
        token = strtok_r(NULL, "|", &first);
        sch = strtok_r(NULL, "|", &second);
        tpl->n_attrs++;
        if (tpl->attr == NULL) {
            tpl->attr = attr;
        } else {
            Attr_t at = tpl->attr;
            while(at->next != NULL){
                at = at->next;
        }
            at->next = attr;
    }
    return tpl;
}
// key is first attribute
bool db_insert(Database_t *db, const char *row, const char *tblname) {

    Table_t *tbl = db->tbl;
    while (tbl != NULL) {
        if (strcmp(tbl->name, tblname)) {
            Tuple_t *tuple = db_create_tuple(row, tbl);
            next->main[0] = tuple; // for testing to see if it works
            return true;
        } else {
            tbl = tbl->next;
        }
    }
    return false;
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
