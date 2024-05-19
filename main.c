#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Process.h"
#include "fileController.h"
#include "Memory.h"
#include "Mutex.h"
#include "Queue.h"
#define PCB_VALS 6
#define VAR_VALS 3
#define TO_CODE 9
#define TO_VAR 6
#define CODE_VALS 9

enum state print(char* token, int lowerBound);

Memory* memory ;
int processID = 1;
MUTEX userInputMutex = {1};
MUTEX userOutputMutex ;
MUTEX fileMutex ;

char* itoaa(int num) {
    static char str[12]; // Maximum number of digits for an int
    sprintf(str, "%d", num);
    return str;
}

enum state execute(Process *process){
    int lowerBound = process->pcb->memoryLowerBoundary;
    char copy[100];
    strcpy(copy, memory->words[process->pcb->pc].value);
    incPC(process->pcb, memory);
    process->remaining_time--;


    char* token = strtok(copy, " ");
    if(strcmp(token, "print") == 0){
        return print(token, lowerBound);
    }
    else if(strcmp(token, "assign") == 0){
        token = strtok(NULL, " ");

        int varIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(varIdx == -1){
            varIdx = allocVar(memory, token, lowerBound + TO_VAR);
        }

        token = strtok(NULL, " ");
        char value[100];
//        memset(value,0, sizeof(value));
        // if input
        if(strcmp(token, "input") == 0){
            printf("Please enter a value:\n");
            scanf("%s", value);
        }
        else if(strcmp(token, "readFile") == 0){
        // if reading file
            token = strtok(NULL, " ");
            int pathIdx = findVar(memory, token, lowerBound + TO_VAR);
            if(pathIdx == -1){
                printf("Unknown Variable name %s", token);
                return Failed;
            }
            FILE* file = fopen(memory->words[pathIdx].value, "r");
            if(file == NULL){
                printf("File not Found\n");
                return Failed;
            }
            size_t size = fread(value, 1, sizeof(value), file);
            value[size] = '\0';
            fclose(file);
        }
        else{
        // if variable in memory
            int valueIdx = findVar(memory, token, lowerBound + TO_VAR);
            if(valueIdx == -1){
                printf("Unknown Variable name %s\n", token);
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
            printf("Unknown Variable name %s\n", token);
            return Failed;
        }

        FILE* file = fopen(memory->words[pathIdx].value, "w");

        token = strtok(NULL, " ");
        int valIdx = findVar(memory, token, lowerBound+TO_VAR);
        if(valIdx == -1){
            printf("Unknown Variable name %s\n", token);
            return Failed;
        }
        fprintf(file, "%s", memory->words[valIdx].value);
        fclose(file);
    }
    else if(strcmp(token, "printFromTo") == 0){
        token = strtok(NULL, " ");
        int lower = findVar(memory, token, lowerBound+TO_VAR);
        if(lower == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        lower = atoi(memory->words[lower].value);

        token = strtok(NULL, " ");
        int upper = findVar(memory, token, lowerBound+TO_VAR);
        if(upper == -1){
            printf("Unknown Variable name %s", token);
            return Failed;
        }
        upper = atoi(memory->words[upper].value);
        for(; lower <= upper; lower++)
            printf("%d ", lower);
        printf("\n");
    }
    else if(strcmp(token, "semWait") == 0){
        token = strtok(NULL, " ");
        if(strcmp(token, "userInput") == 0){
            return semWait(&userInputMutex, process);
        } else if(strcmp(token, "userOutput") == 0)
            return semWait(&userOutputMutex, process);
        else if(strcmp(token, "file") == 0)
            return semWait(&fileMutex, process);
        else{
            printf("Unknown Resource\n");
            return Failed;
        }
        // and enqueue into general blocked queue

    }
    else if(strcmp(token, "semSignal") == 0){
        token = strtok(NULL, " ");
        Process *unblocked;
        if(strcmp(token, "userInput") == 0){
            unblocked = semSignal(&userInputMutex, process);
        } else if(strcmp(token, "userOutput") == 0)
            unblocked = semSignal(&userOutputMutex, process);
        else if(strcmp(token, "file") == 0)
            unblocked = semSignal(&fileMutex, process);
        else{
            printf("Unknown Resource\n");
            return Failed;
        }
        if(unblocked != NULL){
            enqueueML(unblocked->pcb->currentPriority-1, unblocked);
            // and remove from general blocked queue
        }
    }
    return Success;
}

void loadAndExecuteProgram(const char* filePath) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB))  ;
    Process* process = (Process*)malloc(sizeof(Process))  ;
    if (pcb == NULL) {
        printf("Failed to allocate memory for PCB.\n");
        return;
    }
    int CODESIZE = programSize(filePath);
    int upperBoundary =memory->count + PCB_VALS+VAR_VALS+CODESIZE ;
    initPCB(pcb, processID++,memory->count ,upperBoundary );
    process->pcb = pcb;
    process->remaining_time = CODESIZE ;
    addWord(memory, "processID", itoaa(pcb->processID));
    addWord(memory, "processState", pcb->processState);
    addWord(memory, "currentPriority", itoaa(pcb->currentPriority));
    addWord(memory, "programCounter", itoaa(pcb->pc));
    addWord(memory, "memoryLowerBoundary", itoaa(pcb->memoryLowerBoundary));
    addWord(memory, "memoryUpperBoundary", itoaa(pcb->memoryUpperBoundary));
    addWord(memory , "" ,"0");
    addWord(memory , "" ,"0");
    addWord(memory , "" ,"0");
    loadProgramFile(memory,memory->count, filePath);
    enqueueML(0, process);
//    while(process->pcb->pc <= process->pcb->memoryUpperBoundary)
//        execute(process);
//    free(pcb);
}

void init(){
    initMem(&memory);
    initQueue();
    init_mutex(&userInputMutex);
    init_mutex(&userOutputMutex);
    init_mutex(&fileMutex);
}

void terminate(){
    free(memory);
}
int clock = 0;
int main() {
    init();
    // TODO: Use arrival times
//    int arrival_time[2];
//    for(int i = 0; i<3; i++){
//        printf("Arrival time %d:", i+1);
//        scanf("%d", &arrival_time[i]);
//    }

    loadAndExecuteProgram("All_Programs/Program_1");
    loadAndExecuteProgram("All_Programs/Program_2");
    loadAndExecuteProgram("All_Programs/Program_3");
    while(!isAllEmpty()){
        Process *curr = dequeueML(memory);
        bool blocked = false;
        while(curr->remaining_time){
            int state = execute(curr);
            if(state == Blocked){
                blocked = true;
                break;
            }
        }
        // if process finished free it and don't enqueue
        if(curr->pcb->pc > curr->pcb->memoryUpperBoundary){
            free(curr->pcb);
            free(curr);
            continue;
        }
        // if quantum was finished
        if(curr->remaining_time <= 0){
            incPriority(curr->pcb, memory);
        }
        // if it is not blocked put it in ready queue
        // TODO: change state to Ready
        // TODO: make general blocked queue
        if(!blocked)
            enqueueML(curr->pcb->currentPriority-1, curr);
        // else enqueue in blocked queue
    }
    printMemory(memory);
    terminate();
    return 0;
}
enum state print(char* token, int lowerBound){
    token = strtok(NULL, " ");
    int varIdx = findVar(memory, token, lowerBound+TO_VAR);
    if(varIdx == -1){
        printf("Unknown Variable name %s", token);
        return Failed;
    }
    printf("%s\n", memory->words[varIdx].value);
    return Success;
}
