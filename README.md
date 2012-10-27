*Atenção: eu vou melhorar o código do meu trabalho para ficar mais legível e menos idiota (leia-se lazy), no momento eu não vou conseguir fazer isso, mas vou disponibilizar a versão que apresentei e com o tempo eu vou recriando. Não ligue a bagunça :D*

#UFPR - Redes1 - Trabalho1 - Raw Sockets - 2012/1
================================

Implementação em C do trabalho1 de Redes1 da UFPR utilizando Raw Sockets.

##O que tinha que ser feito:
Um comparador de arquivos utilizando Raw Sockets.
##Enunciado:
* Os arquivos estão localizados em máquinas diferentes devendo o arquivo da máquina remota ser transferido para a máquina local para ser comparado.
* A comparação **deve** ser feita a cada mensagem recebida e no caso dos arquivos serem diferentes o usuário **deve** ser informado da diferença, inclusive em qual mensagem tal diferença ocorreu e a transmissão deve ser abortada.
* Usar para-e-espera
* Usar 8 bits para tamanho (dados)
* Usar 4 bits para tipo
* Usar CRC de 8 bits (pode ser feito usando paridade vertical, mas o trabalho vale 9)
* Timeout é obrigatório
* Seguir especificação da sala (abaixo)

##Tipos:
* 0 - Compara
* 1 - CD
* 2 - Y
* 3 - N
* 4 - LS
* 5 - Mostra na tela
* 6 - Fim de arquivo
* 7 - Descritor
* 8 - Dados
* 9 - Abortar
* 15 - Erro

##Observações de implementação do autor:
* Meu trabalho tirou nota máxima, mas o professor foi bondoso nesse semestre por conta da greve. Para se ter uma ideia, ele não removeu o cabo das máquinas para forçar TIMEOUT, de nenhum grupo.
* Meu crc 8 bits foi retirado de um site, mas não me lembro qual, entretanto tem o nome do autor, com uma pesquisa talvez encontre.
* Tem um pequeno problema, as vezes quando se tira o cabo, dá timeout no cliente e no servidor, aí a próxima mensagem dá um erro de sequencia. Eu realmente não sei porque isso acontece, se você descobrir me conte, por favor, me conte (questão de honra :D)
* Para contornar esse erro eu reinicio o envio de tudo haha
* O código não é bonito, mas funciona bem, use por sua conta em risco
* Algumas funções eu tirei de algum trabalho de outra pessoa, mas não me lembro quem (acho que foram vários), qualquer coisa só entrar em contato que eu removo.


