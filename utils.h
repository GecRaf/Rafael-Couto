#ifndef UTILS_H
#define UTILS_H



#define Read_Cli "TMP1"
#define Write_Cli "TMP2"
#define SERVER_FIFO "SERVIDOR"
#define CLIENTE_FIFO "CLIENTE[%d]"
#define MEDICO_FIFO "MEDICO[%d]"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

//char CLIENTE_FIFO_FINAL[100];
//char MEDICO_FIFO_FINAL[100];

typedef struct {
	char nome[100];
	char sintomas[100];
	char especialidade[100];
	char CLIENTE_FIFO_FINAL[100];
	int  prioridade;
	int flag; // 0 - Fechado || 1 - Aberto
	pid_t pid;
}cliente;

typedef struct{
	char nome[100];
	char especialidade[100];
	char MEDICO_FIFO_FINAL[100];
	int flag;
	pid_t pid;
}medico;

typedef struct{
	pid_t pid;
	char msg[100];
}dataMSG;

typedef struct {
	char nome[100];
	cliente clt;
	medico mdc;
	dataMSG mensagem;
	bool userType; // true = cliente || false = medico
	pid_t pidAdm;
}administrador;

void *pipeCliente();
void *pipeMedico();
void *userType();
void *comandos();
void encerraServidor();

#endif