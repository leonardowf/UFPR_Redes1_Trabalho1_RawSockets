#include "includes.h"

void iniciaModoServidor() {
	printf("----- Modo Servidor -----\n");
	while (42) {
		servidorRecebeMensagem();

	}

}

void inicializaRede() {
	struct sigaction sa;
	struct ifreq ifr;

	//Cria socket.
	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if (sock < 0) {
		printf(" - Erro no socket\n");
		exit(0);
	}

	strcpy(ifr.ifr_name, "eth0");
	if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
		printf(" - Erro no ioctl!\n");
		exit(0);
	}

	ioctl(sock, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sock, SIOCSIFFLAGS, &ifr);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SV_INTERRUPT;
	sigaction(SIGALRM, &sa, NULL);

}

void servidorRecebeMensagem() {
	pacote* recebido;
	unsigned char tipo;

	recebido = recebe();
	tipo = recebido->tipo;
	if (recebido->delimitador == TIPO_DELIMITADOR) {
		if (recebido->crc == calculaCRC(recebido)) {
			switch (tipo) {
			case TIPO_LS:
				//printf("Recebi uma mensagem do tipo LS\n");
				enviaACK(); // e se eu perder esse ack?
				servidorTrataLSR(recebido);
				break;
			case TIPO_CD:
				//printf("Recebi uma mensagem do tipo CD\n");
				enviaACK(); // e se eu perder esse ack?
				servidorTrataCDR(recebido);
				break;
			case TIPO_COMPARA:
				//printf("Recebi uma mensagem do tipo DIF\n");
				enviaACK(); // e se eu perder esse ack?
				servidorTrataDIF(recebido);
				break;
			default:
				printf(
						"Recebi uma mensagem de tipo desconhecido porque é: %d\n",
						tipo);
				break;

			}

		} else {
			printf("CRC incorreto, o da mensagem é: %x e o calculado é: %d",
					recebido->crc, calculaCRC(recebido));
			enviaNACK();
		}

	} else
		printf(
				"Recebi uma mensagem que não é para mim pois o delimitador é: %x\n",
				recebido->delimitador);
	free(recebido);
}

unsigned char* calculaTamanhoArquivo(unsigned char* nomeArq) {
	long int size;
	unsigned char* temp;

	temp = calloc(1, 31);
	FILE* arq;
	arq = fopen(nomeArq, "r");
	fseek(arq, 0L, SEEK_END);
	size = ftell(arq);
	//printf("tamanho do arquivo: %ldbytes\n", size);
	sprintf(temp, "%ld", size);
	fclose(arq);
	return temp;

}

pacote* recebe() {
	//printf("to no recebe\n");
	unsigned char local[sizeof(pacote)];
	unsigned char* vect;
	vect = calloc(1, sizeof(pacote));

	recv(sock, (unsigned char *) vect, sizeof(local), 0x0);
	return (pacote*) vect;

}

void enviaACK() {
	pacote* novoPacote;
	unsigned char* buffer;

	buffer = calloc(1, sizeof(pacote));
	novoPacote = calloc(1, sizeof(pacote));
	novoPacote->delimitador = TIPO_DELIMITADOR;
	novoPacote->tamanho = 0;
	novoPacote->sequencia = 0;
	novoPacote->tipo = TIPO_ACK;
	novoPacote->crc = calculaCRC(novoPacote); // calcular crc
	memcpy(buffer, novoPacote, sizeof(pacote));
	envia(buffer, sizeof(pacote));
	//printf("Enviei um ACK\n");

}

void enviaNACK() {
	pacote* novoPacote;
	unsigned char* buffer;

	buffer = calloc(1, sizeof(pacote));
	novoPacote = calloc(1, sizeof(pacote));
	novoPacote->delimitador = TIPO_DELIMITADOR;
	novoPacote->tamanho = 0;
	novoPacote->sequencia = 0;
	novoPacote->tipo = TIPO_NACK;
	novoPacote->crc = calculaCRC(novoPacote); // calcular crc
	memcpy(buffer, novoPacote, sizeof(pacote));
	envia(buffer, sizeof(pacote));
	//printf("Enviei um ACK\n");

}

