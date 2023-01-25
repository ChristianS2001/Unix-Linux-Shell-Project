/*
    Christian Smith, tuk96703@temple.edu, TUID: 915789291, Purpose of this file:
        The purpose of this file is to build commands internally for the shell, check and use said commands for the shell.
*/

//in assignment it doesn't say to implement read from stdin functionality for internal commands
//all functions provide proper error statements

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <limits.h>
#include <dirent.h> //including the directory library to use the methods
#include <errno.h>
#include "external.h"
#define commandCount 7

//this function checks for built in commands in CMDstruct, returns 1 for true, and returns 0 for false
int checkForCMD(char** parseArray){
    char* commandList[commandCount]; //7 internal commands to check for
    commandList[0] = "cd";
    commandList[1] = "clr";
    commandList[2] = "dir";
    commandList[3] = "environ";
    commandList[4] = "echo";
    commandList[5] = "help";
    commandList[6] = "pause";
    //we filled a list to compare

    for(int i = 0; i < commandCount; i++){
        if (strcmp(parseArray[0], commandList[i]) == 0){
            return 1;
        }
    }
    return 0;
}

//this function will print current directory, move into another directory, or print an error
void cd (CMDstruct cmdLine){
    if(cmdLine.parseArray == NULL){
        fprintf(stderr, "An error has occurred\n"); //no CD command2
    }
    if(cmdLine.count == 1) {
        //print out current directory
        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        printf("Current Directory: %s\n", path);
    }
    if(cmdLine.count == 2) {
        int dirCheck = chdir(cmdLine.parseArray[1]);
        if(dirCheck != 0){ //if cd wasn't successfull
            fprintf(stderr, "An error has occurred\n");
        }
        //there are multiple arguments and we need to check correctly
    }
    if(cmdLine.count > 2) {
        fprintf(stderr, "An error has occurred\n"); //to many arguments error
    }
}
//clrScrn() function preforms what is asked of it via the assignment by clearing the shell prompt screen when called
void clrScrn(CMDstruct cmdLine)
{
    if(cmdLine.count == 0){
        fprintf(stderr, "An error has occurred\n"); //argument # check
    }
    if(cmdLine.count > 1){
        fprintf(stderr, "An error has occurred\n"); //argument # check
    }
    if(cmdLine.count == 1){
        const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J"; //this is what actually clears your screen
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
    }
}

//dirCMD() function preforms what is asked of it via the assignment (dir()) and lists out everything in your directory or given directory
void dirCMD(const char* path, FILE* writeFile) {
    //files like . and .. means current directory and prev directory, also files with names .(insertnamehere) mean directories

    DIR* dir = opendir(path); //this dir struct is going to be our directory that we open also "." means I want to see current file in curr direc
    if (dir == NULL) {
        fprintf(stderr, "An error has occurred\n");
        return;
    }
    struct dirent* entity; //we use a pointer to this datatype to iterate over all the elements in the direc we have opened
    while ((entity = readdir(dir)) != NULL) { //to start reading a file in direc we have opened and now run while till entity is NULL
        fprintf(writeFile, "%s\n", entity->d_name); //printing name of files
    }
    closedir(dir);
}
//this function calls the dir() function with the correct information and parameters
int callDir(int count, char* arguments[], FILE* writeFile) { //call this to execute dirCMD, arguments may not work since argv might have special properties
    if(count == 1) { //current dir
        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        dirCMD(path, writeFile);
    }
    if(count == 2){ //specified dir
        dirCMD(arguments[1], writeFile);
    }
    if(count > 2){
        fprintf(stderr, "An error has occurred\n");
        dirCMD(arguments[1], writeFile); //This is what the actual UNIX terminal does so I put it here to replicate that
    }
    return 0;
}

//environ function preforms what is asked via assignment which is to list out your entire environment
void environment(FILE* writeFile){
    extern char** environ; //global var
    int i = 0;
    while(environ[i] != NULL){ //loop to print environment variables
        fprintf(writeFile, "%s\n", environ[i]);
        i++;
    }
}

//echo function preforms what is asked via assignment which is to print out the arguments to stdout
void echo(CMDstruct cmdLine, FILE* writeFile){
    if(cmdLine.count > 1){
        for(int i = 1; i < cmdLine.count; i++){
            fprintf(writeFile, "%s", cmdLine.parseArray[i]);
            if(i == cmdLine.count-1){
                fprintf(writeFile, "\n");
            } else{
                fprintf(writeFile, " ");
            }
        }
    }
}

//help function preforms what is asked via assignment which is to load up readme_doc (documentation) and pipe to more while printing to stdout and it supports redirection removing pipe functionality when required too
void help(FILE* writeFile, CMDstruct help){
    //we don't know what folder readme_doc will be in
    char* env = strdup(getenv("SHELL"));
    CMDstruct more;
    char* shell = getenv("SHELL");
    char* point = strrchr(env, '/');
    point[0] = '\0';
    env = realloc(env, strlen(env) + 13); //we are dynamically allocating + the 13 char's for "/readme_doc"
    strcat(env, "/readme_doc"); //giving it the file to read from
    FILE* f1 = fopen(env, "rb");
    if(f1 == NULL){
        fprintf(stderr, "An error has occurred\n");
        return;
    }
    char* line = NULL;
    size_t len;
    if(help.outputFlag == 1 || help.doubleOutputFlag == 1){ //where we support redirection and do help without piping to more
        while(getline(&line, &len, f1) != -1){
            fprintf(writeFile, "%s", line);
        }
    } else{
        //no arrow go pipe more
        more.parseArray = malloc(2); //at least one command and NULL ptr
        more.parseArray[0] = strdup("more");
        more.parseArray[1] = NULL;
        more.pipeFlag = 0;
        more.ampersandFlag = 0;
        more.outputFlag = 0;
        more.inputFlag = 0;
        more.doubleOutputFlag = 0;
        more.count = 1;
        more.errorFlag = 0;
        externalCommands(more, shell, stdout, f1); //pipe to more
    }
    //clearing potential memory leaks
    free(env);
    free(line);
    fclose(f1);
}

//pause function preforms what is asked via assignment which is to pause your shell till an enter is pressed or \n char is given
void pauseCMD(){ 
    //wait for enter
    printf("myshell is currently paused, please click enter key to resume:  ");
    while(getc(stdin) != '\n'){
        
    }
}

//this function will check for a function based on the given string and other data and run the correct function
void callSpecificFunc(CMDstruct funcName, FILE* writeFile, FILE* readFile){
    //edge case check to make sure we don't try and access something that exists
    if(funcName.count == 0){
        printf("\n");
    } else{ //all the checks for the functions and commands
        if(strcmp(funcName.parseArray[0], "cd") == 0){
            cd(funcName);
        }
        else if(strcmp(funcName.parseArray[0], "clr") == 0){
            clrScrn(funcName);
        }
        else if(strcmp(funcName.parseArray[0], "dir") == 0){
            callDir(funcName.count, funcName.parseArray, writeFile);
        }
        else if(strcmp(funcName.parseArray[0], "environ") == 0){
            environment(writeFile);
        }
        else if(strcmp(funcName.parseArray[0], "echo") == 0){
            echo(funcName, writeFile);
        }
        else if(strcmp(funcName.parseArray[0], "help") == 0){
            help(writeFile, funcName);
        }
        else if(strcmp(funcName.parseArray[0], "pause") == 0){
            pauseCMD();
        }else{
            fprintf(stderr, "An error has occurred\n");
        }
    }
}