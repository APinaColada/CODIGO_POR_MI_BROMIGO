#include "database.c"

int main() {
    Database_t *db = db_create("Registrar");

    db_addtable(db, "CSG", "Course|StudentId|Grade");

    db_insert(db, "CSG", "PH100|67890|C+");
    db_insert(db, "CSG", "EE200|12345|C");
    db_insert(db, "CSG", "EE200|22222|B+");
    db_insert(db, "CSG", "CS101|12345|A");
    db_insert(db, "CSG", "CS101|67890|B");
    db_insert(db, "CSG", "CS101|33333|A-");

    Table_t *CSG = db_gettable(db, "CSG");
    
    table_print(CSG);
    
    Tuple_t *result = db_lookup(db, "PH100|67890|C+", "CSG", "Course|StudentId|Grade");
    
    if (result != NULL) {
        printf("Target: ");
        tuple_print(stdout,result);
    }
    
    db_destroy(db);
    
}
