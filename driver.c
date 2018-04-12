#include "database.h"

int main() {
    Database_t *db = db_create("Registrar");
    char *input= malloc(sizeof(char*));
    setbuf(stdout, NULL);
    
    printf("Hit enter to start the walkthrough of Alejandro and Tyler's Database\n");
    scanf("%c", input);
    //adding tables from 8.1 and 8.2 in Foundations of Computer Science by Aho and Ullman
    db_addtable(db, "CSG", "Course|StudentID|Grade");
    db_addtable(db, "SNAP", "StudentID|Name|Address|Phone");
    db_addtable(db, "CP", "Course|Prerequisite");
    db_addtable(db, "CDH", "Course|Day|Hour");
    db_addtable(db, "CR", "Course|Room");
    
    //inserting CSG data
    db_insert(db, "CSG", "CS101|12345|A");
    db_insert(db, "CSG", "CS101|67890|B");
    db_insert(db, "CSG", "EE200|12345|C");
    db_insert(db, "CSG", "EE200|22222|B+");
    db_insert(db, "CSG", "CS101|33333|A-");
    db_insert(db, "CSG", "PH100|67890|C+");
    
    //inserting SNAP data
    db_insert(db, "SNAP", "12345|C. Brown|12 Apple St.|555-1234");
    db_insert(db, "SNAP", "67890|L. Van Pelt|34 Pear Ave.|555-5678");
    db_insert(db, "SNAP", "22222|P. Patty|56 Grape Blvd.|555-9999");
    
    //inserting CP data
    db_insert(db, "CP", "CS101|CS100");
    db_insert(db, "CP", "EE200|EE005");
    db_insert(db, "CP", "EE200|CS100");
    db_insert(db, "CP", "CS120|CS101");
    db_insert(db, "CP", "CS121|CS120");
    db_insert(db, "CP", "CS205|CS101");
    db_insert(db, "CP", "CS206|CS121");
    db_insert(db, "CP", "CS206|CS205");
    
    //inserting CDH data
    db_insert(db, "CDH", "CS101|M|9AM");
    db_insert(db, "CDH", "CS101|W|9AM");
    db_insert(db, "CDH", "CS101|F|9AM");
    db_insert(db, "CDH", "EE200|Tu|10AM");
    db_insert(db, "CDH", "EE200|W|1PM");
    db_insert(db, "CDH", "EE200|Th|10AM");
    
    //inserting CR data
    db_insert(db, "CR", "CS101|Turing Aud");
    db_insert(db, "CR", "EE200|25 Ohm Hall");
    db_insert(db, "CR", "PH100|Newton Lab");
    
    Table_t *CSG = db_gettable(db, "CSG");
    Table_t *SNAP = db_gettable(db, "SNAP");
    Table_t *CP = db_gettable(db, "CP");
    Table_t *CDH = db_gettable(db, "CDH");
    Table_t *CR = db_gettable(db, "CR");
    
    printf("To prove that we can create tables and then insert data into them, we will print every table from FOCS Figure 8.1 and 8.2 along with their correct data entries. If you look in the code, you can see we insert data using 'db_insert(database, general schema, data entry)'\n");
    printf("\n");
    
    printf("Hit enter to see the Course-StudentID-Grade Schema\n");
    scanf("%c", input);
    table_print(CSG);
    printf("\n");
    printf("Hit enter to see the StudentID-Name-Address-Phone Schema\n");
    scanf("%c", input);
    table_print(SNAP);
    printf("\n");
    printf("Hit enter to see the Course-Prerequisite Schema\n");
    scanf("%c", input);
    table_print(CP);
    printf("\n");
    printf("Hit enter to see the Course-Day-Hour Schema\n");
    scanf("%c", input);
    table_print(CDH);
    printf("\n");
    printf("Hit enter to see the Course-Room Schema\n");
    scanf("%c", input);
    table_print(CR);
    printf("\n");
    
    printf("Example 8.2: \n");
    printf("    a) lookup(('CS101', 12345, *), Course-StudentID-Grade)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    Table_t *searched_table = db_lookup(db_gettable(db, "CSG"),  "CS101|12345|*");
    table_print(searched_table);
    // table_destroy(searched_table);
    printf("\n");
    
    printf("    b) lookup(('CS205', 'CS120'), Course-Prerequisite)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    searched_table = db_lookup(db_gettable(db, "CP"), "CS205|CS120");
    table_print(searched_table);
    // table_destroy(searched_table);
    printf("\n");
    
    printf("    c) delete(('CS101', '*'), Course-Room)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    db_delete(db, "CR", "CS101|*");
    CR = db_gettable(db, "CR");
    table_print(CR);
    printf("\n");
    
    printf("    d) insert(('CS205', 'CS120'), Course-Prerequisite)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    printf("Before: \n");
    table_print(CP);
    printf("After: \n");
    db_insert(db, "CP", "CS205|CS120");
    CP = db_gettable(db, "CP");
    table_print(CP);
    printf("\n");
    
    //There is a problem with this one because it adds even though there is already a tuple equal to it so it should be an easy fix
    printf("    e) insert(('CS205', 'CS101'), Course-Prerequisite)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    printf("Before: \n");
    table_print(CP);
    printf("After: \n");
    db_insert(db, "CP", "CS205|CS101");
    CP = db_gettable(db, "CP");
    table_print(CP);
    printf("\n");
    
    write_to_file(db);
    printf("We will now write the data in the database to a file named 'Database_data.txt'. Find the file and make sure the data has been copied before proceeding. Once you hit enter, the data in the data base will be destroyed forever. \n");
    scanf("%c", input);
    db_destroy(db);
    
    printf("If you go to line 133 in our code, you can see that the database has officially been destroyed. If you want to restore the database, hit enter on your keyboard\n");
    scanf("%c", input);
    Database_t *new_db = read_from_file();
    
    printf("To prove that the new database works, we will run it through a few lookup, insert, and delete functions. The first function is to lookup all the courses that are held on a Wednesday. Hit enter to see\n");
    scanf("%c", input);
    
    searched_table = db_lookup(db_gettable(new_db, "CDH"), "*|W|*");
    table_print(searched_table);
    // table_destroy(searched_table);
    
    printf("We will also add back the deleted CS101 class from the Course-Room Schema. Hit Enter to see the new table\n");
    scanf("%c", input);
    
    db_insert(new_db, "CR", "CS101|Turing Aud");
    CR = db_gettable(new_db, "CR");
    table_print(CR);
    // table_destroy(CR);
    
    printf("********\n");
    printf("PART 2 *\n");
    printf("********\n");
    
    printf("We will now perform the 2 questions for Part 2 of the project\n");
    printf("1) What grade did P. Patty get in EE200? \n");
    printf("Hit Enter to see the Result: \n");
    scanf("%c", input);

    Table_t *SN = db_project(db_gettable(new_db, "SNAP"), "StudentID|Name|*|*", "StudentID|Name");
    Table_t *CSGN = db_join(db_gettable(new_db, "CSG"), SN, "CSGN", "Course|StudentID|Grade|Name");
    Table_t *EE200 = db_select(CSGN, "EE200|*|*|P. Patty");
    Table_t *Grade = db_project(EE200, "*|*|Grade|*", "Grade");
    table_print(Grade);    
    // table_destroy(SN);
    // table_destroy(EE200);
    // table_destroy(Grade);
    
    printf("2) Where is C. Brown at 9 AM on Monday? \n");
    printf("Hit Enter to see the Result: \n");
    scanf("%c", input);

    Table_t *CSGNR = db_join(CSGN, db_gettable(new_db, "CR"), "CSGNR", "Course|StudentID|Grade|Name|Room");
    Table_t *CSGNRDH = db_join(CSGNR, db_gettable(new_db, "CDH"), "CSGNRDH", "Course|StudentID|Grade|Name|Room|Day|Hour");
    Table_t *NRDH = db_select(CSGNRDH, "*|*|*|C. Brown|*|M|9AM");
    Table_t *Room = db_project(NRDH, "*|*|*|*|Room|*|*", "Room");
    table_print(Room);
    // table_destroy(CSGNR);
    // table_destroy(CSGNRDH);
    // table_destroy(NRDH);
    // table_destroy(Room);
    // table_destroy(CSGN);

    printf("********\n");
    printf("PART 3 *\n");
    printf("********\n");
    
    printf("We will no perform the examples in Section 8.8 of FOCS: \n");
    printf("8.12) Result");
    printf("Type Enter to see result: ");
    scanf("%c", input);
    
    CSG = db_select(db_gettable(new_db, "CSG"), "CS101|*|*");
    table_print(CSG);
    // table_destroy(CSG);
    
    printf("8.13) Result");
    printf("Type Enter to see result: ");
    scanf("%c", input);
    
    CSG = db_select(db_gettable(new_db, "CSG"),  "CS101|*|*");
    Table_t *STUDENT_ID = db_project(CSG, "*|StudentID|*", "StudentID");
    table_print(STUDENT_ID);
    // table_destroy(CSG);
    // table_destroy(STUDENT_ID);
    
    printf("8.14) Result");
    printf("Type Enter to see result: ");
    scanf("%c", input);
    
    Table_t *CDHR_ = db_join(db_gettable(new_db, "CDH"), db_gettable(new_db, "CR"), "CDHR", "Course|Day|Hour|Room");
    table_print(CDHR_);
    
    printf("8.15) Result");
    printf("Type Enter to see result: ");
    scanf("%c", input);
    
    //Join Function will go here
    Table_t *Turing = db_select(CDHR_, "*|*|*|Turing Aud");
    Table_t *DH = db_project(Turing, "*|Day|Hour|*", "Day|Hour"); 
    table_print(DH);
    // table_destroy(DH);
    // table_destroy(Turing);
    // table_destroy(CDHR_);
    
    db_destroy(new_db);
    free(input);
}
