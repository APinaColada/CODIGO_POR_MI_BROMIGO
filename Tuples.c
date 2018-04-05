#include "Tuples.h"
#include <string.h>

//Creates Course, Student ID, Grade Schema
//Key: Student ID
CSG *createCSG(char *course, int SID, char *grade) {
    CSG *CSGP = malloc(sizeof(CSG));
    CSGP->course = malloc(sizeof(char)*100);
    strncpy(CSGP->course, course, 100);
    CSGP->SID = SID;
    CSGP->grade = malloc(sizeof(char)*100);
    strncpy(CSGP->grade, grade, 100);
    CSGP->next = NULL;

    return CSGP;
}

//Creates Student ID, Name, Address, and Phone Schema
//Key: Student ID
SNAP *createSNAP(int SID, char *name, char *address, int phone) {
    SNAP *SNAPP = malloc(sizeof(SNAP));
    SNAPP->SID = SID;
    SNAPP->name = malloc(sizeof(char)*100);
    strncpy(SNAPP->name, name, 100);
    SNAPP->address = malloc(sizeof(char)*100);
    strncpy(SNAPP->address, address, 100);
    SNAPP->phone = phone;
    SNAPP->next = NULL;

    return SNAPP;
}

//Creates Course, Prerequisite Schema
//Key: Course
CP *createCP(char *C, char *P) {
    CP *CPP = malloc(sizeof(CP));
    CPP->course = malloc(sizeof(char)*100);
    strncpy(CPP->course, C, 100);
    CPP->prereq = malloc(sizeof(char)*100);
    strncpy(CPP->prereq, P, 100);
    CPP->next = NULL;

    return CPP;
}

//Creates Course, Day, Hour Schema
//Key: Course
CDH *createCDH(char *C, char *D, char *H) {
    CDH *CDHP = malloc(sizeof(CDH));
    CDHP->course = malloc(sizeof(char)*100);
    strncpy(CDHP->course, C, 100);
    CDHP->day = malloc(sizeof(char)*100);
    strncpy(CDHP->day, D, 100);
    CDHP->hour=malloc(sizeof(char)*100);
    strncpy(CDHP->hour, H, 100);
    CDHP->next = NULL;

    return CDHP;
}

//Creates Course, Room Schema
CR *createCR(char *C, char *R) {
    CR *CRP = malloc(sizeof(CR));
    CRP->course=malloc(sizeof(char)*100);
    strncpy(CRP->course, C, 100);
    CRP->room=malloc(sizeof(char)*100);
    strncpy(CRP->room, R, 100);
    CRP->next = NULL;

    return CRP;
}
