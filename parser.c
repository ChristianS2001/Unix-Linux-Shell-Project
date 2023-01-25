/*
    Christian Smith, tuk96703@temple.edu, TUID: 915789291, Purpose of this file:
        The purpose of this file is to parse all info to feed into my myShell.c and any other info we need.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

//is a function to parse a character from cmdLine to save into an array for shell usage
Count parseInput(char* cmdLine) {
    Count count; //creating our Count structure variable named count
    count.count = 0;
    char* singleCmd;
    count.cmdLineArray = malloc(sizeof(CMDstruct)); //allocating mem for an expectation of at least 1 command
    char* start = cmdLine;
    int commandLen = 0;
    char* separator; //this represents pipe and ampersand char's
    while((separator = strpbrk(start, "|&")) != NULL){
        count.cmdLineArray = realloc(count.cmdLineArray, (sizeof(CMDstruct) * (count.count + 1)));
        commandLen = (separator - start);
        singleCmd = malloc(commandLen + 1);
        strncpy(singleCmd, start, commandLen);
        singleCmd[commandLen - 1] = '\0'; //setting a null on the end of the string
        count.cmdLineArray[count.count] = parseSingleCmd(singleCmd);
        if(*separator == '&'){
            count.cmdLineArray[count.count].ampersandFlag = 1;
        }
        if(*separator == '|'){
            count.cmdLineArray[count.count].pipeFlag = 1;
        }
        count.count++; //we have added a command
        start = separator + 1;
        free(singleCmd);
    }
    if (separator == NULL){ 
        CMDstruct checkCmd = parseSingleCmd(start); //we start parsing the single command sequences to put into the commands array
        if(checkCmd.parseArray[0] != NULL){
            count.cmdLineArray = realloc(count.cmdLineArray, (sizeof(CMDstruct) * (count.count + 1)));
            count.cmdLineArray[count.count] = checkCmd;
            count.count++; //we have added a command
        }
    }
    return count;
} 

//is a function that parses a single command for properties and correct data to be used later on
CMDstruct parseSingleCmd (char* cmdLine){
    CMDstruct commands;
    char* savePtr;
    commands.errorFlag = 0;
    commands.outputFlag = 0;
    commands.doubleOutputFlag = 0;
    commands.inputFlag = 0;
    commands.pipeFlag = 0;
    commands.ampersandFlag = 0;
    commands.inputFile = NULL;
    commands.outputFile = NULL;
    char* string = strtok_r(cmdLine, " ", &savePtr);
    if(string != NULL){ //edge case if strtok ended up being null and preventing a segfault
        string = strdup(string);
        commands.parseArray = malloc(sizeof(char*));
        commands.count = 1;
        commands.parseArray[0] = string;
    }else {
        commands.parseArray = NULL;
    }

    //save startingString into commands
    while(string != NULL){ //this is due to the functionality of strtok
        string = strtok_r(NULL, " ", &savePtr); //we are tokenizing our string
        if(string != NULL){
            string = strdup(string);
            if(strcmp(string, ">") == 0){
                commands.outputFlag = 1;
                string = strtok_r(NULL, " ", &savePtr);
                if(string != NULL){
                    string = strdup(string);
                    commands.outputFile = string; //setting our output file name
                } else{
                    commands.errorFlag = 1; //set our error flag
                    break;
                }
            }
            else if(strcmp(string, "<") == 0){ //input redirection
                commands.inputFlag = 1;
                string = strtok_r(NULL, " ", &savePtr);
                if(string != NULL){
                    string = strdup(string);
                    commands.inputFile = string; //setting our inputFile name
                } else{
                    commands.errorFlag = 1; //set our error flag
                    break;
                }
            }
            else if(strcmp(string, ">>") == 0){ //if it is the appending version of output redirection
                commands.doubleOutputFlag = 1;
                string = strtok_r(NULL, " ", &savePtr); //tokenizing our string
                if(string != NULL){
                    string = strdup(string);
                    commands.outputFile = string;
                } else{
                    commands.errorFlag = 1;
                    break;
                }
            } else{
                commands.parseArray = realloc(commands.parseArray, sizeof(char*)*(commands.count + 1)); //reallocating memory for our array of arrays
                commands.parseArray[commands.count] = string;
                commands.count++; //keeping track of our count so we can manually add NULL pointer later and for looping
            }
        }
    }
    commands.parseArray = realloc(commands.parseArray, sizeof(char*)*(commands.count + 1)); //reallocating memory for our array of arrays
    commands.parseArray[commands.count] = NULL; //adding null pointer to the end of parseArray for struct

    return commands;
}

