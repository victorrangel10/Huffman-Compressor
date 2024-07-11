#include <stdio.h>

#include "arvore.h"
#include "bitmap.h"
#include "listaArvores.h"
#define ASCII_SIZE 127
#define MEGABYTE 8388608

void LeString(char *frase, int pesos[ASCII_SIZE], lista *lista)
{
    char letra;
    for (int i = 0; frase[i] != '\0'; i++)
    {
        letra = frase[i];
        pesos[letra]++;
    }
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
    /* char *frase = "joss conras dornie"; */

    // array de ints representando a tabela ASCII que contem os pesos de cada letra
    int pesos[ASCII_SIZE] = {0};

    lista *listaArvores = CriaLista();

    // arquivo de saida compactado
    FILE *arqbin = fopen("arquivos/compressao.comp", "wb");

    // arquivo de entrada
    FILE *arqtxt = fopen(argv[1], "r");

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

    // le o input e determina os pesos de cada letra
    /* printf("string lida do arquivo:\n"); */
    int caractere;
    while ((caractere = fgetc(arqtxt)) != EOF)
    {
        /* putchar(caractere); // Imprime o caractere na tela */
        pesos[caractere]++;
    }

    // busca o fim do arquivo para saber o tamanho dele
    fseek(arqtxt, 0, SEEK_END);
    size = ftell(arqtxt);

    // Adiciona caracteres usados à lista de árvores
    for (size_t i = 0; i < ASCII_SIZE; i++)
    {
        if (pesos[i] > 0)
        {
            node *arvore = CriaArvore(i, pesos[i], CriaVazio(), CriaVazio());
            InsereFinalLista(listaArvores, arvore);
        }
    }
    // cria caractere ETX para comunicar o fim do texto
    node *arvore = CriaArvore(3, 1, CriaVazio(), CriaVazio());
    InsereFinalLista(listaArvores, arvore);

    OrdenaLista(listaArvores);
    CriaArvoreOtima(listaArvores);

    // gera os codigos binarios para cada caractere no texto
    GeraCodigos(ObtemPrimeiraArvore(listaArvores), codigoAtual, 0, ASCII_SIZE, codigos);

    // printa os codigos gerados
    printf("\n");
    for (int i = 0; i < ASCII_SIZE; i++)
    {
        if (codigos[i][0] != '\0')
        {
            printf("Caractere %c(%d): %s\n", i, i, codigos[i]);
        }
    }

    // escreve o conteudo compactado

    bitmap *bm = bitmapInit(MEGABYTE);

    // escrevre a arvore otima em formato binario no arquivo
    EscreveCabecalho(bm, ObtemPrimeiraArvore(listaArvores));

    rewind(arqtxt);

    int trocou = 0, trocando = 0;

    while ((caractere = fgetc(arqtxt)) != EOF)
    {
        trocou = 0;

        if (trocou || trocando)
        {
            printf("caracter procurado: '%c' (%s)\n", caractere, codigos[caractere]);
        }

        if (bitmapGetLength(bm) >= MEGABYTE - 200)
        {
            trocando = 1;
        }

        // escreve cada bit do codigo de um caracter no bitmap
        for (size_t j = 0; codigos[caractere][j] != '\0'; j++)
        {

            // debug
            if (trocando)
            {
                trocando++;
                if (trocando > 200)
                {
                    trocando = 0;
                    printf("--- ACABOU TRECHO ---  \n");
                }
            }
            //  printf("%c", (unsigned char)codigos[caractere][j]);
            bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[caractere][j]);
            // se chegar perto de do MB, dumpa e cria um novo
        }

        if (bitmapGetLength(bm) >= MEGABYTE - 100)
        {
            printf("TROCOU BM\n");
            fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

            //debugging
            printf(" FIRST CONTENTS - ");
            for (size_t k =  0; k < 30; k++)
            {
                printf("%c", bitmapGetBit(bm,k) + '0');
            }
            printf("\n");

            //libera
            bitmapLibera(bm);
            bm = bitmapInit(MEGABYTE);
            trocou = 1;
        }
    }
    // coloca o caractere de final
    printf("COLOCANDO FINAL\n");
    for (size_t j = 0; codigos[03][j] != '\0'; j++)
    {
        printf("%c", (unsigned char)codigos[03][j]);
        bitmapAppendLeastSignificantBit(bm, (unsigned char)codigos[03][j]);
    }

    fwrite(bitmapGetContents(bm), (bitmapGetLength(bm) + 7) / 8, 1, arqbin);

    LiberaListaArvores(listaArvores);
    bitmapLibera(bm);
    fclose(arqbin);
    fclose(arqtxt);
    return EXIT_SUCCESS;
}
