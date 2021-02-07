//
// Created by Ben Brackenbury on 11/12/2020.
//

#ifndef ASSIGNMENT_CONFIG_H
#define ASSIGNMENT_CONFIG_H

#include <stdbool.h>

/**
 * Global struct that stores configurable variables.
 */
struct config {
    bool isUsingDefault;
    char dataFileName[20];
    int minDuration, maxDuration;
    double maxSpeed;
    int numBars;
    double minLon, maxLon;
    double minLat, maxLat;
} config;


//methods
void readConfig(char*);
void writeConfig(char*);
void setDefaultConfig();
void editConfig();

#endif //ASSIGNMENT_CONFIG_H
