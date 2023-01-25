#ifndef INTERNAL_H
#define INTERNAL_H

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
int checkForCMD(char** parseArray);
void cd (CMDstruct cmdLine);
void callSpecificFunc(CMDstruct funcName, FILE* writeFile, FILE* readFile);
void clr(CMDstruct cmdLine);
void dirCMD(const char* dirname, int indents, const char* path);
int callDir(int count, char* arguments[]);
void environment();
void echo(CMDstruct cmdLine, FILE* writeFile);
void help();
void pauseCMD();

#endif