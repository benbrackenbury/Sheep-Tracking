//
// Created by Ben Brackenbury on 22/11/2020.
//
#import "position.h"
#import <stdlib.h>
#import <string.h>
#import <math.h>
#import "../movement/movement.h"
#include "../config/config.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

/**
 * Creates a new  position struct.
 * @param lat latitude
 * @param lon longitude
 * @param time date and time
 * @return pointer to new position struct
 */
position* createPosition(float lat, float lon, time_t time) {
    position* newPositionPtr;
    newPositionPtr = malloc(sizeof(position));
    if (newPositionPtr == NULL) {
        return NULL;
    } else {
        newPositionPtr->lat = lat;
        newPositionPtr->lon = lon;
        newPositionPtr->time = time;
        newPositionPtr->next = NULL;
        return newPositionPtr;
    }
}

/**
 * Inserts a position at the tail of a linked list.
 * @param currentHeadPtr pointer to head of positions linked list
 * @param newPositionPtr pointer to position struct to be added to list
 */
void insertAtTailPositions(position** currentHeadPtr , position *newPositionPtr) {
    position** tempPtr;
    tempPtr = currentHeadPtr;
    while (*tempPtr != NULL) {
        tempPtr = &((*tempPtr)->next);
    }
    newPositionPtr->next = *tempPtr;
    *tempPtr = newPositionPtr;
}

/**
 * Calculates difference in time between two positions.
 * @param first pointer to first position
 * @param second pointer to second position
 * @return time difference in seconds
 */
double calculateTimeDifference(position* first, position* second) {
    return difftime(second->time, first->time);
}

/**
 * Calculates speed of movement between two positions.
 * @param first pointer to first position
 * @param second pointer to second position
 * @return speed in meters per second
 */
double calculateSpeed(position* first, position* second) {
    return calculateDistance(first, second)/calculateTimeDifference(first, second);
}

/**
 * Creates a time_t object from a string.
 * @param timeString date and time as a string
 * @return time_t object
 */
time_t stringToTime(char* timeString) {
    struct tm time;

    char timeStringChar[30];
    strlcpy(timeStringChar, timeString, sizeof(timeStringChar));

    char* dateToken = strtok(timeStringChar, "/:,");
    int dateFieldIndex = 0;

    while (dateToken != NULL) {
        int item = atoi(dateToken);
        switch (dateFieldIndex) {
            case 0:
                time.tm_mday = item;
                break;
            case 1:
                time.tm_mon = item - 1;
                break;
            case 2:
                time.tm_year = item - 1900;
                break;
            case 3:
                time.tm_hour = item;
                break;
            case 4:
                time.tm_min = item;
                break;
            case 5:
                time.tm_sec = item;
                break;
            default:
                break;
        }
        dateFieldIndex = (dateFieldIndex+1) % 6;
        dateToken = strtok(NULL, "/:,");
    }
    time.tm_isdst = -1;

    return mktime(&time);
}

/**
 * Creates a formatted string from a time_t object.
 * @param rawTime time_t object to convert
 * @return formatted string
 */
char* timeToString(time_t rawTime) {
    char* buffer = malloc(sizeof(char[20]));
    strftime(buffer, 20, "%d/%m/%Y, %H:%M:%S", localtime(&rawTime));
    return buffer;
}

/**
 * Goes through each position in linked list,
 * creates movement structs from each pair, and adds each
 * movement struct to a linked list.
 * @param listPtr pointer to head of positions linked list
 */
void generateMovements(position* listPtr) {
    int movementID = 1;
    stationaryCount = 0;
    foragingCount = 0;
    movingCount = 0;
    implausibleCount = 0;

    while (listPtr->next != NULL) {
        movement* newMovement = createMovement(movementID, listPtr, listPtr->next);
        bool matchesConstraints =
                newMovement->speed < config.maxSpeed &&
                newMovement->duration >= config.minDuration &&
                newMovement->duration <= config.maxDuration;
        if (matchesConstraints) {
            insertAtTailMovements(&movementHead, newMovement);
            //count
            if (newMovement->behaviour == STATIONARY) { stationaryCount+= newMovement->duration; }
            else if (newMovement->behaviour == FORAGING) { foragingCount+= newMovement->duration; }
            else if (newMovement->behaviour == MOVING) { movingCount+= newMovement->duration; }
            else { implausibleCount+= newMovement->duration; }
            movementID++;
        }
        listPtr = listPtr->next;
    }
}

/**
 * The great circle distance or the orthodromic distance is the shortest distance
 * between two points on a sphere
 * @param first - a structure containing (at least) the fields Double lng and Double lat
 * @param second - a structure containing (at least) the fields Double lng and Double lat
 * @return distance in m
 */
double calculateDistance(position* first, position* second) {
    double lon1 = degToRad(first->lon);
    double lon2 = degToRad(second->lon);
    double lat1 = degToRad(first->lat);
    double lat2 = degToRad(second->lat);
    // Haversine formula (alt. vincenties formula)
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = pow(sin(dlat / 2), 2)
               + cos(lat1) * cos(lat2)
                 * pow(sin(dlon / 2),2);
    double c = 2 * asin(sqrt(a));
    // Radius of earth in kilometers. Use 3956 for miles
    double r = 6371;
    // calculate the result (in m)
    return((c * r)*1000);
}