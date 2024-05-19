#ifndef MSTWOOS_MEMORY_H
#define MSTWOOS_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCB.h"

#define MAX_WORDS 60
#define MAX_PROGRAM_LINES 100
#define MAX_VARIABLES 3
#define PCB_SIZE 6

typedef struct {
    char name[100];
    char value[100];
} Word;

typedef struct {
    Word words[MAX_WORDS];
    int count;
} Memory;

int findVar(Memory *mem, const char* name, int varIdx){
    for(int i = 0; i<3; i++){
        if(strcmp(name, mem->words[varIdx + i].name) == 0)
            return varIdx+i;
    }
    return -1;
}
int findEmptyVar(Memory *mem, const char* name, int varIdx){
    for(int i = 0; i<3; i++){
        if(strlen(name) == 0)
            return varIdx+i;
    }
    return -1;
}


void initMem(Memory **mem){
    *mem = calloc( 1,sizeof(Memory)) ;
    (*mem)->count = 0;
}

void addWord(Memory* memory, char* name, char* value) {
    if (memory->count < MAX_WORDS) {
            strncpy(memory->words[memory->count].name, name, 20);
            strncpy(memory->words[memory->count].value, value, 20);
        memory->count++;
    } else {
        printf("Memory is full. Cannot add more words.\n");
    }
}

void removeWord(Memory* memory, char* name) {
    int found = 0;
    for (int i = 0; i < memory->count; i++) {
        if (strcmp(memory->words[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < memory->count - 1; j++) {
                memory->words[j] = memory->words[j + 1];
            }
            memory->count--;
            break;
        }
    }
    if (!found) {
        printf("Word not found.\n");
    }
}

void updateWord(Memory* memory, char* name, char* value) {
    int found = 0;
    for (int i = 0; i < memory->count; i++) {
        if (strcmp(memory->words[i].name, name) == 0) {
            strncpy(memory->words[i].value, value, 20);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Word not found.\n");
    }
}

void printMemory(Memory* memory) {
    for (int i = 0; i < memory->count; i++) {
        printf("Location: %d -> %s,%s\n", i ,memory->words[i].name, memory->words[i].value);
    }
}
#endif //MSTWOOS_MEMORY_H
