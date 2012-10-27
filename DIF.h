void clienteDIF(unsigned char* arquivo1, unsigned char* arquivo2); // arquivo1 é o arquivo local, arquivo2 é o remoto
int clienteRecebeResultadoDIF();
int clienteRecebeDadosDIF();
void servidorTrataDIF(pacote* recebido);
void servidorEnviaDadosDIF(unsigned char* nomeArquivoRemoto);
int servidorParaDeEnviarDadosDIF();
