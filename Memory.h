#ifndef MSTWOOS_MEMORY_H
#define MSTWOOS_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    char value[20];
} Word;

typedef struct {
    Word words[60];
    int count; //  num of words in memory will help in add and remove
} Memory;

Memory *initMem() {
    Memory *memory = (Memory *)malloc(sizeof(Memory));
    if (memory != NULL) {
        memory->count = 0;
    }
    return memory;
}

void addWord(Memory *memory, char *name, char *value) {
    if (memory->count < 60) {
        strncpy(memory->words[memory->count].name, name, 20);
        strncpy(memory->words[memory->count].value, value, 20);
        memory->count++;
    } else {
        printf("Memory is full. Cannot add more words.\n");
    }
}

void removeWord(Memory *memory, char *name) {
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

void updateWord(Memory *memory, char *name, char *value) {
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

void printMemory(Memory *memory) {
    for (int i = 0; i < memory->count; i++) {
        printf("Name: %s, Value: %s\n", memory->words[i].name, memory->words[i].value);
    }
}



#endif //MSTWOOS_MEMORY_H
