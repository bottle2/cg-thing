//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//  Versao 09/2010
//
//**********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

#define HEADER_SIZE      14 //sizeof(HEADER) vai dar 16 devido ao alinhamento de bytes
#define INFOHEADER_SIZE  40 //sizeof(INFOHEADER) da 40 e esta correto.

unsigned char *bmp_get(char const *pathname, int *width, int *height)
{
    int imagesize    = 0;
    int bytesPerLine = 0;
    int bits         = 0;

    unsigned char *data = NULL;
  
    struct header     header;
    struct infoheader info;

    FILE *fp = fopen(pathname, "rb");

    if (NULL == fp)
    {
        printf("Erro ao abrir arquivo %s para leitura\n", pathname);
        return NULL;
    }

    printf("Carregando arquivo %s\n", pathname);

    //le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
    //o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);

    //le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
    fread(&info.size,        sizeof(unsigned int),       1, fp);
    fread(&info.width,       sizeof(int),                1, fp);
    fread(&info.height,      sizeof(int),                1, fp);
    fread(&info.planes,      sizeof(unsigned short int), 1, fp);
    fread(&info.bits,        sizeof(unsigned short int), 1, fp);
    fread(&info.compression, sizeof(unsigned int),       1, fp);
    fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fread(&info.xresolution, sizeof(int),                1, fp);
    fread(&info.yresolution, sizeof(int),                1, fp);
    fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

    *width  = info.width;
    *height = info.height;
    bits    = info.bits;
    bytesPerLine = (3 * (*width + 1) / 4) * 4;
    imagesize    = bytesPerLine * *height;
    int delta    = bytesPerLine - (3 * *width);

    #if 0
    printf("\nImagem: %dx%d - Bits: %d", width, height, bits);
    printf("\nbytesPerLine: %d", bytesPerLine);
    printf("\nbytesPerLine: %d", width * 3);
    printf("\ndelta: %d", delta);
    printf("\nimagesize: %d %d", imagesize, info.imagesize);
    #endif

    if (header.type != 19778)
    {
#if 0
		 printf("\nError: Arquivo BMP invalido");
		 getchar();
		 exit(0);
#else
		return NULL;
#endif
    }

#if 0
    if (*width * *height * 3 != imagesize)
    {
		 printf("\nWarning: Arquivo BMP nao tem largura multipla de 4");
		 getchar();
    }
#endif

    if (info.compression != 0)
    {
#if 0
		printf("\nError: Formato BMP comprimido nao suportado");
		getchar();
#else
		return NULL;
#endif
    }

    if (bits != 24)
    {
#if 0
		printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
		getchar();
#else
		return NULL;
#endif
    }

    if (info.planes != 1)
    {
#if 0
		printf("\nError: Numero de Planes nao suportado: %d", info.planes);
		getchar();
#else
		return NULL;
#endif
    }

    data = malloc(imagesize);

    if (NULL == data)
    {
        return NULL;
    }

    fseek(fp, header.offset, SEEK_SET); //correção do bug

    fread(data, sizeof(unsigned char), imagesize, fp);

    fclose(fp);

    return data;
}

