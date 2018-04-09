#pragma once
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Attr_s Attr_t;
struct Attr_s {
    Attr_t *next;
    char *name;
    char *value;
};

typedef struct Tuple_s Tuple_t;
struct Tuple_s {
    Tuple_t *next;
    int n_attrs;
    Attr_t *attr;
};

#define HASHSIZE 109
typedef struct Table_s Table_t;
struct Table_s {
    Table_t *next;
    char *name;
    char *schema;
    Tuple_t *ht[HASHSIZE];
};

typedef struct Database_s Database_t;
struct Database_s {
    char *name;
    Table_t *tbl;
};

Database_t *db_create(char *name);
void db_destroy(Database_t *db);
void db_addtable(Database_t *db, char *name, char *schema);
Database_t *db_load(const char *name);
bool db_dump(Database_t *db);
bool db_insert(Database_t *db, const char *table_name, const char *row);
void db_delete(Database_t *db, const char *table_name, const char *where);
Tuple_t *db_lookup(Database_t *db, const char *table_name, const char *where, const char *schema);
Table_t *db_select(Table_t *tbl, const char *cond);
Table_t *db_gettable(Database_t *db, const char *table_name);
Table_t *db_project(Table_t *tbl, char *cols);
Table_t *db_join(Table_t *R, Table_t *S);

void table_print(Table_t *tbl);

void table_destroy(Table_t *tbl);

void print_tuple(Tuple_t *tuple);

bool equals_ts(Tuple_t *tuple1, char *tuple2);

bool equals_tt(Tuple_t *tuple1, Tuple_t *tuple2);

//returns the strings that make up the tuple in array form
char **return_atts(Tuple_t *tuple);

void destroy_atts(char** atts, int n);

/*
//Creates Course, Student ID, Grade Schema
//Key: Student ID
CSG *createCSG(char *course, int SID, char *grade) {
    CSG *CSGP = malloc(sizeof(CSG));
    CSGP->course = course;
    CSGP->SID = SID;
    CSGP->grade = grade;
    CSGP->next = NULL;

    return CSGP;
}

//Creates Student ID, Name, Address, and Phone Schema
//Key: Student ID
SNAP *createSNAP(int SID, char *name, char *address, char* phone) {
    SNAP *SNAPP = malloc(sizeof(SNAP));
    SNAPP->SID = SID;
    SNAPP->name = name;
    SNAPP->address = address;
    SNAPP->phone = phone;
    SNAPP->next = NULL;

    return SNAPP;
}

//Creates Course, Prerequisite Schema
//Key: Course
CP *createCP(char *C, char *P) {
    CP *CPP = malloc(sizeof(CP));
    CPP->course = C;
    CPP->prereq = P;
    CPP->next = NULL;

    return CPP;
}

//Creates Course, Day, Hour Schema
//Key: Course
CDH *createCDH(char *C, char *D, char *H) {
    CDH *CDHP = malloc(sizeof(CDH));
    CDHP->course = C;
    CDHP->day = D;
    CDHP->hour = H;
    CDHP->next = NULL;

    return CDHP;
}

//Creates Course, Room Schema
//Key: Course
CR *createCR(char *C, char *R) {
    CR *CRP = malloc(sizeof(CR));
    CRP->course = C;
    CRP->room = R;
    CRP->next = NULL;

    return CRP;
}



int equal_SNAP(SNAP* s1, SNAP* s2)
{
  if (s1 == NULL || s2 == NULL) return 0;
  if (!(strcmp(s1->SID,s2->SID) == 0) && !((strcmp(s1->SID,"*") == 0) || (strcmp(s2->SID,"*") == 0))) return 0;
  if (!(strcmp(s1->name,s2->name) == 0) && !((strcmp(s1->name,"*") == 0) || (strcmp(s2->name,"*") == 0))) return 0;
  if (!(strcmp(s1->address,s2->address) == 0) && !((strcmp(s1->address,"*") == 0) || (strcmp(s2->address,"*") == 0))) return 0;
  if (!(strcmp(s1->phone, s2->phone) == 0) && !((strcmp(s1->phone,"*") == 0) || (strcmp(s2->phone,"*") == 0))) return 0;
  return 1;//All are the same! Success
}

int equal_CSG(CSG* c1, CSG* c2)
{
  if (c1 == NULL || c2 == NULL) return 0;
  if (!(strcmp(c1->SID,c2->SID) == 0) && !((strcmp(c1->SID,"*") == 0) || (strcmp(c2->SID,"*") == 0))) return 0;
  if (!(strcmp(c1->course,c2->course) == 0) && !((strcmp(c1->course,"*") == 0) || (strcmp(c2->course,"*") == 0))) return 0;
  if (!(strcmp(c1->grade,c2->grade) == 0) && !((strcmp(c1->grade,"*") == 0) || (strcmp(c2->grade,"*") == 0))) return 0;

  return 1;//All are the same! Success
}

int equal_CP(CP* c1, CP* c2)
{
  if (c1 == NULL || c2 == NULL) return 0;
  if (!(strcmp(c1->course,c2->course) == 0) && !((strcmp(c1->course,"*") == 0) || (strcmp(c2->course,"*") == 0))) return 0;
  if (!(strcmp(c1->prereq,c2->prereq) == 0) && !((strcmp(c1->prereq,"*") == 0) || (strcmp(c2->prereq,"*") == 0))) return 0;
  return 1;//All are the same! Success
}

int equal_CDH(CDH* c1, CDH* c2)
{
  if (c1 == NULL || c2 == NULL) return 0;
  if (!(strcmp(c1->course,c2->course) == 0) && !((strcmp(c1->course,"*") == 0) || (strcmp(c2->course,"*") == 0))) return 0;
  if (!(strcmp(c1->day,c2->day) == 0) && !((strcmp(c1->day,"*") == 0) || (strcmp(c2->day,"*") == 0))) return 0;
  if (!(strcmp(c1->hour,c2->hour) == 0) && !((strcmp(c1->hour,"*") == 0) || (strcmp(c2->hour,"*") == 0))) return 0;
  return 1;//All are the same! Success
}

int equal_CR(CR* c1, CR* c2)
{
  if (c1 == NULL || c2 == NULL) return 0;
  if (!(strcmp(c1->course,c2->course) == 0) && !((strcmp(c1->course,"*") == 0) || (strcmp(c2->course,"*") == 0))) return 0;
  if (!(strcmp(c1->room,c2->room) == 0) && !((strcmp(c1->room,"*") == 0) || (strcmp(c2->room,"*") == 0))) return 0;
  return 1;//All are the same! Success
}
*/
