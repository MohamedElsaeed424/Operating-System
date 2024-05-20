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
MUTEX userInputMutex ;
MUTEX userOutputMutex ;
MUTEX fileMutex ;
BlockedQueue blockedQueue;
char executionOrder[150];

char* itoaa(int num) {
    static char str[12]; // Maximum number of digits for an int
    sprintf(str, "%d", num);
    return str;
}

enum state execute(Process *process){
    int lowerBound = process->pcb->memoryLowerBoundary;
    char copy[100]; // Note: important for strtok because strtok modifies string given
    strcpy(copy, memory->words[process->pcb->pc].value);
    incPC(process->pcb, memory);
    process->remaining_time--;
    printf("🔨 process %d is currently executing: %s 🔨\n", process->pcb->processID, copy);

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
            changeState(unblocked->pcb, memory, "Ready");
            dequeueBlocked(&blockedQueue, unblocked);
        }
    }
    return Success;
}

void loadAndEnqueueProgram(const char* filePath, int id) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB))  ;
    Process* process = (Process*)malloc(sizeof(Process))  ;
    if (pcb == NULL || process == NULL) {
        printf("Failed to allocate memory for PCB.\n");
        return;
    }
    int CODESIZE = programSize(filePath);
    int upperBoundary =memory->count + PCB_VALS+VAR_VALS+CODESIZE ;
//    initPCB(pcb, processID++,memory->count ,upperBoundary );
    initPCB(pcb, id,memory->count ,upperBoundary );
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
    init_BlockedQueue(&blockedQueue);
    init_mutex(&userInputMutex, "userInput");
    init_mutex(&userOutputMutex, "userOutput");
    init_mutex(&fileMutex, "file");
}

void terminate(){
    free(memory);
}
int main() {
    init();
    printf("----------------⚙️ Before Execution️ ⚒️----------------\n");
    int arrival_time1, arrival_time2, arrival_time3;

    printf("Arrival time 1:");
    scanf("%d", &arrival_time1);

    printf("Arrival time 2:");
    scanf("%d", &arrival_time2);

    printf("Arrival time 3:");
    scanf("%d", &arrival_time3);
    int min = arrival_time1 <= arrival_time2? arrival_time1: arrival_time2;
    min = min <= arrival_time3? min : arrival_time3;
//    loadAndEnqueueProgram("All_Programs/Program_1");
//    loadAndEnqueueProgram("All_Programs/Program_2");
//    loadAndEnqueueProgram("All_Programs/Program_3");
    int clock = min;
    Process *curr = NULL;
    bool blocked = false;
    do{
        printf("\n");
        printf("-----⏰⏱️🕧 clock: %d ⏲️🕰️🕛-----\n", clock);
        printf("\n");
        if(clock == arrival_time1)
            loadAndEnqueueProgram("All_Programs/Program_1", 1);
        if(clock == arrival_time2)
            loadAndEnqueueProgram("All_Programs/Program_2", 2);
        if(clock == arrival_time3)
            loadAndEnqueueProgram("All_Programs/Program_3", 3);

        // if there was a process running, and it isn't still running
        if(curr != NULL && !(curr->remaining_time > 0 && !blocked)){
        // if last running process was blocked put it in blocked queue
            if(blocked){
                changeState(curr->pcb, memory, "Blocked");
                enqueueBlocked(&blockedQueue, curr);
            }
            else{
                // else enqueue in ready queue
                changeState(curr->pcb, memory, "Ready");
                enqueueML(curr->pcb->currentPriority-1, curr);
            }
        }


        if(curr == NULL || strcmp(curr->pcb->processState, "Running") != 0)
            curr = dequeueML(memory);
        if(curr == NULL){
            clock++;
            continue;
        }

        blocked = false;
        //Execution of process
        printf("\n");
        printf("🏃‍♂️ Current running process: %d 🏃\n", curr->pcb->processID);
        int state = execute(curr);
        printf("\n");
        blocked = state == Blocked;

        executionOrder[clock] = (char)curr->pcb->processID;
        clock++;

        // if process finished free it and don't enqueue
        if(curr->pcb->pc > curr->pcb->memoryUpperBoundary){
            changeState(curr->pcb, memory, "Finished");
            printf("🏁 process %d has finished everything 🏁\n", curr->pcb->processID);
            free(curr->pcb);
            free(curr);
            curr = NULL;
            printMemory(memory);
            continue;
        }
        // if quantum was finished
        if(curr->remaining_time <= 0){
            incPriority(curr->pcb, memory);
        }

        printMemory(memory);

    }while(!isAllEmpty() || (curr != NULL && curr->remaining_time > 0) || clock <= arrival_time1 || clock <= arrival_time2 || clock <= arrival_time3);
    printf("----------------⚙️After Execution⚒️----------------\n") ;
    printMemory(memory);
    printf("📃Order of Execution:🛞\n");
    for(int i = 0; i<150; i++)
        printf("%d | ", executionOrder[i]);

    terminate();
    return 0;
}
enum state print(char* token, int lowerBound){
    token = strtok(NULL, " ");
    int varIdx = findVar(memory, token, lowerBound+TO_VAR);
    if(varIdx == -1){
        printf("Unknown Variable name %s\n", token);
        return Failed;
    }
    printf("%s\n", memory->words[varIdx].value);
    return Success;
}
