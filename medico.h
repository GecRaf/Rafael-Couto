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
#include <errno.h>
#include "balcao.h"

//#define SERVER_FIFO "SERVIDOR"
//#define CLIENTE_FIFO "CLIENTE[%d]"
//#define MEDICO_FIFO "MEDICO [%d]"

typedef struct medico {
	char nome[100];
	char especialidade[100];
}medico;

#endif