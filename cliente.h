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

#define SERVER_FIFO "SERVIDOR"
#define CLIENTE_FIFO "CLIENTE[%d]"
#define MEDICO_FIFO "MEDICO [%d]"

char CLIENTE_FIFO_FINAL [100];

typedef struct
{
	char nome[100];
	char sintomas[100];
	char especialidade[100];
	int prioridade;
}cliente;

typedef struct 
{
	pid_t pid;
	char msg[100];
}dataMSGCTL;

typedef struct 
{
	char res[100];
}dataRPLCTL;

#endif