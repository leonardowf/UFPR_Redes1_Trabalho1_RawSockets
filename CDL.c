#include "includes.h"

void clienteCDL(unsigned char* argumento) {
	int x;
	x = chdir(argumento);
	if (x == -1)
		printf("diretório inexistente");

}
