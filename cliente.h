#ifndef CLIENTE_H
#define CLIENTE_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <errno.h>
#include "balcao.h"

//#define SERVER_FIFO "SERVIDOR"
//#define CLIENTE_FIFO "CLIENTE[%d]"
//#define MEDICO_FIFO "MEDICO [%d]"

typedef struct
{
	char nome[100];
	char sintomas[100];
	char especialidade[100];
	char CLIENTE_FIFO_FINAL [100];
	int prioridade;
}cliente;

#endif