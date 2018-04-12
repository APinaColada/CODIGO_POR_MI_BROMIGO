#include "database.h"

int main() {
    Database_t *db = db_create("Registrar");
    char *input= malloc(sizeof(char*));
    setbuf(stdout, NULL);
    
    printf("Hit enter to start the walkthrough of Alejandro and Tyler's Database\n");
    scanf("%c", input);
    //adding tables from 8.1 and 8.2 in Foundations of Computer Science by Aho and Ullman
    db_addtable(db, "CSG", "Course|StudentId|Grade");
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
    Table_t *searched_table = db_lookup(db, "CS101|12345|*", "CSG");
    table_print(searched_table);
    printf("\n");
    
    printf("    b) lookup(('CS205', 'CS120'), Course-Prerequisite)\n");
    printf("    Hit Enter to see answer: \n");
    scanf("%c", input);
    searched_table = db_lookup(db, "CS205|CS120", "CP");
    table_print(searched_table);
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
    
    searched_table = db_lookup(new_db, "*|W|*", "CDH");
    table_print(searched_table);
    
    printf("We will also add back the deleted CS101 class from the Course-Room Schema. Hit Enter to see the new table\n");
    scanf("%c", input);
    
    db_insert(new_db, "CR", "CS101|Turing Aud");
    CR = db_gettable(new_db, "CR");
    table_print(CR);
    
    printf("********\n");
    printf("PART 2 *\n");
    printf("********\n");
    
    printf("We will now perform the 2 questions for Part 2 of the project\n");
    printf("1) What grade did StudentName get in CourseName? \n");
    printf("Hit Enter to see the Result: \n");
    scanf("%c", input);
    
    //place for answer
    
    printf("2) Where is StudentName at Time on Day? \n");
    printf("Hit Enter to see the Result: \n");
    scanf("%c", input);
    
    //place for answer
    
    printf("********\n");
    printf("PART 3 *\n");
    printf("********\n");
    
    printf("********\n");
    printf("Extra Credit *\n");
    printf("********\n");
    
    printf("We decided to go for extra credit in this project and create a generic database data structure. To prove this, we will make a database for MLB Baseball Player's Fortnite statistics. Our Schemas will be <Name-Team-Position>, <Name-WinRate-Rank>, < )
    db_destroy(new_db);
    free(input);
}
