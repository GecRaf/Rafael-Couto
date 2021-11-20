#ifndef BALCAO_H
#define BALCAO_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>
#include "funcoes.c"
#include "cliente.h"
#include "medico.h"

//#define BALCAO_FIFO "/tmp/common_fifo"		\\ Por implementar!
//#define CLIENTE_FIFO "/tmp/cliente_%d_fifo"	\\ Por implementar!
//#define MEDICO_FIFO "/tmp/medico_%d_fifo"	\\ Por implementar!

void iniciaClassificador();
void classificador(const char *sintomas);
char comandos();

typedef struct administrador admin;
struct {
	char nome[20];
}administrador;

#endif