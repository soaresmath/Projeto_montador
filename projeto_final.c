/* Funcionamento: Deixe o arquivo .asm que você deseja traduzir na mesma pasta do montador.
   Compile o código, e depois execute o montador.exe. O programa irá pedir para você inserir
   o nome do arquivo .asm. Após inserir o nome, se tudo ocorrer como esperado, o programa
   irá traduzir o arquivo .asm para um arquivo chamado "Traduzido.mem". */

/*aluno: Matheus Soares Nascimento matricula: SP3162079*/

/* Incluindo as bibliotecas necessárias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Declarando variáveis globais */
FILE *arquivoEntrada;    // Variável para o arquivo de entrada (.ASM)
FILE *arquivoSaida;      // Variável para o arquivo de saída (.mem)
char nomeArquivo[51];    // Nome do arquivo com espaço para o terminador nulo
char bufferLinha[51];    // Buffer para armazenar cada linha lida do arquivo
char parteLinha[10];     // Buffer para guardar partes das linhas durante a tradução

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

    // Verifica se o primeiro caractere é um número
    if (isdigit(bufferLinha[i])) {
        // Avança até o final do número
        while (isdigit(bufferLinha[i])) {
            i++;
        }
        // Verifica se o próximo caractere é uma letra ou espaço
        if (isalpha(bufferLinha[i]) || bufferLinha[i] == ' ') {
            return i;  // Retorna a posição onde o número termina
        }
    }
    return -1;  // Se não for um item da lista, retorna -1
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
        fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "STA", 3) == 0) {
        sscanf(bufferLinha, "STA %s", parteLinha);
        fputc(0x10, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "LDA", 3) == 0) {
        sscanf(bufferLinha, "LDA %s", parteLinha);
        fputc(0x20, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "ADD", 3) == 0) {
        sscanf(bufferLinha, "ADD %s", parteLinha);
        fputc(0x30, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "OR", 2) == 0) {
        sscanf(bufferLinha, "OR %s", parteLinha);
        fputc(0x40, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "AND", 3) == 0) {
        sscanf(bufferLinha, "AND %s", parteLinha);
        fputc(0x50, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "NOT", 3) == 0) {
        fputc(0x60, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "JMP", 3) == 0) {
        sscanf(bufferLinha, "JMP %s", parteLinha);
        fputc(0x80, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "JN", 2) == 0) {
        sscanf(bufferLinha, "JN %s", parteLinha);
        fputc(0x90, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "JZ", 2) == 0) {
        sscanf(bufferLinha, "JZ %s", parteLinha);
        fputc(0xA0, arquivoSaida); fputc(0x00, arquivoSaida); fputc(hex_para_byte(parteLinha), arquivoSaida); fputc(0x00, arquivoSaida);
    } else if (strncmp(bufferLinha, "HLT", 3) == 0) {
        fputc(0xF0, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida); fputc(0x00, arquivoSaida);
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
    fputc(0x03, arquivoSaida);
    fputc(0x4E, arquivoSaida);
    fputc(0x44, arquivoSaida);
    fputc(0x52, arquivoSaida);

    /* Agora lê o arquivo linha por linha e traduz para o formato .mem */
    while (fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada) != NULL) {
        printf("%s", bufferLinha);
        processa_linha(bufferLinha); /* Processa cada linha e a traduz */
    }

    /* Verifica se leu todo o conteúdo do arquivo */
    if (feof(arquivoEntrada)) {
        printf("\nArquivo lido completamente.\n");
    } else {
        printf("\nErro ao ler o arquivo.\n");
    }
    
    /* Fecha os arquivos depois que terminar */
    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    /* Abre o arquivo traduzido no Notepad para você ver o resultado */
    system("Notepad Traduzido.mem");

    return 0;
}
