//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//
//  Referencia:  http://astronomy.swin.edu.au/~pbourke/dataformats/bmp/
//  Versao 09/2010
//
//**********************************************************

#ifndef ___BMP__H___
#define ___BMP__H___

struct header
{
   unsigned short int type;      // Magic identifier.
   unsigned int       size;      // File size in bytes.
   unsigned short int reserved1;
   unsigned short int reserved2;
   unsigned int       offset;    // Offset to image data em bytes.
};

struct infoheader
{
   unsigned int       size;        // Header size in bytes.
   int                width;       // Width of image.
   int                height;      // Height of image.
   unsigned short int planes;      // Number of colour planes.
   unsigned short int bits;        // Bits per pixel.
   unsigned int       compression; // Compression type.
   unsigned int       imagesize;   // Image size in bytes.
   int                xresolution; // Pixels per meter.
   int                yresolution; // Pixels per meter.
   unsigned int       ncolours;    // Number of colours.
   unsigned int       impcolours;  // Important colours.
};

unsigned char * bmp_get(char const *pathname, int *width, int *height);

#endif
