//
// Created by Ben Brackenbury on 07/12/2020.
//
#include "movement.h"
#include <stdlib.h>
#include "../file/file.h"

/**
 * Returns behaviour based on speed.
 * @param speed
 * @return integer enum representing behaviour types.
 */
int determineBehaviour(double speed) {
    if (0 <= speed && speed < 0.02) {
        return STATIONARY;
    } else if (0.02 <= speed && speed < 0.33) {
        return FORAGING;
    } else if (0.33 <= speed && speed < 12) {
        return MOVING;
    } else {
        return IMPLAUSIBLE;
    }
}

/**
 * Prints behaviour int as enumerated string.
 * @param value behaviour int
 * @return behaviour type
 */
char* getBehaviour(int value) {
    char* behaviour = malloc(sizeof(char[20]));
    switch (value) {
        case 0:
            strlcpy(behaviour, "STATIONARY", sizeof(behaviour));
            break;
        case 1:
            strlcpy(behaviour, "FORAGING", sizeof(behaviour));
            break;
        case 2:
            strlcpy(behaviour, "MOVING", sizeof(behaviour));
            break;
        case 3:
            strlcpy(behaviour, "IMPLAUSIBLE", sizeof(behaviour));
            break;
        default:
            strlcpy(behaviour, "STATIONARY", sizeof(behaviour));
            break;
    }
    return behaviour;
}

/**
 * Creates a new movement struct.
 * @param id position id
 * @param position1 first position
 * @param position2  second position
 * @return pointer to new movement struct
 */
movement* createMovement(int id, position* position1, position* position2) {
    movement* newMovementPtr;
    newMovementPtr = malloc(sizeof(movement));
    if (newMovementPtr == NULL) {
        return NULL;
    } else {
        newMovementPtr->id = id;
        newMovementPtr->startTime = position1->time;
        newMovementPtr->startLat = position1->lat;
        newMovementPtr->startLon = position1->lon;
        newMovementPtr->endLat = position2->lat;
        newMovementPtr->endLon = position2->lon;
        newMovementPtr->distance = calculateDistance(position1, position2);
        newMovementPtr->duration = calculateTimeDifference(position1, position2);
        newMovementPtr->speed = calculateSpeed(position1, position2);
        newMovementPtr->behaviour = determineBehaviour(newMovementPtr->speed);
        newMovementPtr->next = NULL;
        return newMovementPtr;
    }
}

/**
 * Inserts movement at tail of linked list.
 * @param currentHeadPtr pointer to head of movements linked list
 * @param newMovementPtr pointer to movement struct to add to list
 */
void insertAtTailMovements(movement** currentHeadPtr , movement* newMovementPtr) {
    movement** tempPtr;
    tempPtr = currentHeadPtr;
    while (*tempPtr != NULL) {
        tempPtr = &((*tempPtr)->next);
    }
    newMovementPtr->next = *tempPtr;
    *tempPtr = newMovementPtr;
}

/**
 * Goes through movements linked list and writes all movement
 * data to a csv file.
 * @param listPtr
 */
void writeMovementsToFile(movement* listPtr) {
    printf("Preparing for file write...\n");
    strcpy(filename, "movement_data.csv");
    file = fopen(filename, "w");
    printf("Writing movement data to %s...\n", filename);
    while (listPtr != NULL) {
        //write to file
        fprintf(file, "%d, %s, %f, %f, %f, %f, %f, %f, %f, %s\n",
                listPtr->id,
                timeToString(listPtr->startTime),
                listPtr->startLon,
                listPtr->startLat,
                listPtr->endLon,
                listPtr->endLat,
                listPtr->distance,
                listPtr->duration,
                listPtr->speed,
                getBehaviour(listPtr->behaviour)
            );
        listPtr = listPtr->next;
    }
    printf("Write complete\n\n");
    fclose(file);
}

/**
 * Displays the total time in seconds for
 * each behaviour.
 */
void printTimes() {
    printf("\nTotal times (seconds):\n"
           "==========================\n"
           "Stationary: %.0f,\n"
           "Foraging: %.0f,\n"
           "Moving: %.0f,\n"
           "Implausible: %.0f\n"
           "----------------------\n"
           "Total: %.0f\n\n",
           stationaryCount, foragingCount, movingCount, implausibleCount,
           stationaryCount+foragingCount+movingCount+implausibleCount
    );
}