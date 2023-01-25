#ifndef PARSER_H
#define PARSER_H

typedef struct cmdstruct {
    int count;
    char* inputFile;
    char* outputFile;
    int inputFlag; //if any non zero or neg # then it means there is a flag
    int outputFlag; //if any non zero or neg # then it means there is a flag
    int doubleOutputFlag; //if any non zero or neg # then it means there is a flag
    int ampersandFlag; //if 0 no ampersand found if 1 means ampersand found
    int pipeFlag; //if 0 no pipe found if 1 pipe found
    char** parseArray;
    int errorFlag; //if this flag is any non zero integer it means that the cmdLine when parsed picked up on an error and running cmdLine would result in an error
} CMDstruct;

typedef struct countIndex {
    int count;
    CMDstruct* cmdLineArray;
} Count;

Count parseInput(char* cmdLine);
CMDstruct parseSingleCmd (char* cmdLine);

#endif