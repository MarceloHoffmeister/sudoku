#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERROR_FILE_MSG "Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION "Opcao invalida! Tente novamente!"
#define SIZE 9

enum boolean { FALSE = 0, TRUE = 1 };

FILE* load(char[SIZE][SIZE]);
FILE* playSavedGame(char[SIZE][SIZE], char*);
void playNewGame(char[SIZE][SIZE], char*);
int whatSubMatrix(int, int);
FILE* createBinaryFile(char[SIZE][SIZE]);
int isValid(const char[SIZE][SIZE], int, int, int);
int isValidColumn(const char[SIZE][SIZE], int, int);
int isValidSubMatrix(const char[SIZE][SIZE], int, int, int);
int isValidLine(const char[SIZE][SIZE], int, int);
int hasEmptyFields(const char[SIZE][SIZE]);
void printBoard(const char[SIZE][SIZE], int);
void play();
void solve(FILE*, char[SIZE][SIZE]);
void solveNextPosition(char[SIZE][SIZE]);
void saveGame(FILE*, char[SIZE][SIZE]);
int fill(char[SIZE][SIZE], int, int, int);
char* askForFileName();
int readMovementsNumber(FILE*);

int fim_x(int);
int fim_y(int);
int readChoice();
void gen_random(char*, int);
int ini_x(int);
int ini_y(int);
void menu();
void fileMenu();

int main() {
	play();

	return 0;
}

FILE* load(char board[SIZE][SIZE]) {
	fileMenu();

	switch(readChoice()) {
		case 1:
            playNewGame(board, askForFileName());
			break;

		case 2:
            return playSavedGame(board, askForFileName());

		case 9:
			break;

		default:
            puts(INVALID_OPTION);
            break;
	}
}

char* askForFileName()
{
    char *gameName = (char*) malloc(sizeof(char) * 50);

    printf("Nome do arquivo a ser carregado: ");
    scanf("%s", gameName);

    return gameName;
}

FILE* playSavedGame(char board[SIZE][SIZE], char *fileName) {
    FILE *file;
    int n = 0;

    sprintf(fileName, "%s.dat", fileName);

    file = fopen(fileName, "rb+");

    if (file == NULL) printf(ERROR_FILE_MSG);

    fseek(file, sizeof(int)*((SIZE*SIZE)*-1), SEEK_END);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fread(&n, sizeof(int), 1, file);
            board[i][j] = n;
        }
    }

    return file;
}

void playNewGame(char board[SIZE][SIZE], char *fileName) {
	FILE *file;
    int n = 0;

    sprintf(fileName, "%s.txt", fileName);

    file = fopen(fileName, "r");

    if (file == NULL) printf(ERROR_FILE_MSG);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d ", &n);
            board[i][j] = n;
        }
    }

    fclose(file);
}

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

int whatSubMatrix(int x, int y) {
	if (x < 3 && y < 3)
		return 1;
	else if (x < 3 && y < 6)
		return 2;
	else if (x < 3 && y < 9)
		return 3;
	else if (x < 6 && y < 3)
		return 4;
	else if (x < 6 && y < 6)
		return 5;
	else if (x < 6 && y < 9)
		return 6;
	else if (x < 9 && y < 3)
		return 7;
	else if (x < 9 && y < 6)
		return 8;
	else
		return 9;
}

int isValid(const char board[SIZE][SIZE], int x, int y, int value) {
    if (value == 0) return TRUE;

	if (!isValidColumn(board, y, value)) return FALSE;
	if (!isValidLine(board, x, value)) return FALSE;
	if (!isValidSubMatrix(board, x, y, value)) return FALSE;

	return TRUE;
}

int isValidColumn(const char board[SIZE][SIZE], int y, int value) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][y] == value) return FALSE;
    }

    return TRUE;
}

int isValidLine(const char board[SIZE][SIZE], int x, int value) {
    for (int i = 0; i < SIZE; i++) {
        if (board[x][i] == value) return FALSE;
    }

    return TRUE;
}

int isValidSubMatrix(const char board[SIZE][SIZE], int x, int y, int value) {
    int subMatrix;

    subMatrix = whatSubMatrix(x, y);

    for (int i = ini_x(subMatrix); i <= fim_x(subMatrix); i++) {
        for (int j = ini_y(subMatrix); j <= fim_y(subMatrix); j++) {
            if (board[i][j] == value) return FALSE;
        }
    }

    return TRUE;
}

int hasEmptyFields(const char board[SIZE][SIZE]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j] == 0) {
                return TRUE;
            }
		}
	}

	return FALSE;
}

