/*
    Christian Smith, tuk96703@temple.edu, TUID: 915789291, Purpose of this file:
        The purpose of this file is to check for parent shell commands via a path and to properly fork, wait, and execute programs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <limits.h>
#include "external.h"
#include <sys/wait.h>
#include <dirent.h> //including the directory library to use the methods
#include <errno.h> //for errors and EXIT codes


//externalCommands() is our function that takes our path and works to support forking, executing, waiting, redirection, piping, and running programs in the background
//we also properly open and close important files such as STDIN and STDOUT
void externalCommands(CMDstruct cmdLine, char* shell, FILE* write, FILE* read){
    char* path = cmdLine.parseArray[0];
    if(strchr(cmdLine.parseArray[0], '/') == NULL){ //if we have not seen a '/' meaning we use env path
        path = find(cmdLine.parseArray[0]);
    }

    if(path == NULL){
        fprintf(stderr, "An error has occurred\n"); //this is for when path is not found via find() function
        return;
    }

    int rc = fork();
    int fileNumWrite = fileno(write);
    int fileNumRead = fileno(read);
    if(rc < 0){
        fprintf(stderr, "An error has occurred\n");
    } else if(rc == 0){
        if(write != stdout){ //create a single if statement for functionality regarding '>', '>>', and '|'
            dup2(fileNumWrite, STDOUT_FILENO); //closes stdout and then copies old into new (new being stdout)
        }
        if(read != stdin){
            dup2(fileNumRead, STDIN_FILENO);
        }
        setenv("PARENT", shell, 1);
        execv(path, cmdLine.parseArray); //execute the child
        fprintf(stderr, "An error has occurred\n");//child didn't execute
        exit(1);
    } else{
        if(cmdLine.ampersandFlag == 1){
            //don't wait
        }else {
            waitpid(rc, NULL, 0);
        }
        free(path);
    }
}

//find() function is doing the work we need to find a external command by allocating space, checking, locating, and returning said command
char* find(char* command){ //if any data is leaked from find it is deleted when we exec later on.
    char* path = strdup(getenv("PATH"));
    char* check = strtok(path, ":");
    if(check != NULL){
        char* checkCpy = malloc(strlen(check) + 2 + strlen(command));
        strcpy(checkCpy, check);
        strcat(checkCpy, "/");
        strcat(checkCpy, command);
        int checkAccess = access(checkCpy, X_OK); //returns 0 on success, on error -1 is returned
        if(checkAccess == 0){
            free(path);
            return checkCpy; //returning our new path string for externalCommands() to use
        } else{
            free(checkCpy);
            while((check = strtok(NULL, ":")) != NULL){
                char* checkCpy = malloc(strlen(check) + 2 + strlen(command));
                strcpy(checkCpy, check);
                strcat(checkCpy, "/");
                strcat(checkCpy, command);
                int checkAccess = access(checkCpy, X_OK);
                if(checkAccess == 0){
                    free(path);
                    return checkCpy; //returning our new path string for externalCommands() to use
                }
                free(checkCpy);
            }
        }
    }
    free(path);
    return NULL; //return null if nothing is found or we could not create a path
}