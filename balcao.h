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
#include <errno.h>
#include "funcoes.c"
#include "cliente.h"
#include "medico.h"

#define SERVER_FIFO "SERVIDOR"
#define CLIENTE_FIFO "CLIENTE[%d]"
#define MEDICO_FIFO "MEDICO [%d]"

char CLIENTE_FIFO_FINAL[100];
char MEDICO_FIFO_FINAL[100];

typedef struct administrador admin;
struct {
	char nome[100];
	cliente clt;
	medico med;
}administrador;

#endif