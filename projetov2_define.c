/* Funcionamento: Deixe o arquivo .asm que você deseja traduzir na mesma pasta do montador.
   Compile o código, e depois execute o montador.exe. O programa irá pedir para você inserir
   o nome do arquivo .asm. Após inserir o nome, se tudo ocorrer como esperado, o programa
   irá traduzir o arquivo .asm para um arquivo chamado "Traduzido.mem". */

/* aluno: Matheus Soares Nascimento matricula: SP3162079 */

/* Incluindo as bibliotecas necessárias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Definições de constantes e macros */
#define TAM_NOME_ARQUIVO 51
#define TAM_BUFFER_LINHA 51
#define TAM_PARTE_LINHA 10

#define CABECALHO_BYTE1 0x03
#define CABECALHO_BYTE2 0x4E
#define CABECALHO_BYTE3 0x44
#define CABECALHO_BYTE4 0x52

#define OPCODE_NOP 0x00
#define OPCODE_STA 0x10
#define OPCODE_LDA 0x20
#define OPCODE_ADD 0x30
#define OPCODE_OR  0x40
#define OPCODE_AND 0x50
#define OPCODE_NOT 0x60
#define OPCODE_JMP 0x80
#define OPCODE_JN  0x90
#define OPCODE_JZ  0xA0
#define OPCODE_HLT 0xF0

/* Declarando variáveis globais */
FILE *arquivoEntrada;    // Variável para o arquivo de entrada (.ASM)
FILE *arquivoSaida;      // Variável para o arquivo de saída (.mem)
char nomeArquivo[TAM_NOME_ARQUIVO];    // Nome do arquivo com espaço para o terminador nulo
char bufferLinha[TAM_BUFFER_LINHA];    // Buffer para armazenar cada linha lida do arquivo
char parteLinha[TAM_PARTE_LINHA];      // Buffer para guardar partes das linhas durante a tradução

/* Declarando os protótipos das funções que serão usadas */
unsigned char hex_para_byte(const char *hex);
void processa_linha(char *bufferLinha);
int identifica_lista(const char *bufferLinha);

/* Função que converte uma string hexadecimal em um byte (um número de 0 a 255) */
unsigned char hex_para_byte(const char *hex) {
    unsigned int valor;
    sscanf(hex, "%2x", &valor);
    return (unsigned char)valor;
}

/* Função que verifica se uma linha começa com um número seguido de texto (ex: "1 Musica") */
int identifica_lista(const char *bufferLinha) {
    int i = 0;

    // Ignora espaços em branco iniciais se houver//
    while (isspace(bufferLinha[i])) {
        i++;
    }

    // Verifica se há um número no início após os espaços em branco
    if (isdigit(bufferLinha[i])) {
        // Avança até o final do número
        while (isdigit(bufferLinha[i])) {
            i++;
        }

        // Ignora o espaço que pode vir após o número
        if (isspace(bufferLinha[i])) {
            i++;
        }

        // Se há um caractere após o número e possível espaço, continua o processamento
        if (isalpha(bufferLinha[i]) || bufferLinha[i] == '\0') {
            return i;  // Retorna a posição onde o número e possível espaço terminam
        }
    }

    // Se não houver número ou o número não for seguido por um comando válido, considera a linha como não tendo lista
    return 0;  // Retorna 0 para indicar que não há um item de lista ou espaço a ser ignorado
}

/* Função que processa cada linha do arquivo ASM e traduz para o formato .mem */
void processa_linha(char *bufferLinha) {
    int pos_lista = identifica_lista(bufferLinha);
    
    if (pos_lista != -1) {
        // Ignora o número da lista e ajusta a linha para processar o texto
        bufferLinha += pos_lista;
    }

    /* Aqui fazemos a correspondência das instruções do Neander com os códigos binários */
    if (strncmp(bufferLinha, "NOP", 3) == 0) {
        fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "STA", 3) == 0) {
        sscanf(bufferLinha, "STA %s", parteLinha);
        fputc(OPCODE_STA, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "LDA", 3) == 0) {
        sscanf(bufferLinha, "LDA %s", parteLinha);
        fputc(OPCODE_LDA, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "ADD", 3) == 0) {
        sscanf(bufferLinha, "ADD %s", parteLinha);
        fputc(OPCODE_ADD, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "OR", 2) == 0) {
        sscanf(bufferLinha, "OR %s", parteLinha);
        fputc(OPCODE_OR, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "AND", 3) == 0) {
        sscanf(bufferLinha, "AND %s", parteLinha);
        fputc(OPCODE_AND, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "NOT", 3) == 0) {
        fputc(OPCODE_NOT, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "JMP", 3) == 0) {
        sscanf(bufferLinha, "JMP %s", parteLinha);
        fputc(OPCODE_JMP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "JN", 2) == 0) {
        sscanf(bufferLinha, "JN %s", parteLinha);
        fputc(OPCODE_JN, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "JZ", 2) == 0) {
        sscanf(bufferLinha, "JZ %s", parteLinha);
        fputc(OPCODE_JZ, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else if (strncmp(bufferLinha, "HLT", 3) == 0) {
        fputc(OPCODE_HLT, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida); fputc(OPCODE_NOP, arquivoSaida);
    } else {
        printf("Instrução não reconhecida: %s\n", bufferLinha);
    }
}

/* Função principal onde o programa começa a executar */
int main() {
    /* Pergunta o nome do arquivo que você quer traduzir */
    printf("\nDigite o nome do arquivo: ");
    scanf("%s", nomeArquivo);
    fflush(stdin);

    printf("\nEste foi o nome do arquivo digitado: %s\n", nomeArquivo);

    /* Tenta abrir o arquivo ASM que você especificou */
    arquivoEntrada = fopen(nomeArquivo, "r");

    /* Verifica se conseguiu abrir o arquivo */
    if (arquivoEntrada == NULL) {
        printf("\nIMPOSSÍVEL ABRIR O ARQUIVO!");
        exit(0);
    }
    
    /* Cria o arquivo onde vai salvar a tradução (arquivo .mem) */
    arquivoSaida = fopen("Traduzido.mem", "w");
    if (arquivoSaida == NULL) {
        printf("\nIMPOSSÍVEL CRIAR O ARQUIVO Traduzido.mem");
        fclose(arquivoEntrada);
        exit(0);
    }

    /* Escreve um cabeçalho padrão no começo do arquivo .mem */
    fputc(CABECALHO_BYTE1, arquivoSaida);
    fputc(CABECALHO_BYTE2, arquivoSaida);
    fputc(CABECALHO_BYTE3, arquivoSaida);
    fputc(CABECALHO_BYTE4, arquivoSaida);

    /* Agora lê o arquivo linha por linha e traduz para o formato .mem */
    while (fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada) != NULL) {
        /* Ignora linhas em branco */
        if (bufferLinha[0] != '\n') {
            processa_linha(bufferLinha);
        }
    }

    /* Fecha os arquivos */
    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    /* Abre o arquivo traduzido para ver o resultado (no Notepad, por exemplo) */
    system("notepad Traduzido.mem");

    return 0;
}