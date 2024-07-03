#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127

/* void determina_pesos(FILE *arq, int pesos[ASCII_SIZE])
{
    char ch='\0';
    printf("entrou na determina pesos\n");
    while (ch != EOF)
    {
        ch = fgetc(arq);
        printf("peguei %c do arquivo\n", ch);
        pesos[ch]++;
    }
} */

void LeString(char *frase, int pesos[ASCII_SIZE], lista *lista)
{
    char letra;
    for (int i = 0; frase[i] != '\0'; i++)
    {
        letra = frase[i];
        pesos[letra]++;
    }
}

void EscreveCodigo(FILE *arqbin, bitmap *bm, char *frase, int tam, char codigos[tam][tam])
{
    char letra;

    printf("Codificacao:\n");
    for (int i = 0; frase[i] != '\0'; i++)
    {
        letra = frase[i];
        for (size_t j = 0; codigos[letra][j] != '\0'; j++)
        {
            printf("%c", (unsigned char)codigos[letra][j]);
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[letra][j]);
        }
        printf(" ");
    }

    fwrite(bitmapGetContents(bm), bitmapGetLength(bm), 1, arqbin);

    printf("\ntamanho do bitmap %d, agora dividido por 8 %d, agora resto 8 %d\n", bitmapGetLength(bm), bitmapGetLength(bm) / 8, bitmapGetLength(bm) % 8);

    printf("\nBitmap:\n");

    for (int i = 0; i < (bitmapGetLength(bm) + 7) / 8; i++)
    {
        printf("%2X ", bitmapGetContents(bm)[i]);
    }
    printf("\n");
}

void CriaArvoreOtima(lista *listaArvores)
{
    node *t1, *t2, *tr;
    while (RetornaTam(listaArvores) > 1)
    {
        tr = SomaDuasPrimeirasArvores(listaArvores, &t1, &t2);
        if (t1 && t2)
        {
            RetiraLista(listaArvores, t1);
            RetiraLista(listaArvores, t2);
            InsereFinalLista(listaArvores, tr);
        }
    }
}

int main(int argc, char *argv[])
{
    char *frase = "joss conras dornie";

    //array de ints representando a tabela ASCII que contem os pesos de cada letra
    int pesos[ASCII_SIZE] = {0};

    lista *listaArvores = CriaLista();

    //arquivo de saida compactado
    FILE *arqbin = fopen("coded.bin", "wb");

    //arquivo de entrada
    FILE *arqtxt = fopen("entrada.txt", "r");

    if (arqtxt == NULL)
    {
        perror("Erro ao abrir arquivo txt");
        return EXIT_FAILURE;
    }

    if (arqbin == NULL)
    {
        perror("Erro ao abrir arquivo bin!");
        return EXIT_FAILURE;
    }

    long size;
    char codigoAtual[ASCII_SIZE];
    char codigos[ASCII_SIZE][ASCII_SIZE] = {0};


    /* determina_pesos(arqtxt, pesos); */
    LeString(frase, pesos, listaArvores);

   //busca o fim do arquivo para saber o tamanho dele
    fseek(arqtxt, 0, SEEK_END);

    size = ftell(arqtxt);

    printf("size eh %d\n", size);

    bitmap *bm = bitmapInit(sizeof(codigos)*8);

    // Adiciona caracteres usados à lista de árvores
    for (size_t i = 32; i < ASCII_SIZE; i++)
    {
        if (pesos[i] > 0)
        {
            node *arvore = CriaArvore(i, pesos[i], CriaVazio(), CriaVazio());
            InsereFinalLista(listaArvores, arvore);
        }
    }

    OrdenaLista(listaArvores);
    CriaArvoreOtima(listaArvores);

    // escrevre a arvore otima em formato binario no arquivo
    EscreveCabecalho(bm, ObtemPrimeiraArvore(listaArvores));

    // gera os codigos binarios para cada caractere no texto
    GeraCodigos(ObtemPrimeiraArvore(listaArvores), codigoAtual, 0, ASCII_SIZE, codigos);

    // printa os codigos gerados
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        if (codigos[i][0] != '\0')
        {
            printf("Caractere %c: %s\n", i, codigos[i]);
        }
    }

    // escreve o conteudo compactado
    EscreveCodigo(arqbin, bm, frase, ASCII_SIZE, codigos);
    // DesalocaLista
    // desaloca bitmap
    fclose(arqbin);
    return EXIT_SUCCESS;
}
