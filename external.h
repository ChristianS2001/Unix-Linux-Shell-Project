#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "parser.h"
#include <stdio.h>
void externalCommands(CMDstruct cmdLine, char* shell, FILE* write, FILE* read);
char* find(char* command);

#endif