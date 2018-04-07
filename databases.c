#pragma once

#include <stdlib.h>
#include <stdio.h>

//define structs for each relation

//Course, SID, Grade
struct CSG {
    char *course;
    int SID;
    char *grade;
    struct CSG *next;
};
typedef struct CSG CSG;
//SID, Name, Address, Phone
struct SNAP {
    int SID;
    char *name;
    char *address;
    char *phone;
    struct SNAP *next;
};
typedef struct SNAP SNAP;
//Course, Prerequisite
struct CP {
    char *course;
    char *prereq;
    struct CP *next;
};
typedef struct CP CP;
//Course, Day, Hour
struct CDH {
    char *course;
    char *day;
    char *hour;
    struct CDH *next;
};
typedef struct CDH CDH;
//Course, Room
struct CR {
    char *course;
    char *room;
    struct CR *next;
};
typedef struct CR CR;

//function prototypes
CSG *createCSG(char *, int, char *);
SNAP *createSNAP(int, char *, char *, char *);
CP *createCP(char *, char *);
CDH *createCDH(char *, char *, char *);
CR *createCR(char *, char *);

//equals functions
int equal_SNAP(SNAP* s1, SNAP* s2);
int equal_CSG(CSG* c1, CSG* c2);
int equal_CP(CP* c1, CP* c2);
int equal_CDH(CDH* c1, CDH* c2);
int equal_CR(CR* c1, CR* c2);
