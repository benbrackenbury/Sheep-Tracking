//
// Created by Ben Brackenbury on 11/12/2020.
//

#include "config.h"
#include "../file/file.h"
#include <stdlib.h>
#include <string.h>

/**
 * Reads data from config file and sets the config struct
 * to the values provided.
 * @param name name of config file to read.
 */
void readConfig(char* name) {
    strlcpy(filename, name, sizeof(filename));
    file = fopen(filename, "r");
    char* ignore;
    config.isUsingDefault = false;

    while(!feof(file)) {
        char scannedLine[200];
        fscanf(file, " %[^\n]s", scannedLine);

        char* token = strtok(scannedLine, " <>:");
        while (token != NULL) {
            if (strcmp(token, "data_file_name") == 0) {
                token = strtok(NULL, " <>:");
                char* nameWithoutQuotes = strtok(token, "\"");
                strlcpy(config.dataFileName, nameWithoutQuotes, sizeof(config.dataFileName));
            }
            else if (strcmp(token, "movement_duration") == 0) {
                token = strtok(NULL, " <>:");
                config.minDuration = atoi(token);
                token = strtok(NULL, " <>:");
                config.maxDuration = atoi(token);
            } else if (strcmp(token, "maximum_speed") == 0) {
                token = strtok(NULL, " <>:");
                config.maxSpeed = strtod(token, &ignore);
            } else if (strcmp(token, "num_bars") == 0) {
                token = strtok(NULL, " <>:");
                config.numBars = atoi(token);
            } else if (strcmp(token, "range_lon") == 0) {
                token = strtok(NULL, " <>:");
                config.minLon = strtod(token, &ignore);
                token = strtok(NULL, " <>:");
                config.maxLon = strtod(token, &ignore);
            } else if (strcmp(token, "range_lat") == 0) {
                token = strtok(NULL, " <>:");
                config.minLat = strtod(token, &ignore);
                token = strtok(NULL, " <>:");
                config.maxLat = strtod(token, &ignore);
            } else {
                token = strtok(NULL, " <>:");
            }
        }
    }

    fclose(file);
}

/**
 * Sets config struct values to defaults. Used when
 * no config file is provided.
 */
void setDefaultConfig() {
    config.isUsingDefault = true;
    config.minDuration = 0;
    config.maxDuration = 604800;
    config.maxSpeed = 50;
    config.numBars = 20;
    config.minLon = 0;
    config.maxLon = 0;
    config.minLat = 0;
    config.maxLat = 0;
}

/**
 * Edits current configuration
 */
void editConfig() {
    char dateFileName[20];
    char minDurationStr[20];
    char maxDurationStr[20];
    char maxSpeedStr[20];
    char numBarsStr[20];
    char minLonStr[20];
    char maxLonStr[20];
    char minLatStr[20];
    char maxLatStr[20];

    printf("\nData file name:\n> ");
    scanf(" %[^\n]s", dateFileName);
    printf("Min movement duration:\n> ");
    scanf(" %[^\n]s", minDurationStr);
    printf("Max movement duration:\n> ");
    scanf(" %[^\n]s", maxDurationStr);
    printf("Max movement speed:\n> ");
    scanf(" %[^\n]s", maxSpeedStr);
    printf("Number of bars:\n> ");
    scanf(" %[^\n]s", numBarsStr);
    printf("Minimum longitude:\n> ");
    scanf(" %[^\n]s", minLonStr);
    printf("Maximum longitude:\n> ");
    scanf(" %[^\n]s", maxLonStr);
    printf("Minimum latitude:\n> ");
    scanf(" %[^\n]s", minLatStr);
    printf("Maximum latitude:\n> ");
    scanf(" %[^\n]s", maxLatStr);

    strlcpy(config.dataFileName, dateFileName, sizeof(config.dataFileName));
    config.minDuration = atoi(minDurationStr);
    config.maxDuration = atoi(maxDurationStr);
    config.maxSpeed = atof(maxSpeedStr);
    config.numBars = atoi(numBarsStr);
    config.minLon = atof(minLonStr);
    config.maxLon = atof(maxLonStr);
    config.minLat = atof(minLatStr);
    config.maxLat = atof(maxLatStr);

    printf("\nConfig updated\n");
}

/**
 * Writes current configuration to config file.
 * Overwrites a file if it already exists.
 * @param name name of config file to write to.
 */
void writeConfig(char* name) {
    strlcpy(filename, name, sizeof(filename));
    file = fopen(filename, "w");
    printf("Writing config data to %s...\n", filename);

    if (config.isUsingDefault) {
        printf("\nNo data file specified\n");
        printf("Enter name of data file for config:\n> ");
        char fileName[20];
        scanf(" %[^\n]s", fileName);
        strlcpy(config.dataFileName, fileName, sizeof(config.dataFileName));
    }

    fprintf(file, "data_file_name \"%s\"\n"
                  "movement_duration < %d : %d >\n"
                  "maximum_speed %f\n"
                  "num_bars %d\n"
                  "range_lon < %f : %f>\n"
                  "range_lat < %f : %f>",
            config.dataFileName,
            config.minDuration,
            config.maxDuration,
            config.maxSpeed,
            config.numBars,
            config.minLon,
            config.maxLon,
            config.minLat,
            config.maxLat
    );

    printf("Write complete\n\n");
    fclose(file);
}