int envia(unsigned char* buff, int tam) {
	int retornoSend;
	struct sockaddr_ll to;
	socklen_t len;
	to.sll_protocol = htons(ETH_P_ALL);
	to.sll_family = AF_PACKET;
	to.sll_ifindex = _NUM_IFACE_;
	to.sll_halen = 8;
	to.sll_protocol = 0;
	to.sll_hatype = 0;
	to.sll_pkttype = 0;
	len = sizeof(struct sockaddr_ll);
	retornoSend = sendto(sock, (unsigned char *) buff, tam, 0, (struct sockaddr*) &to,
			len);
	return retornoSend;
}

void clienteEnviaMensagem(unsigned char* mensagem, int sequencia, int tipo, int tam) {

	struct pollfd pfd;
	int pollRet = 0;
	int tentativas = 0;

	pfd.fd = sock;
	pfd.events = POLLIN;

	pacote* novoPacote;
	unsigned char* buffer;
	buffer = calloc(1, sizeof(pacote));
	novoPacote = calloc(1, sizeof(pacote));
	novoPacote->delimitador = TIPO_DELIMITADOR;
	novoPacote->tamanho = tam;
	novoPacote->sequencia = sequencia;
	novoPacote->tipo = tipo;
	memcpy(novoPacote->dados, mensagem, tam);
	novoPacote->crc = calculaCRC(novoPacote); // calcular crc
	memcpy(buffer, novoPacote, sizeof(pacote));

	do {
		//printf("ainda nao recebi um ack\n");
		tentativas = 0;
		do {
			envia(buffer, sizeof(pacote));
			pollRet = poll(&pfd, 1, TIMEOUT);
			if (pollRet == 0) {
				printf("Erro, nao consegui conectar. tentativa n: %d!\n",
						tentativas + 1);
				tentativas++;
			}
		} while (pollRet == 0 && tentativas < MAX_TENTATIVAS);

		if (tentativas >= MAX_TENTATIVAS) {
			printf("Erro, Número de tentativas excedida.");
			exit(0);
		}
	} while (!recebeuACK());

}

int recebeuACK() {
	//printf("to esperando acks adoidado na mensagem: %s", mensagem);
	pacote* test;
	test = recebe();
	//printf("sai do recebe\n");
	if (test->delimitador == TIPO_DELIMITADOR
			&& (test->tipo == TIPO_ACK || test->tipo == TIPO_TEST)) // e crc ok
			{
		//printf("deu certo, recebi um ACK\n");
		return 1;
	} else {
		//printf("nao recebi um ack\n");
		return 0;
	}
}

int dadosIguais(unsigned char* dadosRemoto, int tamanho, int numeroMensagem) {
	unsigned char dadosLocal[255];
	int bytesLidos = 0;



	memset(dadosLocal, 0, 255);
	bytesLidos = fread(dadosLocal, 1, tamanho, arquivoCliente);
	fflush(arquivoCliente);


	//printf("dadosRemoto tem: |%s|\n", dadosRemoto);
	//printf("dadosLocal tem: |%s|\n", dadosLocal);

	if (bytesLidos != tamanho) {
		printf(
				"Erro, já não consegui ler a quantidade de bytes no arquivo local, portanto são diferentes.\n");
		return FALSE;
	} else if (buffersIguais2(dadosLocal, dadosRemoto, tamanho)) {
		return TRUE;
	} else {
		printf("Erro, os buffers não são iguais.\n");
		return FALSE;
	}

}

int buffersIguais(unsigned char* buffer1, unsigned char* buffer2, int tamanho) {
	int i;
	int byte1, byte2;

	for (i = 0; i < tamanho; i++) {
		byte1 = buffer1[i];
		byte2 = buffer2[i];
		if (byte1 - byte2 != 0) {
			imprimeDebugs("buffers diferentes");
			return FALSE;
		}

	}
	imprimeDebugs("buffers iguais");
	printf("tamanho %d\n", tamanho);
	return TRUE;

}

int buffersIguais2(unsigned char* buffer1, unsigned char* buffer2, int tamanho) {
	int i;
	int byte1, byte2;
	int outcome;

	outcome = memcmp(buffer1, buffer2, tamanho);
		  if (outcome)
			  return FALSE;
		  else


	return TRUE;

}

int arquivoExiste(unsigned char* nomeArq) {
	FILE *arq;

	if (!(arq = fopen(nomeArq, "r"))) {
		return FALSE;
	} else {
		fclose(arq);
		return TRUE;
	}
}

void imprimeDebugs(unsigned char* imprime) {
	if (DEBUG == TRUE) {
		printf("%s\n", imprime);
	}
}
