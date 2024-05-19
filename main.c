
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Process.h"
#include "fileController.h"
#include "Memory.h"
#include "Mutex.h"


#define PCB_VALS 6
#define VAR_VALS 3
#define TO_CODE 9
#define TO_VAR 6
#define CODE_VALS 9

Memory* memory ;
int processID = 1;
MUTEX userInputMutex ;
MUTEX userOutputMutex ;
MUTEX fileMutex ;

char* itoaa(int num) {
    static char str[12]; // Maximum number of digits for an int
    sprintf(str, "%d", num);
    return str;
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

void loadAndExecuteProgram(const char* filePath) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB))  ;
    Process* process = (Process*)malloc(sizeof(Process))  ;
    if (pcb == NULL) {
        printf("Failed to allocate memory for PCB.\n");
        return;
    }
    int CODESIZE = (strcmp(filePath ,"All_Programs/Program_1")==0 || strcmp(filePath, "All_Programs/Program_2")) ? 7 : 9 ;    // layethha 👌
    int upperBoundary =memory->count + PCB_VALS+VAR_VALS+CODESIZE ;
    initPCB(pcb, processID++,memory->count ,upperBoundary );
    addWord(memory, "processID", itoaa(pcb->processID));
    addWord(memory, "processState", pcb->processState);
    addWord(memory, "currentPriority", itoaa(pcb->currentPriority));
    addWord(memory, "programCounter", itoaa(pcb->pc));
    addWord(memory, "memoryLowerBoundary", itoaa(pcb->memoryLowerBoundary));
    addWord(memory, "memoryUpperBoundary", itoaa(pcb->memoryUpperBoundary));
    process->pcb = pcb;
    process->remaining_time = CODESIZE ;
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
