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

// Codigo Meta 1

void encerraServidor(int fd, char *nameFIFO)
{
    printf("Adeus!\n");
    close(fd);
    unlink(nameFIFO);
    exit(1);
}
//char criaUtente();
//char fichaUtente(char *especialidade, char *prioridade, int nUtentes, int nEspecialistas);
//char respondeMedico();

#endif