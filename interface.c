#include "includes.h"

void interfaceCliente() {
	unsigned char entrada[255];
	while (42) {
		memset(entrada, 0, 255);
		printf("> ");
		scanf("%[^\n]", entrada);
		getchar(); // captura o retorno do carrier

		// temos os seguintes comandos
		// lsl -l, lsr -r, cdr <argumento>, cdl <argumento>, dif <arquivo1> <arquivo2>

		// tratando ls's
		if (strlen(entrada) < 3)
			printf("Erro, comando muito pequeno.\n");
		else {
			if (entrada[0] == 'l' && entrada[1] == 's' && entrada[2] == 'l')
				trataLSL(entrada);
			else if (entrada[0] == 'l' && entrada[1] == 's'
					&& entrada[2] == 'r')
				trataLSR(entrada);
			else if (entrada[0] == 'c' && entrada[1] == 'd'
					&& entrada[2] == 'r')
				trataCDR(entrada);
			else if (entrada[0] == 'c' && entrada[1] == 'd'
					&& entrada[2] == 'l')
				trataCDL(entrada);
			else if (entrada[0] == 'd' && entrada[1] == 'i'
					&& entrada[2] == 'f')
				trataDIF(entrada);
			else if (entrada[0] == 'c' && entrada[1] == 'l'
					&& entrada[2] == 'r')
				system("clear");
			else
				printf("Erro, comando inexistente.\n");

		}

	}

}

void trataLSL(unsigned char* entrada) {
	//printf("trata LSL\n");
	if (!strcmp(entrada, "lsl"))
		clienteLSL(0);
	else if (!strcmp(entrada, "lsl -l"))
		clienteLSL(1);
	else if (!strcmp(entrada, "lsl -a"))
		clienteLSL(2);
	else if (!strcmp(entrada, "lsl -la") || !strcmp(entrada, "lsl -al")
			|| !strcmp(entrada, "lsl -l -a") || !strcmp(entrada, "lsl -a -l"))
		clienteLSL(3);
	else
		printf("Erro, comando inválido.\n");

}

void trataLSR(unsigned char* entrada) {
	//printf("trata LSR\n");
	if (!strcmp(entrada, "lsr"))
		clienteLSR(0);
	else if (!strcmp(entrada, "lsr -a"))
		clienteLSR(1);
	else if (!strcmp(entrada, "lsr -l"))
		clienteLSR(2);
	else if (!strcmp(entrada, "lsr -la") || !strcmp(entrada, "lsr -al")
			|| !strcmp(entrada, "lsr -l -a") || !strcmp(entrada, "lsr -a -l"))
		clienteLSR(3);
	else if (!strcmp(entrada, "lsr -c"))
		clienteLSR(4);
	else
		printf("Erro, comando inválido.");
}

void trataCDR(unsigned char* entrada) {
	//printf("trata CDR\n");
	unsigned char argumento[255];

	if (entrada[3] != ' ')
		printf("Erro, comando cdr inválido\n");
	else if (strlen(entrada) > 4) {
		memset(argumento, 0, 255);
		memcpy(argumento, entrada + 4, 255 - 4);
		clienteCDR(argumento);
	} else
		printf("Erro, argumento muito pequeno.\n");
}

void trataCDL(unsigned char* entrada) {
	// cdl <1..*> caracteres
	//printf("trata CDL\n");
	unsigned char argumento[255];

	if (entrada[3] != ' ')
		printf("Erro, comando cdl inválido\n");
	else if (strlen(entrada) > 4) {
		memset(argumento, 0, 255);
		memcpy(argumento, entrada + 4, 255 - 4);
		//printf("argumento é: %s|\n", argumento);
		clienteCDL(argumento);
	} else
		printf("Erro, argumento muito pequeno.\n");
}

void trataDIF(unsigned char* entrada) {

	printf("trata DIF\n");
	unsigned char ** pch;
	int i;

	pch = calloc(5, sizeof(unsigned char*));
	for (i = 0; i < 5; ++i) {
		pch[i] = calloc(1, sizeof(unsigned char*));
	}
	i = 0;

	pch[i] = strtok(entrada, " ");
	while (pch[i] != NULL && i < 3) {
		i++;
		pch[i] = strtok(NULL, " ");
	}
	if (pch[i] != NULL)
		printf("Erro, muitos argumentos.\n");
	else if (i != 3)
		printf("Erro, número de argumentos inferior ao esperado.\n");
	else {
		printf("arquivo 1: |%s|\n", pch[1]);
		printf("arquivo 2: |%s|\n", pch[2]);
		if (temCarateresInvalidos(pch[1]))
			printf("Erro, \t%s é nome de arquivo inválido\n", pch[1]);
		else if (temCarateresInvalidos(pch[2]))
			printf("Erro, \t%s é nome de arquivo inválido\n", pch[2]);
		else
			clienteDIF(pch[1], pch[2]);
	}

}

int temCarateresInvalidos(unsigned char* nome) {

	if (!strcmp(nome, ".") || !strcmp(nome, "..") || strchr(nome, '/') != NULL)
		return TRUE;
	else
		return FALSE;

}
