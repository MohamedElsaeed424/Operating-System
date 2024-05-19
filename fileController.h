#ifndef MSTWOOS_FILECONTROLLER_H
#define MSTWOOS_FILECONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readProgramFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open program file.\n");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* program = (char*)malloc(fileSize + 1);
    if (program == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }
    fread(program, 1, fileSize, file);
    program[fileSize] = '\0';

    fclose(file);
    return program;
}

char** tokenizeProgram(char* program, int* numTokens) {
    const char* delimiters = " \t\n"; // Whitespace characters
    char* token;
    char* copy = strdup(program);

    *numTokens = 0;
    token = strtok(copy, delimiters);
    while (token != NULL) {
        (*numTokens)++;
        token = strtok(NULL, delimiters);
    }
    char** tokens = (char**)malloc((*numTokens) * sizeof(char*));
    if (tokens == NULL) {
        printf("Memory allocation failed.\n");
        free(copy);
        return NULL;
    }
    int i = 0;
    token = strtok(program, delimiters);
    while (token != NULL) {
        tokens[i] = strdup(token);
        i++;
        token = strtok(NULL, delimiters);
    }
//    printf("-------- Tokens --------\n");
//    for (int i = 0; i < *numTokens; i++) {
//        printf("%s\n", tokens[i]);
//    }
    free(copy);
    return tokens;
}

void freeTokens(char** tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

//// Example usage
//int main() {
//    char programFile[100];
//    printf("Enter the program file path: ");
//    scanf("%s", programFile);
//
//    executeProgram(programFile);
//
//    return 0;
//}

#endif //MSTWOOS_FILECONTROLLER_H
