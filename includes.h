#include <stdio.h>
#include <string.h>
#include <unistd.h> // chdir
#include <signal.h> // socket
#include <errno.h>
#include <sys/ioctl.h> // socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <assert.h> // pro crc


#include <stddef.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/statvfs.h>

#include <ctype.h>

#include <sys/poll.h>
#include <time.h>



typedef struct pacote {
	unsigned char delimitador;
	unsigned char tamanho;
	unsigned char sequencia :4;
	unsigned char tipo :4;
	unsigned char dados[255];
	unsigned char crc;
} pacote;

#define TIPO_CD 1
#define TIPO_ACK 2
#define TIPO_NACK 3
#define TIPO_LS 4
#define TIPO_MOSTRA 5
#define TIPO_FIM 6
#define TIPO_DESCRITOR 7
#define TIPO_DADOS 8
#define TIPO_ABORTA 9
#define TIPO_ERRO 15
#define TIPO_DELIMITADOR 126
#define TIPO_COMPARA 0
#define _NUM_IFACE_  2
#define TIPO_CONTINUA 10
#define TIPO_TEST 11

#define FALSE 0
#define TRUE 1

#define MAX_TENTATIVAS 100
#define TIMEOUT 1000

int sock; // nosso socket

unsigned char nomeArquivoServidor[255];
FILE* arquivoCliente;

#define DEBUG 0

#include "LSR.h"
#include "LSL.h"
#include "CDL.h"
#include "CDR.h"
#include "crc.h"
#include "DIF.h"
#include "secundarias.h"
#include "interface.h"
