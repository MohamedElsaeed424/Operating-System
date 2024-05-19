//#define _OPEN_SYS_ITOA_EXT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "PCB.h"
#include "fileController.h"
#include "Memory.h"
#define PCB_VALS 6
#define VAR_VALS 3
#define TO_CODE 9
#define TO_VAR 6
#define CODE_VALS 9
enum state {Failed = -1, Good = 0, Blocked = 1};
Memory* memory ;
int processID = 1;

pthread_mutex_t userInputMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t userOutputMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

char* itoa(int num) {
    static char str[12]; // Maximum number of digits for an int
    sprintf(str, "%d", num);
    return str;
}

void semWait(pthread_mutex_t* mutex) {
    pthread_mutex_lock(mutex);
}

void semSignal(pthread_mutex_t* mutex) {
    pthread_mutex_unlock(mutex);
}
enum state execute(int lowerBound){
    int index = lowerBound + TO_CODE;
    char copy[100];
    strcpy(copy, memory->words[index].value);

    char* token = strtok(copy, " ");
    if(strcmp(token, "print") == 0){
        token = strtok(NULL, " ");
        int varIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(varIdx == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        printf("%s", memory->words[varIdx].value);
    }
    else if(strcmp(token, "assign") == 0){
        token = strtok(NULL, " ");

        int varIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(varIdx == -1){
            varIdx = findEmptyVar(memory, token, lowerBound + TO_VAR);
        }
        strcpy(memory->words[varIdx].name, token);
        token = strtok(NULL, " ");
        char value[100];
        // if input
        if(strcmp(token, "input") == 0){
            printf("Please enter a value:\n");
            scanf("%s", value);
        }
        // if reading file
        else if(strcmp(token, "readFile") == 0){
            token = strtok(NULL, " ");
            int pathIdx = findVar(memory, token, lowerBound + TO_VAR);
            if(pathIdx == -1){
                printf("Unknown Variable name %s", token);
                return Failed;
            }
            FILE* file = fopen(memory->words[pathIdx].value, "r");
            fread(value, sizeof(value), 1, file);
            fclose(file);
        }
        // if variable in memory
        else{
            int valueIdx = findVar(memory, token, lowerBound + TO_VAR);
            if(valueIdx == -1){
                printf("Unknown Variable name %s", token);
                return Failed;
            }
            strcpy(value, memory->words[valueIdx].value);
        }

        strcpy(memory->words[varIdx].value, value);
    }
    else if(strcmp(token, "writeFile") == 0){
        token = strtok(NULL, " ");
        int pathIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(pathIdx == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }

        FILE* file = fopen(memory->words[pathIdx].value, "w");

        token = strtok(NULL, " ");
        int valIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(valIdx == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        fprintf(file, "%s", memory->words[valIdx].value);
        fclose(file);
    }
    else if(strcmp(token, "printFromTo") == 0){
        int lower = findVar(memory, token, lowerBound+TO_VAR);
        if(lower == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        lower = atoi(memory->words[lower].value);

        int upper = findVar(memory, token, lowerBound+TO_VAR);
        if(upper == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        upper = atoi(memory->words[upper].value);
        for(; lower <= upper; lower++)
            printf("%d ", lower);
    }
    else if(strcmp(token, "semWait") == 0){
        token = strtok(NULL, " ");

    }
    return 0;
}
void executeLine(char** tokens, int* index) {
    if (strcmp(tokens[*index], "print") == 0) {
        (*index)++;

        printf("%s\n", tokens[*index]);
    } else if (strcmp(tokens[*index], "assign") == 0) {
        (*index)++;
        char* variable = tokens[*index];
        (*index)++;
        if (strcmp(tokens[*index], "input") == 0) {
//            semWait(&userInputMutex);
            printf("Please enter a value for %s: ", variable);
            char inputValue[20];
            scanf("%s", inputValue);
//            semSignal(&userInputMutex);
            addWord(memory, variable, inputValue);
        } else {
            addWord(memory, variable, tokens[*index]);
        }
    } else if (strcmp(tokens[*index], "writeFile") == 0) {
        (*index)++;
        char* filename = tokens[*index];
        (*index)++;
        char* data = tokens[*index];
//        semWait(&fileMutex);
        FILE* file = fopen(filename, "a");
        if (file != NULL) {
            fprintf(file, "%s\n", data);
            fclose(file);
        }
//        semSignal(&fileMutex);
    } else if (strcmp(tokens[*index], "readFile") == 0) {
        (*index)++;
        char* filename = tokens[*index];
//        semWait(&fileMutex);
        FILE* file = fopen(filename, "r");
        if (file != NULL) {
            char buffer[20];
            if (fscanf(file, "%s", buffer) != EOF) {
                printf("Read from file %s: %s\n", filename, buffer);
                addWord(memory, filename, buffer);
            }
            fclose(file);
        }
//        semSignal(&fileMutex);
    } else if (strcmp(tokens[*index], "printFromTo") == 0) {
        (*index)++;
        int start = atoi(tokens[*index]);
        (*index)++;
        int end = atoi(tokens[*index]);
        printf("%d",end);
        for (int i = start; i <= end; i++) {
            printf("%d ", i);
        }
        printf("\n");
    } else if (strcmp(tokens[*index], "semWait") == 0) {
        (*index)++;
        if (strcmp(tokens[*index], "userInput") == 0) {
            semWait(&userInputMutex);
        } else if (strcmp(tokens[*index], "userOutput") == 0) {
            semWait(&userOutputMutex);
        } else if (strcmp(tokens[*index], "file") == 0) {
            semWait(&fileMutex);
        }
    } else if (strcmp(tokens[*index], "semSignal") == 0) {
        (*index)++;
        if (strcmp(tokens[*index], "userInput") == 0) {
            semSignal(&userInputMutex);
        } else if (strcmp(tokens[*index], "userOutput") == 0) {
            semSignal(&userOutputMutex);
        } else if (strcmp(tokens[*index], "file") == 0) {
            semSignal(&fileMutex);
        }
    }
}

void interpretProgram(char** tokens, int numTokens) {
    printf("Interpreting the program...\n");
    int i ;
    for (i = 0; i < numTokens; i++) {
        executeLine(tokens, &i);
    }
}

void loadAndExecuteProgram(const char* filePath) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB))  ;
    Process* process = (Process*)malloc(sizeof(Process))  ;
    if (pcb == NULL) {
        printf("Failed to allocate memory for PCB.\n");
        return;
    }
    int CODESIZE = (filePath == "All_Programs/Program_1" ||filePath == "All_Programs/Program_2") ? 7 : 9 ;    // layethha 👌
    int upperBoundary =memory->count + PCB_VALS+VAR_VALS+CODESIZE ;
    initPCB(pcb, 0,memory->count ,upperBoundary );
    addWord(memory, "processID", itoa(pcb->processID));
    addWord(memory, "processState", pcb->processState);
    addWord(memory, "currentPriority", itoa(pcb->currentPriority));
    addWord(memory, "programCounter", itoa(pcb->programCounter));
    addWord(memory, "memoryLowerBoundary", itoa(pcb->memoryLowerBoundary));
    addWord(memory, "memoryUpperBoundary", itoa(pcb->memoryUpperBoundary));
    process->pcb = pcb;
    printf("-------PCB process %d------------------\n" , pcb->processID);
    printPCB(pcb);
    printf("--------------------------\n");
    addWord(memory , "a" , "0");
    addWord(memory , "b" , "0");
    addWord(memory , "c" , "0");
    char* program = readProgramFile(filePath);
    if (program == NULL) {
        free(pcb);
        return;
    }
    int numTokens;
    char** tokens = tokenizeProgram(program, &numTokens);
    free(program);
    if (tokens == NULL) {
        free(pcb);
        return;
    }
    interpretProgram(tokens, numTokens);
    freeTokens(tokens, numTokens);
    free(pcb);
}

void init(){
    initMem(&memory);
}

void terminate(){
    free(memory);
}

int main() {
    init();
//    loadAndExecuteProgram("All_Programs/Program_1");
//    loadAndExecuteProgram("All_Programs/Program_2");
    loadAndExecuteProgram("All_Programs/Program_1");
//    printf("----------Memory----------\n");
    printMemory(memory);
    return 0;
}
