#ifndef SUDOKU_FILE_H
#define SUDOKU_FILE_H

#endif //SUDOKU_FILE_H

#define SIZE 9
#define ERROR_FILE_MSG "Nao foi possivel abrir o arquivo!\n"

FILE* createBinaryFile(char[SIZE][SIZE]);
void saveGame(FILE*, char[SIZE][SIZE]);
int readMovementsNumber(FILE*);