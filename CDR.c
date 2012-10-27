#include "includes.h"

void servidorTrataCDR(pacote* recebido) {
	unsigned char argumento[255];
	int tamanho, resultado;

	memset(argumento, 0, 255);
	tamanho = recebido->tamanho;

	memcpy(argumento, recebido->dados, tamanho);

	resultado = chdir(argumento);

	if (resultado == -1)
		clienteEnviaMensagem("0", 0, TIPO_ERRO, 1);
	else
		clienteEnviaMensagem("0", 0, TIPO_ACK, 1);

}

void clienteCDR(unsigned char* argumento) {
	clienteEnviaMensagem(argumento, 0, TIPO_CD, strlen(argumento));
	clienteRecebeResultadoCDR();

}

void clienteRecebeResultadoCDR() {
	pacote* recebido;
	int recebeuCerto = FALSE;

	do {
		recebido = recebe();
		if (recebido->delimitador == TIPO_DELIMITADOR) {
			if (recebido->crc == calculaCRC(recebido)) {
				enviaACK();
				if (recebido->tipo == TIPO_ERRO) {
					recebeuCerto = TRUE;
					printf("Erro, diretório inexisteste.\n");
				} else if (recebido->tipo == TIPO_ACK) {
					printf("Diretório mudou.\n");
					recebeuCerto = TRUE;
				}

			} else {
				printf("CRC errado no clienteRecebeResultadoCDR\n");
				enviaNACK();
			}

		} else
			printf(
					"Recebi uma mensagem que não é para mim no clienteRecebeResultadoCDR pois o delimitados é %d\n",
					recebido->delimitador);

	} while (!recebeuCerto);
}
