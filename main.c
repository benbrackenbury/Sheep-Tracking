#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "config/config.h"
#include "file/file.h"
#include "position/position.h"
#include "movement/movement.h"

/**
 * Initialises linked list heads as NULL,
 * and sets the config's isUsingDefault value to true.
 */
void init() {
    positionHead = NULL;
    movementHead = NULL;
    config.isUsingDefault = true;
}

/**
 * Starts the main functionality to read data from a
 * data file, generate movements, and write movement data
 * to a CSV output file.
 */
void run() {
    selectFile();
    scanFile();
    generateMovements(positionHead);
    writeMovementsToFile(movementHead);
    printTimes();
}

/**
 * Displays the main menu for the application.
 */
void mainMenu() {
    int selection = 0;

    while (selection > -1) {
        fflush(stdin);
        printf("Select an option:\n"
               "(enter negative number to quit)\n"
               "=============================\n"
               "1. Load a configuration file\n"
               "2. Generate csv from data\n"
               "3. Edit current config\n"
               "4. Save changes to config file\n"
               "-----------------------------\n"
               "> ");
        char selectionStr[1];

        scanf(" %[^\n]s", selectionStr);
        char* cfName = malloc(sizeof(char[20]));
        selection = atoi(selectionStr);
        switch (selection) {
            case 1:
                printf("Name of config file\n> ");
                scanf(" %[^\n]s", cfName);
                printf("Using config file '%s'\n", cfName);
                readConfig(cfName);
                printf("\n\n");
                break;
            case 2:
                run();
                break;
            case 3:
                editConfig();
                break;
            case 4:
                printf("Name of config file\n> ");
                scanf(" %[^\n]s", cfName);
                writeConfig(cfName);
                printf("\n\n");
                break;
            default:
                if (selection > -1) {
                    printf("Not a valid selection\n\n");
                }
                break;
        }
        free(cfName);
    }
}

/**
 * Main method. Reads config file if provided,
 * otherwise displays main menu
 * @param argc number of arguments
 * @param argv command line arguments
 * @return 0 on successful execution
 */
int main(int argc, char *argv[]) {
    init();
    
    if(argc == 2) {
        readConfig(argv[1]);
        run();
    } else {
        setDefaultConfig();
        printf("No arguments given, using default configuration\n\n");
        mainMenu();
    }

    return 0;
}