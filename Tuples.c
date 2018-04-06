#include "Tuples.h"
#include <string.h>

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
