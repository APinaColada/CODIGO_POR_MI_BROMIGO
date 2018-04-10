#include "database.c"

int main() {
    Database_t *db = db_create("Registrar");

    db_addtable(db, "CSG", "Course|StudentId|Grade");
    db_addtable(db, "CR", "Course|Room");

    db_insert(db, "CSG", "PH100|67890|C+");
    db_insert(db, "CSG", "EE200|12345|C");
    db_insert(db, "CSG", "EE200|22222|B+");
    db_insert(db, "CSG", "CS101|12345|A");
    db_insert(db, "CSG", "CS101|67890|B");
    db_insert(db, "CSG", "CS101|33333|A-");
    
    db_insert(db, "CR", "CS101|R130");
    db_insert(db, "CR", "CS101|R100");
    db_insert(db, "CR", "EE200|R100");
    
    Table_t *CSG = db_gettable(db, "CSG");
    Table_t *CR = db_gettable(db, "CR");
    
    table_print(CSG);
    table_print(CR);
    db_delete(db,"CR", "CS101|*");
    table_print(CR);
    Table_t *result = db_lookup(db, "*|*|T+", "CSG");
    printf("Result\n");
    table_print(result);
    
    db_destroy(db);
    
}
