#include "includes.h"

void servidorTrataLSR(pacote* recebido) {
	int tamanho;
	unsigned char dados[255];

	tamanho = recebido->tamanho;

	if (tamanho == 0) {
		servidorLSR(0);
	} else {
		memset(dados, 0, 255);
		memcpy(dados, recebido->dados, tamanho);
		if (!strcmp(dados, "-a"))
			servidorLSR(1);
		else if (!strcmp(dados, "-l"))
			servidorLSR(2);
		else if (!strcmp(dados, "-al"))
			servidorLSR(3);
		else if (!strcmp(dados, "-c"))
			system("clear");
		else
			printf(
					"Erro no servidorTrataLSR, recebi isso em dados %s com tamnho %d\n",
					dados, tamanho);
	}

}

void servidorLSR(int parametro) {

	FILE* arquivo;
	int tamanho, sequencia;
	unsigned char dados[255];

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
		system("ls -al > .temp.txt");
		break;
	default:
		printf("Erro muito louco no servidor LSR\n");
		break;

	}
	arquivo = fopen(".temp.txt", "r");
	tamanho = 1;
	sequencia = 0;
	while (tamanho != 0) {
		memset(dados, 0, 255);
		tamanho = fread(dados, 1, 255, arquivo);
		if (tamanho > 0)
			clienteEnviaMensagem(dados, sequencia, TIPO_MOSTRA, tamanho);
		sequencia++;
		if (sequencia == 8)
			sequencia = 0;

	}
	clienteEnviaMensagem("", 0, TIPO_FIM, 0);

	fclose(arquivo);
	system("rm .temp.txt");

}

void clienteLSR(int argumento) {

	switch (argumento) {
	case 0:
		clienteEnviaMensagem("", 0, TIPO_LS, 0);
		clienteRecebeMostra();
		break;
	case 1:
		clienteEnviaMensagem("-a", 0, TIPO_LS, 2);
		clienteRecebeMostra();
		break;
	case 2:
		clienteEnviaMensagem("-l", 0, TIPO_LS, 2);
		clienteRecebeMostra();
		break;
	case 3:
		clienteEnviaMensagem("-al", 0, TIPO_LS, 3);
		clienteRecebeMostra();
		break;
	case 4:
		clienteEnviaMensagem("-c", 0, TIPO_LS, 2);
		break;
	default:
		printf("Erro misterioso no clienteLSR\n");
		break;
	}

}

void clienteRecebeMostra() {
	pacote* recebido;
	FILE* arquivoTemp;

	arquivoTemp = fopen(".temp.txt", "w");

	do {
		recebido = recebe();
		if (recebido->delimitador == TIPO_DELIMITADOR)
			if (recebido->crc == calculaCRC(recebido)) {
				enviaACK();
				if (recebido->tipo == TIPO_MOSTRA) {
					fwrite(recebido->dados, 1, recebido->tamanho, arquivoTemp);

				}

			} else {
				printf("CRC errado no clienteRecebeMosta");
				enviaNACK();
			}

		else
			printf(
					"Recebi uma mensagem no clienteRecebeMostra que não é para mim pois o delimitador é %d\n",
					recebido->delimitador);

	} while (recebido->tipo != TIPO_FIM);

	fclose(arquivoTemp);
	system("cat .temp.txt");
	system("rm .temp.txt");
	printf("\n");

}


