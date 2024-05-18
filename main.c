//#define _OPEN_SYS_ITOA_EXT
#define TO_VAR 6
#define TO_CODE 9
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCB.h"
#include "fileController.h"

typedef struct {
    char *name;
    char *value;
}Pair;

Pair* memory[60]; /// all NULL

int findEmptyIndex(){
    for(int i = 0; i<60; i++){
        if(memory[i] == NULL)
            return i;
    }
    return -1;
}
int loadPCB(PCB* curr){
    return -1;
}
void loadCode(const char* filePath){
    char* program = readProgramFile(filePath);

}


int main(){
    int x = 123;
    char s[5];

    itoa(x,s, 10);
    s[0] = '5';
    printf("%s",s);
    return 0;
}
