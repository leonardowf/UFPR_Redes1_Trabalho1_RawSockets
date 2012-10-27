#include "includes.h"

void clienteDIF(unsigned char* arquivo1, unsigned char* arquivo2) { // arquivo1 é o arquivo local, arquivo2 é o remoto
	int resultado;
	if (arquivoExiste(arquivo1)) {
		arquivoCliente = fopen(arquivo1, "rb");
		//printf("mandei isso: |%s|\n", arquivo2);

		clienteEnviaMensagem(arquivo2, 0, TIPO_COMPARA, strlen(arquivo2));
		resultado = clienteRecebeResultadoDIF();
		fclose(arquivoCliente);
		if (resultado == 42)
			clienteDIF(arquivo1, arquivo2);

	} else
		printf("Erro, o arquivo <%s> não existe.\n", arquivo1);

}

int clienteRecebeResultadoDIF() {
	int recebeuCerto = FALSE;
	pacote* recebido;

	do {
		recebido = recebe();
		if (recebido->delimitador == TIPO_DELIMITADOR) {
			if (recebido->crc == calculaCRC(recebido)) {
				if (recebido->tipo == TIPO_DESCRITOR) {
					enviaACK();
					//printf("O tamanho do arquivo é %s bytes.\n",
							//recebido->dados);
					int resultado = clienteRecebeDadosDIF(); //espera dados
					if (resultado == 42)
						return 42;
					recebeuCerto = TRUE;

				} else if (recebido->tipo == TIPO_ERRO) {
					enviaACK();
					recebeuCerto = TRUE;
					printf("Erro, arquivo inexistente pq?\n");
					//clienteEnviaMensagem("", 0, TIPO_ABORTA, 0); // tem que ver isso aqui !!!!!
				}

			} else {
				printf("Erro de CRC 666\n"); //666
				enviaNACK();
			}

		} else
			printf(
					"Recebi uma mensagem no clienteRecebeResultadoDif com delimitador %d\n",
					recebido->delimitador);

	} while (!recebeuCerto);

}

int clienteRecebeDadosDIF() {
	int terminou = FALSE;
	int numeroMensagem = 0;
	unsigned char dados[255];
	int tamanho;
	int umByte = 0;
	unsigned char* tempUmByte[255];
	int sequenciaCerta = 0;
	int tentativasCRC = 0;

	pacote* recebido;
	do {
		recebido = recebe();
		if (recebido->delimitador == TIPO_DELIMITADOR) {
			if (recebido->crc == calculaCRC(recebido)) {
				if (recebido->tipo == TIPO_DADOS) {
					if (recebido->sequencia == sequenciaCerta) {
						sequenciaCerta++;
						if (sequenciaCerta == 8)
							sequenciaCerta = 0;

						enviaACK();
						tamanho = recebido->tamanho;
						memset(dados, 0, 255);
						memcpy(dados, recebido->dados, tamanho);
						if (dadosIguais(dados, tamanho, numeroMensagem)) {
							clienteEnviaMensagem("", 0, TIPO_ACK, 0); // mensagem de <continua>
							numeroMensagem++;

						} else // sao diferentes
						{
							printf(
									"1) Os arquivos são diferentes. Essa diferença se deu na mensagem #%d de dados.\n",
									numeroMensagem);

							clienteEnviaMensagem("", 0, TIPO_ABORTA, 0);
							terminou = TRUE;
						}

					} else {
						//printf("Sequencia errada\n");
						clienteEnviaMensagem("", 0, TIPO_TEST, 0);
						terminou = TRUE;
						return 42;
					}

				} else if (recebido->tipo == TIPO_FIM) {
					enviaACK(); // mechi aqui deu pau

					// tenta ler mais um byte.. se consegue ler esse byte significa que o arquivo remoto é menor só que eles são diferentes por causa que o local ainda nao terminou.
					memset(tempUmByte, 0, 255);
					umByte = fread(tempUmByte, 1, 1, arquivoCliente);
					fflush(arquivoCliente);
					if (umByte > 0) {
						printf(
								"2) Os arquivos são diferentes. Essa diferença se deu na mensagem #%d de dados.\n",
								numeroMensagem - 1);
					} else
						printf("Arquivos iguais.\n");

					terminou = TRUE;
				}

			} else {
				printf("CRC errado, 778\n");
//				clienteEnviaMensagem("", 0, TIPO_TEST, 0);
				enviaNACK();
			}

		} else
			printf("Erro 777, Delimitador %d\n", recebido->delimitador);

	} while (!terminou);

}

void servidorTrataDIF(pacote* recebido) {
	int tamanho;
	unsigned char nomeArquivo[255];
	unsigned char* tamanhoArquivo;

	memset(nomeArquivo, 0, 255);
	tamanho = recebido->tamanho;
	memcpy(nomeArquivo, recebido->dados, tamanho);

	if (arquivoExiste(nomeArquivo)) {
		imprimeDebugs("arquivo existe");
		//printf("nome do arquivo: %s\n", nomeArquivo);
		tamanhoArquivo = calculaTamanhoArquivo(nomeArquivo);
		clienteEnviaMensagem(tamanhoArquivo, 0, TIPO_DESCRITOR,
				strlen(tamanhoArquivo));
		//imprimeDebugs(
				//"enviei um descritor pq o arquivo existe e recebi confirmação");
		servidorEnviaDadosDIF(nomeArquivo);

	} else {
		clienteEnviaMensagem("0", 0, TIPO_ERRO, 1);
		printf("nome do arquivo que nao existe eh: %s", nomeArquivo);
	}

}

void servidorEnviaDadosDIF(unsigned char* nomeArquivoRemoto) {

	FILE* arquivo;
	int tamanho = 1, sequencia = 0;
	unsigned char dados[255];
	int resultado = 0;

	arquivo = fopen(nomeArquivoRemoto, "rb");

	while (tamanho > 0) {
		memset(dados, 0, 255);
		tamanho = fread(dados, 1, 255, arquivo);
		fflush(arquivo);
		if (tamanho > 0) {
			clienteEnviaMensagem(dados, sequencia, TIPO_DADOS, tamanho);
			resultado = servidorParaDeEnviarDadosDIF();
			if (resultado == TRUE) {
				imprimeDebugs("sai pq deu diferença");
				break;
			} else if (resultado == FALSE) {
				sequencia++;
				if (sequencia == 8)
					sequencia = 0;
			} else if (resultado == 42) {

				break;
			}

		} else {
			imprimeDebugs("enviei uma mensagem do tipo fim");
			clienteEnviaMensagem("", 0, TIPO_FIM, 0);
		}

	}

	fclose(arquivo);

}

int servidorParaDeEnviarDadosDIF() {
	pacote* recebido;
	int recebeuCerto = FALSE;
	do {
		recebido = recebe();
		if (recebido->delimitador == TIPO_DELIMITADOR) {
			if (recebido->crc == calculaCRC(recebido)) {
				if (recebido->tipo == TIPO_ACK) {
					enviaACK();
					//imprimeDebugs("continua enviando pq recebi um ack");
					return FALSE;
				} else if (recebido->tipo == TIPO_ABORTA) {
					enviaACK();
					imprimeDebugs("parou de enviar pq recebi um abortar");
					return TRUE;
				} else if (recebido->tipo == TIPO_TEST) {
					enviaACK();
					return 42;
				}

			} else {
				printf("Erro de CRC 111.\n");
				enviaNACK();
			}

		} else
			printf("Erro, 999.\n");

	} while (!recebeuCerto);
	return FALSE;
}
