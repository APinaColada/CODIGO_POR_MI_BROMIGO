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
        tuple->n_attrs += 1;
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

Attr_t *attr_clone(Attr_t *attr) {
    Attr_t *clone = calloc(1, sizeof(Attr_t));
    clone->name = strdup(attr->name);
    clone->value = strdup(attr->value);
    return clone;
}

Tuple_t *tuple_clone(Tuple_t *tuple) {
    Tuple_t *clone = calloc(1, sizeof(Tuple_t));
    clone->n_attrs = tuple->n_attrs;
    Attr_t *attr_Clone = clone->attr;
    for (Attr_t *attr = tuple->attr; attr != NULL; attr = attr->next) {
        if (attr_Clone == NULL) {
            attr_Clone = attr_clone(attr);
            clone->attr = attr_Clone;
        } else {
            attr_Clone->next = attr_clone(attr);
            attr_Clone = attr_Clone->next;
        }
    }
    return clone;
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

// pass in query followed by the tuple
bool tuple_match(Tuple_t *query, Tuple_t *tuple) {
    Attr_t *q_attr = query->attr;
    Attr_t *t_attr = tuple->attr;
    if (query->n_attrs != tuple->n_attrs) {
        return false;
    }
    while (q_attr != NULL) {
        if (STREQ(q_attr->value, "*")) {

        } else {
            if (!STREQ(q_attr->value, t_attr->value)) {
                return false;
            }
        }
        q_attr = q_attr->next;
        t_attr = t_attr->next;
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
    printf("\n");
}

Table_t *db_gettable(Database_t *db, const char *name) {
    for (Table_t *tbl = db->tbl; tbl != NULL; tbl = tbl->next) {
        if (STREQ(tbl->name, name))
            return tbl;
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

bool db_insert(Database_t *db, const char *name, const char *row) {
    Table_t *tbl = db_gettable(db, name);
    Table_t *lookup = db_lookup(tbl, row);
    if (lookup != NULL) {
        for (int i = 0; i < HASHSIZE; i++) {
            if (lookup->ht[i] != NULL) {
                table_destroy(lookup);
                return false;
            }
        }
        table_destroy(lookup);
    }
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

void print_tuple(Tuple_t *tuple) {
    int n = tuple->n_attrs;
    Attr_t *attr = tuple->attr;
    for (int i = 0; i < n; i++) {
        printf("%s    ", attr->name);
    }
    printf("\n");
}

// Assuming primary key can be a wild-card, forced to iterate over all rows
void db_delete(Database_t *db, const char *table_name, const char *where) {
    Table_t *tbl = db_gettable(db, table_name);
    Tuple_t *query = tuple_create(tbl->schema, where);
    Tuple_t *prev, *next;
    for (int i = 0; i < HASHSIZE; i++) {
        prev = NULL;
        for (Tuple_t *tp = tbl->ht[i]; tp; tp = next) {
            if (tuple_match(query, tp)) {
                if (prev == NULL) {
                    tbl->ht[i] = tp->next;
                    next = tp->next;
                    tuple_destroy(tp);

                } else {
                    prev->next = tp->next;
                    next = tp->next;
                    tuple_destroy(tp);
                }
            } else {
                prev = tp;
                next = tp->next;
            }
        }
    }
    tuple_destroy(query);
}

Table_t *db_lookup(Table_t *tbl, const char *where) {
    Table_t *result = calloc(1, sizeof(Table_t));
    result->name = strdup(tbl->name);
    result->schema = strdup(tbl->schema);
    Tuple_t *query = tuple_create(tbl->schema, where);

    for (int i = 0; i < HASHSIZE; i++) {
        for (Tuple_t *tp = tbl->ht[i]; tp; tp = tp->next) {
            if (tuple_match(query, tp)) {
                if (result->ht[i] == NULL) {
                    result->ht[i] = tuple_clone(tp);
                } else {
                    Tuple_t *tail = result->ht[i];
                    while (tail->next != NULL) {
                        tail = tail->next;
                    }
                    tail->next = tuple_clone(tp);
                }
            }
        }
    }
    tuple_destroy(query);
    return result;
}

Table_t *db_select(Table_t *tbl, const char *cond) {
    return db_lookup(tbl, cond);
}

Tuple_t *project(Table_t *tbl, Tuple_t *tuple, char *attrs) {
    if (attrs == NULL) {
        return tuple_clone(tuple);
    } else {
        Tuple_t *project = tuple_create(tbl->schema, attrs);
        Tuple_t *clone = calloc(1, sizeof(Tuple_t));
        clone->n_attrs = tuple->n_attrs;
        Attr_t *attr_Clone = clone->attr;
        Attr_t *proj = project->attr;
        for (Attr_t *attr = tuple->attr; attr != NULL; attr = attr->next) {
            if (STREQ(proj->value, "*")) {
                proj = proj->next;
            } else {
                if (attr_Clone == NULL) {
                    attr_Clone = attr_clone(attr);
                    clone->attr = attr_Clone;
                } else {
                    attr_Clone->next = attr_clone(attr);
                    attr_Clone = attr_Clone->next;
                }
                proj = proj->next;
            }
        }
        tuple_destroy(project);
        return clone;
    }
}

Table_t *db_project(Table_t *tbl, char *attrs) {
    Table_t *result = calloc(1, sizeof(Table_t));
    result->name = strdup(tbl->name);
    result->schema = strdup(attrs);
    for (int i = 0; i < HASHSIZE; i++) {
        for (Tuple_t *tp = tbl->ht[i]; tp; tp = tp->next) {
            if (result->ht[i] == NULL) {
                result->ht[i] = project(tbl, tp, attrs);
            } else {
                Tuple_t *tail = result->ht[i];
                while (tail->next != NULL) {
                    tail = tail->next;
                }
                tail->next = project(tbl, tp, attrs);
            }
        }
    }
    return result;
}

Attr_t *attr_append(Attr_t *list, Attr_t *attr) {
    if (list == NULL) return attr;
    Attr_t *tail = list;
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = attr;
    return list;
}

Tuple_t *tuple_append(Tuple_t *list, Tuple_t *tuple) {
    if (list == NULL) return tuple;
    Tuple_t *tail = list;
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = tuple;
    return list;
}

Attr_t *attr_find(Tuple_t *tuple, const char *name) {
    for (Attr_t *ap = tuple->attr; ap != NULL; ap = ap->next)
        if (STREQ(ap->name, name))
            return ap;
    return NULL;
}

Tuple_t *tuple_intersection(Tuple_t *t1, Tuple_t *t2) {
    Tuple_t *I = calloc(1, sizeof(Tuple_t));
    for (Attr_t *a = t1->attr; a; a = a->next) {
        if (attr_find(t2, a->name)) {
            I->attr = attr_append(I->attr, attr_clone(a));
            I->n_attrs++;
        }
    }
    return I;
}

bool attr_equal(Attr_t *a, Attr_t *b) {
    if (strcmp(a->name, b->name) != 0) return false;
    return STREQ(a->value, b->value);
}

Table_t *db_join(Table_t *R, Table_t *S, const char *name, const char *schema) {
    Table_t *join = calloc(1, sizeof(Table_t));
    join->schema = strdup(schema);
    join->name = strdup(name);
    Tuple_t *t1 = tuple_create(R->schema, "*|*|*|*|*");
    Tuple_t *t2 = tuple_create(S->schema, "*|*|*|*|*");
    Tuple_t *I = tuple_intersection(t1, t2);

    for (int i = 0; i < HASHSIZE; i++) {
        for (Tuple_t *r = R->ht[i]; r; r = r->next) {
            for (int j = 0; j < HASHSIZE; j++) {
                for (Tuple_t *s = S->ht[j]; s; s = s->next) {
                    Attr_t *ra = attr_find(r, I->attr->name);
                    Attr_t *sa = attr_find(s, I->attr->name);
                    if (attr_equal(ra, sa)) {
                        Tuple_t *row = tuple_clone(r);
                        for (Attr_t *a = sa->next; a; a = a->next) {
                            row->attr = attr_append(row->attr, attr_clone(a));
                            row->n_attrs++;
                        }
                        join->ht[i] = tuple_append(join->ht[i], row);
                    }
                }
            }
        }
    }
    tuple_destroy(I);
    return join;
}


void write_to_file(Database_t *db) {
    FILE *file = fopen("Database_Data.txt", "w");
    Table_t *tb = db->tbl;
    while (tb != NULL) {
        fprintf(file, "*\n");
        int n = fprintf(file, "%s\n", tb->schema);
        fprintf(file, "%s\n", tb->name);
        for (int i = 0; i < n; i++)
            fprintf(file, "=");
        fprintf(file, "\n");
        for (int i = 0; i < HASHSIZE; i++) {
            for (Tuple_t *tpl = tb->ht[i]; tpl != NULL; tpl = tpl->next)
                tuple_print(file, tpl);
        }
        tb = tb->next;
    }

    fclose(file);
}

Database_t *read_from_file() {
    // open the file for reading and create the new Database
    FILE *file = fopen("Database_Data.txt", "r");
    // make sure the file opened properly
    if (NULL == file) {
        fprintf(stderr, "Cannot open file: 'Database_Data.txt'\n");
        return NULL;
    }

    Database_t *db = db_create("Registrar");

    // set up the buffer to read the line into. Don't worry too much
    // if some of the lines are longer than 80 characters - the buffer
    // will be made bigger if need be.
    size_t buffer_size = 80;
    char *buffer = malloc(buffer_size * sizeof(char));
    // first and second line will hold the shema and the name of the table
    char *schema = NULL;
    char *table_name = NULL;

    // read each line and print it to the screen
    // int line_number = 0;
    while (-1 != getline(&buffer, &buffer_size, file)) {
        if (buffer[0] == '*') {
            if (schema != NULL)
                free(schema);
            if (table_name != NULL)
                free(table_name);
            getline(&buffer, &buffer_size, file);
            schema = strdup(buffer);
            schema[strlen(buffer) - 1] = '\0';
            getline(&buffer, &buffer_size, file);
            table_name = strdup(buffer);
            table_name[strlen(buffer) - 1] = '\0';
            db_addtable(db, table_name, schema);
        } else {
            if (buffer[0] == '=') {
                // do nothing and go onto the next line
            } else {
                buffer[strlen(buffer) - 1] = '\0';
                db_insert(db, table_name, buffer);
            }
        }
    }
    fflush(stdout);

    fclose(file);
    free(buffer);
    free(schema);
    free(table_name);
    return db;
}

