#ifndef MSTWOOS_FILECONTROLLER_H
#define MSTWOOS_FILECONTROLLER_H
#define LINE_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Memory.h"

int programSize(const char* filePath){
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open program file.\n");
        return -1;
    }
    char code[LINE_SIZE];
    int i = 0;
    while(fgets(code, LINE_SIZE, file)) i++;
    return i-1;
}
int loadProgramFile(Memory* mem, int start, const char* filePath){
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open program file.\n");
        return -1;
    }
    char code[LINE_SIZE];
    int i = 1;
    while(fgets(code, LINE_SIZE, file)){
        strtok(code, "\n");
        sprintf(mem->words[start+i-1].name, "Instruction %d", i);
        addWord(mem, mem->words[start+i-1].name, code);
        i++;
    }
    return start-1;
}


#endif //MSTWOOS_FILECONTROLLER_H
