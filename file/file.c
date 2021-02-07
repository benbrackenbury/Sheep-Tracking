//
// Created by Ben Brackenbury on 11/12/2020.
//

#include "../movement/movement.h"
#include "../config/config.h"
#include <stdlib.h>
#include <stdio.h>
#include "file.h"

/**
 * Handles opening the data file to use.
 * Reads file name from config if provided,
 * otherwise prompts to user for a file name.
 */
void selectFile() {
    if (!config.isUsingDefault) {
        strlcpy(filename, config.dataFileName, sizeof(filename));
    } else {
        printf("Name of data file:\n> ");
        scanf(" %[^\n]s", filename);
    }
    file = fopen(filename, "r");
}

/**
 * Determines whether a string can successfully be
 * converted to a float type.
 * @param token string to attempt to convert
 * @return true if provided string can be a float,
 * otherwise false.
 */
bool isValidFloat(char* token) {
    float _float;
    return (sscanf(token, "%f", &_float) == 1);
}

/**
 * Reads each lines of data file, discarding any invalid
 * or duplicated lines.
 */
void scanFile() {
    printf("Reading data from file...\n");

    int validLinesCount = 0;
    int duplicateLinesCount = 0;
    int invalidLinesCount = 0;

    prevLat = 0;
    prevLon = 0;
//    previousPosition = malloc(sizeof(position));

    while(!feof(file)) {
        //for each line of file
        char scannedLine[200];

        //save line to string
        fscanf(file, " %[^\n]s", scannedLine);
        char* token = strtok(scannedLine, ", ");
        fieldIndex = 0;

        //split line into tokens and assign to temp variables
        while (token != NULL) {
            //break if invalid
            if (!isValidFloat(token)) {
                invalidLinesCount++;
                break;
            }

            switch (fieldIndex) {
                case LAT:
                    tmpLat = strtof(token, 0);
                    break;
                case LON:
                    tmpLon = strtof(token, 0);
                    break;
                case DATE:
                    strlcpy(tmpDateTimeStr, token, sizeof(tmpDateTimeStr));
                    break;
                case TIME:
                    strlcat(tmpDateTimeStr, ", ", sizeof(tmpDateTimeStr));
                    strlcat(tmpDateTimeStr, token, sizeof(tmpDateTimeStr));
                    break;
                default:
                    printf("Something went wrong :(\n");
            }

            fieldIndex = (fieldIndex+1) % 4;
            token = strtok(NULL, ", ");
        }

        //create new position struct
        position* newPosition = createPosition(tmpLat, tmpLon, stringToTime(tmpDateTimeStr));

        bool matchesConstraints =
                (config.isUsingDefault ||
                    config.minLat==0 ||config.maxLat==0 || config.minLon==0 || config.maxLat==0
                ) ||
                (newPosition->lat >= config.minLat && newPosition->lat <= config.maxLat &&
                newPosition->lon >= config.minLon && newPosition->lon <= config.maxLon);

        bool diffPos = newPosition->lat != prevLat || newPosition->lon != prevLon;
        if (diffPos && matchesConstraints) {
            insertAtTailPositions(&positionHead, newPosition);
            validLinesCount++;
        } else {
            duplicateLinesCount++;
        }

        prevLat = newPosition->lat;
        prevLon = newPosition->lon;
//        insertAtTailPositions(&positionHead, newPosition);
//        validLinesCount++;
    }

    fclose(file);
    printf("Created %d position items;\n", validLinesCount);
    printf("Removed %d duplicates\n", duplicateLinesCount);
    printf("and %d invalid lines\n\n\n", invalidLinesCount);
}