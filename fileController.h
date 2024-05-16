#ifndef MSTWOOS_FILECONTROLLER_H
#define MSTWOOS_FILECONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read the program file
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

// Function to tokenize the program
char** tokenizeProgram(char* program, int* numTokens) {
    const char* delimiters = " \t\n"; // Whitespace characters
    char* token;
    char* copy = strdup(program);

    // Count the number of tokens
    *numTokens = 0;
    token = strtok(copy, delimiters);
    while (token != NULL) {
        (*numTokens)++;
        token = strtok(NULL, delimiters);
    }

    // Allocate memory for the tokens
    char** tokens = (char**)malloc((*numTokens) * sizeof(char*));
    if (tokens == NULL) {
        printf("Memory allocation failed.\n");
        free(copy);
        return NULL;
    }

    // Tokenize the program
    int i = 0;
    token = strtok(program, delimiters);
    while (token != NULL) {
        tokens[i] = strdup(token);
        i++;
        token = strtok(NULL, delimiters);
    }

    free(copy);
    return tokens;
}

// Function to free the memory allocated for tokens
void freeTokens(char** tokens, int numTokens) {
    for (int i = 0; i < numTokens; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

// Function to parse and interpret the program
void interpretProgram(char** tokens, int numTokens) {
    // Implement interpretation of the program
    // Traverse the tokens and execute the corresponding actions
    // Handle expressions, statements, control flow, variables, etc.
    printf("Interpreting the program...\n");
    for (int i = 0; i < numTokens; i++) {
        printf("Token %d: %s\n", i, tokens[i]);
    }
}

// Function to execute the program
void executeProgram(const char* filePath) {
    // Read the program file
    char* program = readProgramFile(filePath);
    if (program == NULL) {
        return;
    }

    // Tokenize the program
    int numTokens;
    char** tokens = tokenizeProgram(program, &numTokens);
    free(program);
    if (tokens == NULL) {
        return;
    }

    // Interpret the program
    interpretProgram(tokens, numTokens);

    // Free the memory allocated for tokens
    freeTokens(tokens, numTokens);
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
