#include "database.h"

Database_t *db_create(char *name) {
    struct Database_s *db = malloc(sizeof(struct Database_s));
	db -> name       = name;
	db -> tbl        = NULL;
	return db;
}


void db_addtable(Database_t *db, char *name, char *schema){
	struct Table_s *tbl  = malloc(sizeof(struct Table_s));
	tbl -> name   = name;
	tbl -> schema = schema;
	tbl -> next   = NULL;
	tbl -> size   = 0;
	
	//traverses the table list until it finds an opening to place the new table in
	Table_t *next = db -> tbl;
	if (next == NULL) {
		//if there is nothing in the database
		db -> tbl = tbl;
	} else {
		//if it needs to traverse the list
		while (next -> next != NULL) {
			next = next -> next;
		}
		next -> next = tbl;
	}
	
	int n_attr = 0;
	int i      = 0;
	while (schema[i] != '\0') {
		n_attr += 1;
        i++;
	}
	
	tbl -> secondary = malloc(sizeof(int) * (n_attr - 1));
    
    //creates the array of size 109 for each secondary index
    for (i = 0; i < (n_attr - 1); i++) {
        tbl -> secondary[i] = malloc(sizeof(int) * 109);
    }
}

/*
void db_destroy(Database_t *db);

Database_t *db_load(const char *name);
bool db_dump(Database_t *db);
bool db_insert(Database_t *db, const char *row, const char *tblname);
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

