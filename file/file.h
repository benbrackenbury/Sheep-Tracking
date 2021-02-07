//
// Created by Ben Brackenbury on 11/12/2020.
//

#ifndef ASSIGNMENT_FILE_H
#define ASSIGNMENT_FILE_H

#include <stdio.h>
#include <stdbool.h>

//variable definitions
FILE* file;
char filename[20];

//methods
void selectFile();
bool isValidFloat(char*);
void scanFile();

#endif //ASSIGNMENT_FILE_H
