#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int readChoice() {
    int choice;

    printf("Opcao: ");
    scanf("%d", &choice);

    return choice;
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
