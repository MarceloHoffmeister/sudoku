#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "file.h"

FILE* createBinaryFile(char board[SIZE][SIZE]) {
    FILE *file;
    const int gameNameLength = 8;
    char *gameName = (char*) malloc(sizeof(char)*(gameNameLength+7));
    int n = 0;

    gen_random(gameName, gameNameLength);
    sprintf(gameName, "%s.dat", gameName);

    file = fopen(gameName, "wb+");

    if (file == NULL) puts(ERROR_FILE_MSG);

    fwrite(&n, sizeof(int), 1, file);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            n = board[i][j];
            fwrite(&n, sizeof(int), 1, file);
        }
    }

    return file;
}

void saveGame(FILE *file, char board[SIZE][SIZE]) {
    int n = 0, count = 0;

    fseek(file, 0, SEEK_SET);

    fread(&count, sizeof(int), 1, file);

    count++;

    fseek(file, 0, SEEK_SET);

    fwrite(&count, sizeof(int), 1, file);

    fseek(file, 0, SEEK_END);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            n = board[i][j];
            fwrite(&n, sizeof(int), 1, file);
        }
    }
}

int readMovementsNumber(FILE *file) {
    int count = 0;

    if (file != NULL) {
        fseek(file, 0, SEEK_SET);

        fread(&count, sizeof(int), 1, file);

        return count;
    }

    return 0;
}