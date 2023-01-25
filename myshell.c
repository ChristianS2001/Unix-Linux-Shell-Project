/*
    Christian Smith, tuk96703@temple.edu, TUID: 915789291, Purpose of this file:
        The purpose of this file is to build, run, and work the terminal I create.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "internal.h"
#include "external.h"
#include <limits.h>

//todo make sure to add all neccessary comments for this project, write and organize documentation, also bug test extensivly

//creating our method to run our loop that is our shell
int main (int argc, char* argv[]){ //this has a argument to check for batch file
    char error_message[30] = "An error has occurred\n";
    char *line = NULL;
    FILE *stream = stdin; //setting stream var to stdin file
    size_t len = 0; 
    char* shell = realpath(argv[0], NULL);
    setenv("SHELL", shell, 1);
    int inAFile = 0;
    int exit = 0;
    ssize_t size = 0; //specific var created for allowing shell to work similar to unix shell by if statement scoping
    char* path;
    path = malloc(PATH_MAX); //allocating the memory
    
    if(argc == 1){
        //read from stdin
    } else if (argc == 2) {
        //read from file (to figure out if we need to do batchFile functionality)
        inAFile = 1;
        stream = fopen(argv[1], "rb");
    } else { //edge case
        write(STDERR_FILENO, error_message, strlen(error_message)); //printing error since we have to many args
    }

    while(exit != 1){
        //start our shell
        if(inAFile == 0){
            //we are reading from user input not a batch file
            getcwd(path, PATH_MAX);
            printf("myShell-(%s)>", path);
        }
        size = getline(&line, &len, stream);//getting our stdin and *line becomes the string of input
        if (size > 1){ //if there is something at all typed we can run the rest
            //removing the \n char so we can check for "exit" and "quit"
            int fixedLineSize = strlen(line) - 1;
            if(line[fixedLineSize] == '\n'){
                line[fixedLineSize] = '\0';
            }

            Count commands = parseInput(line); //obtaining our array of multiple CMDstructs within another struct to use for shell functionality
            int pipeArray[2] = {-1, -1}; //initializing pipeArray for pipe() function
            for(int i = 0; i < commands.count; i++){
                FILE *writeFile = stdout;
                FILE *readFile = stdin;
                if(pipeArray[0] != -1){ //edge case to make sure we read on first file when piping
                    readFile = fdopen(pipeArray[0], "rb");
                }
                if(commands.cmdLineArray[i].pipeFlag != 0){//piping functionality done here in tandem with lines: 36-42 in external.c where we support redirection already
                    pipe(pipeArray);
                    writeFile = fdopen(pipeArray[1], "wb"); //to make sure we write on next file
                }else{ //edge case for properly implementing piping functionality and fixing these open elements to not confuse previous piping functionality with potential upcoming ones
                    pipeArray[1] = -1;
                    pipeArray[0] = -1;
                }

                if(commands.cmdLineArray[i].errorFlag != 0){ //edge case for if we have errors regarding parsing
                    fprintf(stderr, "An error has occurred\n"); //error flag is up do not continue
                    continue;
                }
                if(commands.cmdLineArray[i].outputFlag == 1){ //create re direction variable that will write in a file or delete and start writing if that file exists, and create the file if it doesn't exist
                    writeFile = fopen(commands.cmdLineArray[i].outputFile, "wb");
                    if(writeFile == NULL){
                        fprintf(stderr, "An error has occurred\n");
                    }
                }
                if(commands.cmdLineArray[i].doubleOutputFlag == 1){ //create re direction variable that will append to end of a file, and create the file if it doesn't exist
                    writeFile = fopen(commands.cmdLineArray[i].outputFile, "ab");
                    if(writeFile == NULL){
                        fprintf(stderr, "An error has occurred\n");
                    }
                }
                if(commands.cmdLineArray[i].inputFlag == 1){ //checking input flag for potential redirection
                    readFile = fopen(commands.cmdLineArray[i].inputFile, "rb");
                    if(readFile == NULL){
                        fprintf(stderr, "An error has occurred\n");
                    }
                }
                //checking for exit or quit functionality in myshell
                if(strcmp(commands.cmdLineArray[i].parseArray[0], "exit") == 0 || strcmp(commands.cmdLineArray[i].parseArray[0], "quit") == 0){
                    exit = 1;
                } else {
                    if(checkForCMD(commands.cmdLineArray[i].parseArray) == 1){
                        callSpecificFunc(commands.cmdLineArray[i], writeFile, readFile); //running potential internal commands
                    } else if (checkForCMD(commands.cmdLineArray[i].parseArray) == 0){
                        externalCommands(commands.cmdLineArray[i], shell, writeFile, readFile); //running external commands
                    }
                }
                free(commands.cmdLineArray[i].inputFile);
                free(commands.cmdLineArray[i].outputFile);
                for(int j = 0; j < commands.cmdLineArray[i].count; j++){
                    free(commands.cmdLineArray[i].parseArray[j]);
                }
                free(commands.cmdLineArray[i].parseArray);
                if(writeFile != stdout){ //edge case for writing
                    fclose(writeFile);
                }
                if(readFile != stdin){ //edge case for reading
                    fclose(readFile);
                }
            }
            free(commands.cmdLineArray);
        } else if (size == -1){ //for batchFile functionality
            exit = 1;
        }
    }
    inAFile = 0;
    //clearing up potential memory leaks
    fclose(stream);
    free(line);
    free(shell);
    free(path);
}