#include "Tuples.c"

int main() {
    char* name = "Tyler Pauly";
    char* phone = "847-602-8270";
    char* course = "CSC 173";
    char* grade = "-C";
    int SID = 29944361;
    char* address ="2 Brittany Lane Lincolnshire, IL";
    
    CSG *CSGP = createCSG(course, SID, grade);
    
    printf("Course: %s, ID: %d, Grade: %s\n", CSGP->course, CSGP->SID, CSGP->grade);
    
    SNAP *SNAPP = createSNAP(SID, name, address, phone);
    
    printf("ID: %d, Name: %s, Address: %s, Phone: %s\n", SNAPP->SID, SNAPP->name, SNAPP->address, SNAPP->phone);
    
    CDH *CDHP = createCDH(course, "Monday", "12:00pm");
    
    printf("Course: %s, Day: %s, Time: %s\n", CDHP->course, CDHP->day, CDHP->hour);
    
    CP *CPP = createCP(course, "CSC 172");
    
    printf("Course: %s, Prerequisite: %s\n", CPP->course, CPP->prereq);
    
    CR *CRP = createCR(course, "Latimore");
    
    printf("Course: %s, Room: %s\n", CRP->course, CRP->room);
}

