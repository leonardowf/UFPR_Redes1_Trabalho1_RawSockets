/****************************** crc8.c **************************************\

Functions to calculate 8 bit checksums.

This code was created by Henrik Björkman using examples
from crc_v3.txt written by Ross N. Williams
You are free to use the code as long as you keep a
comment about where you got it.

History:

1.0 Created by Henrik Bjorkman 1996-05-02

\*****************************************************************************/
#include "includes.h"

unsigned int crc8_calculate(unsigned char *buf, int size)
{
  unsigned i=0; unsigned short crc=0;

  assert(buf);

  for (i=0;i<size;i++) {crc=CRC8_COMPUTE(crc,buf[i]);}

  assert(crc<256);

  return(crc);
}

int calculaCRC(pacote* recebido) {
	unsigned char* tudoMenosDelimitador;
	unsigned char* semDelESemCRC;
	int r;
//
//	//recebido->crc = 0;
////	printf("recebido->seuqneica %d\n", recebido->sequencia);
////	printf("recebido->tam %d\n", recebido->tamanho);
////	printf("recebido->dados %s\n", recebido->dados);
////	printf("recebido->tipo %d\n", recebido->tipo);
////	printf("recebido->deli %d\n", recebido->delimitador);
//
	tudoMenosDelimitador = calloc(1, sizeof(pacote));
	semDelESemCRC = calloc(1, sizeof(pacote) - 2);
	memcpy(tudoMenosDelimitador, recebido, sizeof(pacote));
	memcpy(semDelESemCRC, tudoMenosDelimitador + 1, sizeof(pacote) - 2);
//
////	printf("semDelESen...: %s\n", semDelESemCRC);
////	printf("tudo menos delimitador é: %s\n", tudoMenosDelimitador);
	r = crc8_calculate(semDelESemCRC, sizeof(pacote) - 2);
//
////	printf("CRC é: %d\n", r);
//
	return r;

	return 0;

}
