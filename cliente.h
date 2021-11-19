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

extern char nome;
extern char* perguntaResposta;

char criaUtente();
char fichaUtente(char *especialidade, char *prioridade, int nUtentes, int nEspecialistas);
char respondeMedico();

typedef struct cliente clt;
struct {
	char nome[20];
	char sintomas;
}cliente;

#endif