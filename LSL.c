#include "includes.h"

void clienteLSL(int parametro) {

	FILE* arquivo;
	unsigned char conteudo[255];
	int len = 1;

	switch (parametro) {
	case 0:
		system("ls > .temp.txt");
		break;
	case 1:
		system("ls -a > .temp.txt");
		break;
	case 2:
		system("ls -l > .temp.txt");
		break;
	case 3:
		system("ls -la > .temp.txt");
		break;
	default:
		printf("Erro misterioso no Cliente LSL");
		break;
	}

	arquivo = fopen(".temp.txt", "r");

	while (len != 0) {
		memset(conteudo, 0, 255);
		len = fread(conteudo, 1, 255, arquivo);
		printf("%s", conteudo);

	}

	system("rm .temp.txt");
	fclose(arquivo);

}
