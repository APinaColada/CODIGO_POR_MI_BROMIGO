#include "database.c"

int main() {
    
    Database_t *db = db_create("Main");
    printf("Hey\n");
    db_addtable(db, "CSG", "CSG");
    printf("Hey\n");
    
}
