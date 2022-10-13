#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERROR_FILE_MSG	"Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION	"Opcao invalida! Tente novamente!"
#define SIZE 9

enum boolean {
	FALSO=0, VERDADEIRO=1
};

/* Prototipos */
FILE* carregar(char[SIZE][SIZE]);
FILE* carregar_continuar_jogo (char[SIZE][SIZE], char*);
void carregar_novo_jogo(char[SIZE][SIZE], char*);
int determinar_quadrante(int, int);
FILE* criar_arquivo_binario(char[SIZE][SIZE]);
int e_valido(const char[SIZE][SIZE], int, int, int);
int e_valido_na_coluna(const char[SIZE][SIZE], int, int);
int e_valido_no_quadro3x3(const char[SIZE][SIZE], int, int, int);
int e_valido_na_linha(const char[SIZE][SIZE], int, int);
int existe_campo_vazio(const char[SIZE][SIZE]);
void imprimir(const char[SIZE][SIZE]);
void jogar();
void resolver(FILE*, char[SIZE][SIZE]);
void resolver_um_passo(char[SIZE][SIZE]);
void salvar_jogada_bin(FILE*, char[SIZE][SIZE]);
int fill(char[SIZE][SIZE], int, int, int);

/* Funcoes auxiliares */
int fim_x(int);
int fim_y(int);
int le_opcao();
void gen_random(char*, int);
int ini_x(int);
int ini_y(int);
void menu();
void menu_arquivo();


/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * MAIN
 * /////////////////////////////////////////////////////////////////////////////
 */
int main () {

	// inicia o jogo
	jogar();

	return 0;
}

