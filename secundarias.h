void iniciaModoServidor();
void inicializaRede();
void servidorRecebeMensagem();
unsigned char* calculaTamanhoArquivo(unsigned char* nomeArq);
pacote* recebe();
void enviaACK();
void enviaNACK();
int envia(unsigned char* buff, int tam);
void clienteEnviaMensagem(unsigned char* mensagem, int sequencia, int tipo, int tam);
int recebeuACK();
int dadosIguais(unsigned char* dadosRemoto, int tamanho, int numeroMensagem);
int buffersIguais(unsigned char* buffer1, unsigned char* buffer2, int tamanho);
int arquivoExiste(unsigned char* nomeArq);
void imprimeDebugs(unsigned char* imprime);
int buffersIguais2(unsigned char* buffer1, unsigned char* buffer2, int tamanho);

