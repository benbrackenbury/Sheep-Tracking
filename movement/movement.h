//
// Created by Ben Brackenbury on 07/12/2020.
//

#ifndef ASSIGNMENT_MOVEMENT_H
#define ASSIGNMENT_MOVEMENT_H

#include "time.h"
#include "string.h"
#include "../position/position.h"

//variable definitions
double stationaryCount;
double foragingCount;
double movingCount;
double implausibleCount;

//enum for behaviour type
enum behaviour {STATIONARY=0, FORAGING=1, MOVING=2, IMPLAUSIBLE=3};

/**
 * Struct representing movement data.
 */
typedef struct movement {
    int id;
    time_t startTime;
    double startLat, startLon, endLat, endLon,
            distance, duration, speed;
    int behaviour;
    struct movement*  next;
} movement;

//movements linked list head
movement* movementHead;

//methods
movement* createMovement(int, position*, position*);
void insertAtTailMovements(movement**, movement*);
int determineBehaviour(double);
char* getBehaviour(int);
void writeMovementsToFile(movement *listPtr);
void printTimes();

#endif //ASSIGNMENT_MOVEMENT_H