/* -----------------------------------------------------------------------------
 * CARREGAR
 * Inicializa o SUDOKU a partir de um novo jogo ou estado de jogo anterior
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregar(char quadro[SIZE][SIZE]) {
	int opcao;
    char gameName[50];

	menu_arquivo();
	opcao = le_opcao();

	// TODO Função incompleta

	switch(opcao) {

		// carregar novo sudoku
		case 1:
            puts("Nome do arquivo a ser carregado: ");
            scanf("%s", gameName);
            carregar_novo_jogo(quadro, gameName);
			break;

		// continuar jogo
		case 2:
            puts("Nome do arquivo a ser carregado: ");
            scanf("%s", gameName);
            return carregar_continuar_jogo(quadro, gameName);

		// retornar ao menu anterior
		case 9:
			break;

		default:
            puts(INVALID_OPTION);
            break;
	}
}

/* -----------------------------------------------------------------------------
 * CARREGAR CONTINUAR JOGO
 * Carrega um estado de jogo a partir de um arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregar_continuar_jogo(char quadro[SIZE][SIZE], char *nome_arquivo) {
    FILE *file;
    int n = 0;

    sprintf(nome_arquivo, "%s.dat", nome_arquivo);

    file = fopen(nome_arquivo, "rb+");

    if (file == NULL) printf(ERROR_FILE_MSG);

    fseek(file, sizeof(int)*((SIZE*SIZE)*-1), SEEK_END);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fread(&n, sizeof(int), 1, file);
            quadro[i][j] = n;
        }
    }

    return file;
}

/* -----------------------------------------------------------------------------
 * CARREGAR NOVO JOGO
 * Carrega um novo jogo do Sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void carregar_novo_jogo(char quadro[SIZE][SIZE], char *nome_arquivo) {
	FILE *file;
    int n = 0;

    sprintf(nome_arquivo, "%s.txt", nome_arquivo);

    file = fopen(nome_arquivo, "r");

    if (file == NULL) printf(ERROR_FILE_MSG);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d ", &n);
            quadro[i][j] = n;
        }
    }

    fclose(file);
}

/* -----------------------------------------------------------------------------
 * CRIAR ARQUIVO BINARIO
 * Criar arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* criar_arquivo_binario(char quadro[SIZE][SIZE]) {
	FILE *file;
    const int gameNameLength = 8;
    char *gameName = (char*) malloc(sizeof(char)*(gameNameLength+7));
    int n = 0;

    gen_random(gameName, gameNameLength);
    sprintf(gameName, "%s.dat", gameName);

    file = fopen(gameName, "wb+");

    if (file == NULL) puts(ERROR_FILE_MSG);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            n = quadro[i][j];
            fwrite(&n, sizeof(int), 1, file);
        }
    }

    return file;
}

/* -----------------------------------------------------------------------------
 * DETERMINAR QUADRANTE
 * Dado as posicoes x e y, determina o quadrante do quadro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int determinar_quadrante(int x, int y) {
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

/* -----------------------------------------------------------------------------
 * E VALIDO
 * Determina se um valor na posicao x e y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido(const char quadro[SIZE][SIZE], int x, int y, int valor) {
    if (valor == 0) return VERDADEIRO;

	// verifica as tres condicoes
	if (!e_valido_na_coluna(quadro, y, valor)) return FALSO;
	if (!e_valido_na_linha(quadro, x, valor)) return FALSO;
	if (!e_valido_no_quadro3x3(quadro, x, y, valor)) return FALSO;

	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA COLUNA
 * Verifica se um valor na coluna y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_coluna(const char quadro[SIZE][SIZE], int y, int valor) {
    for (int i = 0; i < SIZE; i++) {
        if (quadro[i][y] == valor) return FALSO;
    }

    return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA LINHA
 * Verifica se um valor na linha x e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_na_linha(const char quadro[SIZE][SIZE], int x, int valor) {
    for (int i = 0; i < SIZE; i++) {
        if (quadro[x][i] == valor) return FALSO;
    }

    return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NO QUADRO 3X3
 * Verifica se um valor e valido no quadrante da posicao x, y
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int e_valido_no_quadro3x3(const char quadro[SIZE][SIZE], int x, int y, int valor) {
    int subMatriz;

    subMatriz = determinar_quadrante(x, y);

    for (int i = ini_x(subMatriz); i < fim_x(subMatriz); i++) {
        for (int j = ini_y(subMatriz); j < fim_y(subMatriz); j++) {
            if (quadro[i][j] == valor) return FALSO;
        }
    }

    return VERDADEIRO;
}



/* -----------------------------------------------------------------------------
 * EXISTE CAMPO VAZIO
 * Verifica se existe um campo nao preenchido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int existe_campo_vazio (const char quadro[SIZE][SIZE]) {
	int i, j;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (quadro[i][j] == 0)
				return VERDADEIRO;
		}
	}

	return FALSO;
}

/* -----------------------------------------------------------------------------
 * IMPRIMIR
 * Imprime o quadro inteiro do sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void imprimir (const char quadro[SIZE][SIZE]) {
	int i, j;

//	puts("~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("    1 2 3   4 5 6   7 8 9");
	for (i = 0; i < 9; i++) {
		if (i % 3 == 0)
			puts("  -------------------------");
		for (j = 0; j < 9; j++) {

			if (j == 0)
				printf("%d | ", i+1);
			else if (j % 3 == 0)
				printf("| ");

			if (quadro[i][j] == 0)
				printf("- ");
			else
				printf("%d ", quadro[i][j]);
		}
		puts("|");
	}
	puts("  -------------------------");
}

/* -----------------------------------------------------------------------------
 * JOGAR
 * Realiza toda a logica do jogo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void jogar() {
	int opcao;
	char quadro[9][9] = {
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
	FILE *fb = NULL;

	opcao = 0;

	while (opcao != 9) {
		// imprime na tela o quadro atual
		imprimir(quadro);

		// apresenta um menu com as opcoes
		menu();
		opcao = le_opcao();

		switch(opcao) {

		// carregar sudoku
		case 1:
			fb = carregar(quadro);

			if (fb == NULL) {
				fb = criar_arquivo_binario(quadro);
			}
			break;

		// preencher quadro com um valor
		case 2: {
			int x, y, valor;

			printf("Entre com a posicao e o valor (linha, coluna, valor): ");
			scanf("%d %d %d", &x, &y, &valor);


			if (e_valido(quadro, x, y, valor)) {
				quadro[x][y] = valor;
				salvar_jogada_bin(fb, quadro);
			}
			else
				puts("Valor ou posicao incorreta! Tente novamente!");
		}
			break;

		// resolver 1 passo
		case 3:
			resolver_um_passo(quadro);
			salvar_jogada_bin(fb, quadro);
			puts("Um passo resolvido!");
			break;

		// resolver o sudoku
		case 4:
			resolver(fb, quadro);
			break;

		case 9:
			puts("Programa finalizado ..");
			break;

		default:
			puts(INVALID_OPTION);
			break;
		}
	}

    fclose(fb);
}

/* -----------------------------------------------------------------------------
 * RESOLVER
 * Resolve o sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver (FILE *fb, char quadro[SIZE][SIZE]) {

	// enquanto houver posicoes vazias no quadro
	while(existe_campo_vazio(quadro)) {
		resolver_um_passo(quadro);
		salvar_jogada_bin(fb, quadro);
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER UM PASSO
 * Preenche apenas um campo vazio
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolver_um_passo(char quadro[SIZE][SIZE]) {
    int found = 0;

    for (int i = 0; i < SIZE && !found; i++) {
        for (int j = 0; j < SIZE && !found; j++) {
            if (quadro[i][j] == 0) {
                found = fill(quadro, i, j, 1);
            }
        }
    }
}

int fill(char quadro[SIZE][SIZE], int i, int j, int value)
{
    if (value <=  9 && e_valido(quadro, i, j, value)) {
        quadro[i][j] = value;

        return VERDADEIRO;
    } else {
        if (value < 9) {
            fill(quadro, i, j, ++value);
        } else {
            if (j == 0 && i != 0) {
                i -= 1;
                j = SIZE-1;
            }
            quadro[i][j] = 0;
            fill(quadro, i, j-1, quadro[i][j-1]+1);
        }

        return VERDADEIRO;
    }
}

/* -----------------------------------------------------------------------------
 * SALVAR JOGADA BINARIO
 * Salva o estado atual do quadro no arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void salvar_jogada_bin(FILE *fb, char quadro[SIZE][SIZE]) {
    int n = 0;

    fseek(fb, 0, SEEK_END);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            n = quadro[i][j];
            fwrite(&n, sizeof(int), 1, fb);
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							FUNCOES AUXILIARES
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -----------------------------------------------------------------------------
 * FIM X
 * Indice final da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_x (int quadr) {
	switch(quadr) {
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

/* -----------------------------------------------------------------------------
 * FIM Y
 * Indice final da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_y (int quadr) {
	switch(quadr) {
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

/* -----------------------------------------------------------------------------
 * GEN_RANDOM
 * Gera uma cadeira de caracteres randomica de tamanho len
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void gen_random (char *s, const int len) {
	srand(time(NULL));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
	int i;

    for (i = 0; i < len; i++) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];;
    }

    s[len] = 0;
}

/* -----------------------------------------------------------------------------
 * INI X
 * Indice inicial da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_x (int quadr) {
	switch(quadr) {
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

/* -----------------------------------------------------------------------------
 * INI Y
 * Indice inicial da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_y (int quadr) {
	switch(quadr) {
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

/* -----------------------------------------------------------------------------
 * LE OPCAO
 * Imprime a mensagem a faz a leitura da opcao
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int le_opcao () {
	int opcao;

	printf("Opcao: ");
	scanf("%d", &opcao);

	return opcao;
}

/* -----------------------------------------------------------------------------
 * MENU
 * Imprime o menu de opcoes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu () {
	puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("[1] - Carregar jogo");
	puts("[2] - Jogar");
	puts("[3] - Resolver um passo");
	puts("[4] - Resolver");
	puts("[9] - Finalizar");
	puts("--------");
}

/* -----------------------------------------------------------------------------
 * MENU ARQUIVO
 * Imprime o menu de opcoes do arquivo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu_arquivo () {
	puts("\n~~~~~ MENU ARQUIVO ~~~~~");
	puts("[1] - Novo jogo");
	puts("[2] - Continuar jogo");
	puts("[9] - Retornar ao menu anterior");
	puts("--------");
}

