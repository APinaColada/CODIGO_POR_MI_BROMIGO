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

Result_t *createResult(Tuple_t *tuple) {
    Result_t *result = malloc(sizeof(Result_t));
    result -> tuple = tuple;
    result -> next = NULL;
    return result;
}

void destroy_result(Result_t *result) {
    free(result);
}

void print_tuple(Tuple_t *tuple) {
    int n = tuple -> n_attrs;
    Attr_t *attr = tuple -> attr;
    for (int i = 0; i < n; i++) {
        printf("%s    ", attr -> name );
    }
    printf("\n");
}

void print_result(Result_t *res) {
    while (res -> next != NULL) {
        print_tuple(res -> tuple);
    }
}

Result_t *db_lookup(Database_t *db, const char *tuple, const char *tblname) {
    if (db -> tbl == NULL) {
        printf("Error 1: Null Database\n");
        return NULL;
    }
    Table_t *tbl = db -> tbl;
    printf("%s\n", tbl->name);
    
    while (tbl != NULL) {
        if (strcmp(tblname, tbl -> name) == 0) {
            break;
        } else {
            tbl = tbl -> next;
        }
    }
    
    int i = 0;
    char key[50] = "";
    int key_val = 0;
    int index;
    
    while (tuple[i] != '|' || tuple[i] == '\0') {
        key[i] = tuple[i];
        key_val += (int) tuple[i];
        i++;
    }
    
    printf("Key: %s\n", key);
    printf("Key Value: %d\n", key_val);
    if (strcmp(key, "*") == 0) {
        //This means that the key hasn't been given so we need to brute force search the table
    } else {
        index = key_val % 109;
        printf("Hash from: %d\n", index);
        if (tbl -> main[index] == NULL) {
            printf("Error: Tuple not found\n");
        } else {
            Tuple_t *tuple_s = tbl -> main[index];
            printf("Tuple attributes: %d\n", tuple_s -> n_attrs);
            printf("Shiiiit\n");
            //There could be a problem with this tuple above
            bool equals = equals_ts(tuple_s, strdup(tuple));
            if (equals == true) {
                return createResult(tuple_s);
            } else {
                printf("Error: Tuple not Equal\n");
            }
        }
    }
    
    return NULL;
}


//Checks to see if the tuples are equal
bool equals_tt(Tuple_t *tuple1, Tuple_t *tuple2) {
    Attr_t *attr1 = tuple1 -> attr;
    Attr_t *attr2 = tuple2 -> attr;
    if (tuple1 -> n_attrs == tuple2 -> n_attrs) {
        int n_attr = tuple1 -> n_attrs;
        for (int i = 1; i <= n_attr; i++) {
            if (attr1 -> name != attr2 -> name) {
                return false;
            }
            attr1 = attr1 -> next;
            attr2 = attr2 -> next;
        }
    } else {
        return false;
    }
    return true;
}


bool equals_ts(Tuple_t *tuple1, char *tuple2) {
    Attr_t *attr1 = tuple1 -> attr;
    //checks to see the size of the tuple string
    int i = 0;
    int n_attrs = 1;
    while (tuple2[i] != '\0') {
        if (tuple2[i] == '|') {
            n_attrs +=1;
        }
        i++;
    }
    if (tuple1 -> n_attrs == n_attrs) {
        i = 0;
        int j = 0;
        char* key = "";
        for (int i = 1; i <= n_attrs; i++) {
            while (tuple2[i] != '|' || tuple2[i] != '\0') {
                j = 0;
                key[j] = tuple2[i];
                i++;
                j++;
            }
            if (attr1 -> name != key) {
                return false;
            }
            key = "";
        }
    } else {
        return false;
    }
    return true;
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
