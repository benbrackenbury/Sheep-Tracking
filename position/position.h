//
// Created by Ben Brackenbury on 22/11/2020.
//

#ifndef ASSIGNMENT_POSITION_H
#define ASSIGNMENT_POSITION_H

#include <time.h>

/**
 * Struct representing position data.
 */
typedef struct position {
    float lat, lon;
    time_t time;
    struct position* next;
} position;

typedef member* node_ptr;
typedef struct {
    char name[100];
    int id;
    int age;
    node_ptr next;
} member;

node_ptr make_node(char * name, int age, int id) {
    node_ptr newMemberPtr;
    newMemberPtr = malloc(sizeof(member));
    if (newMemberPtr == NULL) {
        return NULL;
    } else {
        strcpy(newMemberPtr->name, name);
        newMemberPtr->age = age;
        newMemberPtr->id = id;
        return newMemberPtr;
    }
}

void insert_at_top(node_pointer * existingHeadPtr , node_pointer newNodePtr) {
    newNodePtr->next = *existingHeadPtr ;
    *existingHeadPtr = newNodePtr;
}

node_ptr memberHead = NULL;
node_ptr newMember = make_node("John Doe", 21, 123456);
insert_at_top(&memberHead, newMember);

void listMembers(node_pointer listPtr){
    //while there is a node to display
    while (listPtr != NULL) {
        //print attributes of currrent member
        printf("Name: %s,  ", listPtr->name);
        printf("Age: %d,  ", listPtr->age);
        printf("ID: %d;\n", listPtr->id);
        //move pointer to next member in linked list
        listPtr = listPtr->next;
    }
}

void deleteMemberList(node_pointer listPtr) {
    node_ptr current = *listPtr;
    node_ptr next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    listPtr = NULL;
}


//positions linked list head
position* positionHead;

//methods
position* createPosition(float, float, time_t);
void insertAtTailPositions(position**, position*);
double calculateSpeed(position*, position*);
double calculateTimeDifference(position*, position*);
time_t stringToTime(char*);
char* timeToString(time_t time);
void generateMovements(position* listPtr);
double calculateDistance(position*, position*);

//enum for position attributes
enum field {LAT=0, LON=1, DATE=2, TIME=3};
int fieldIndex;

//variable definitions
float tmpLat, tmpLon;
char tmpDateTimeStr[40];
float prevLat, prevLon;

#endif //ASSIGNMENT_POSITION_H