void printBoard(const char board[SIZE][SIZE], int numberOfTurns) {
	int i, j;

	puts("\n    1 2 3   4 5 6   7 8 9");
	for (i = 0; i < 9; i++) {
		if (i % 3 == 0)
			puts("  -------------------------");
		for (j = 0; j < 9; j++) {

			if (j == 0)
				printf("%d | ", i+1);
			else if (j % 3 == 0)
				printf("| ");

			if (board[i][j] == 0)
				printf("- ");
			else
				printf("%d ", board[i][j]);
		}
		puts("|");
	}
	puts("  -------------------------");

    printf("Numero de jogadas: %d\n", numberOfTurns);
}

void play() {
	int choice = 0;

	char board[9][9] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	FILE *file = NULL;

	while (choice != 9) {
		printBoard(board, readMovementsNumber(file));

		menu();
		choice = readChoice();

		switch(choice) {
            case 1:
                file = load(board);

                if (file == NULL) {
                    file = createBinaryFile(board);
                }
                break;

            case 2: {
                int x, y, value;

                printf("Entre com a posicao e o valor (linha, coluna, valor): ");
                scanf("%d %d %d", &x, &y, &value);

                if (isValid(board, x, y, value)) {
                    board[x][y] = value;
                    saveGame(file, board);
                } else {
                    puts("Valor ou posicao incorreta! Tente novamente!");
                }
                break;
            }

            case 3:
                if (file == NULL) {
                    file = createBinaryFile(board);
                }

                solveNextPosition(board);
                saveGame(file, board);
                puts("Um passo resolvido!");
                break;

            case 4:
                if (file == NULL) {
                    file = createBinaryFile(board);
                }

                solve(file, board);
                break;

            case 9:
                puts("Programa finalizado ..");
                break;

            default:
                puts(INVALID_OPTION);
                break;
		}
	}

    fclose(file);
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

void solve(FILE *fb, char board[SIZE][SIZE]) {
	while(hasEmptyFields(board)) {
        solveNextPosition(board);
        saveGame(fb, board);
	}
}

void solveNextPosition(char board[SIZE][SIZE]) {
    int found = 0;

    for (int i = 0; i < SIZE && !found; i++) {
        for (int j = 0; j < SIZE && !found; j++) {
            if (board[i][j] == 0) {
                found = fill(board, i, j, 1);
            }
        }
    }
}

int fill(char board[SIZE][SIZE], int i, int j, int value)
{
    if (value <= 9 && isValid(board, i, j, value)) {
        board[i][j] = value;

        return TRUE;
    } else {
        if (value < 9) {
            fill(board, i, j, ++value);
        } else {
            board[i][j] = 0;

            if (j == 0 && i != 0) {
                i -= 1;
                j = SIZE-1;
            } else {
                j -= 1;
                value = board[i][j]+1;
            }

            fill(board, i, j, value);
        }

        return TRUE;
    }
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

int fim_x(int subMatrix) {
    switch (subMatrix) {
        case 1:
        case 2:
        case 3:
            return 2;

        case 4:
        case 5:
        case 6:
            return 5;

        default:
            return 8;
    }
}

int fim_y(int subMatrix) {
	switch (subMatrix) {
		case 1:
		case 4:
		case 7:
			return 2;

		case 2:
		case 5:
		case 8:
			return 5;

		default:
			return 8;
	}
}

void gen_random(char *string, const int stringLength) {
	srand(time(NULL));

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

	int i;

    for (i = 0; i < stringLength; i++) {
        string[i] = alphanum[rand() % (sizeof(alphanum) - 1)];;
    }

    string[stringLength] = 0;
}

int ini_x(int subMatrix) {
	switch (subMatrix) {
		case 1:
		case 2:
		case 3:
            return 0;

		case 4:
		case 5:
		case 6:
			return 3;

		default:
			return 7;
	}
}

int ini_y(int subMatrix) {
	switch(subMatrix) {
		case 1:
		case 4:
		case 7:
			return 0;

		case 2:
		case 5:
		case 8:
			return 3;

		default:
			return 7;
	}
}

int readChoice() {
	int choice;

	printf("Opcao: ");
	scanf("%d", &choice);

	return choice;
}

void menu() {
	puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("[1] Carregar jogo");
	puts("[2] Jogar");
	puts("[3] Resolver um passo");
	puts("[4] Resolver");
	puts("[9] Finalizar");
	puts("--------");
}

void fileMenu() {
	puts("\n~~~~~ MENU ARQUIVO ~~~~~");
	puts("[1] Novo jogo");
	puts("[2] Continuar jogo");
	puts("[9] Retornar ao menu anterior");
	puts("--------");
}

