#ifndef MEDICO_H
#define MEDICO_H
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

typedef struct medico {
	char nome[20];
	char especialidade;
}medico;

#endif