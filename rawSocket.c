/*
 ============================================================================
 Name        : rawSocket(release).c
 Author      : Leonardo Wistuba de França e Lennon Alberto dos Santos
 Version     : 1.0
 Description : Primeiro trabalho da Disciplina de Redes I UFPR
 	 	 	   Comparador de arquivo usando Raw Sockets.
 ============================================================================
 */

#include "includes.h"


int main() {

	int opcao;

	//system("clear");
	inicializaRede();
	
	printf("Escolha a opção para o programa:\n");
	printf("\t1) Cliente\n");
	printf("\t2) Servidor\n");
	scanf("%d", &opcao);
	getchar();

	if (opcao == 1)
		interfaceCliente();
	else if (opcao == 2)
		iniciaModoServidor();

	return EXIT_SUCCESS;

}


