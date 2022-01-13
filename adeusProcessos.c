#include "utils.h"



do
{
    scanf("%s", buffer);
    if(strcmp(buffer, "sair") == 0)
    {
        dadosEnviar[0].stop = 0;
        dadosEnviar[1].stop = 0;
        pthread_kill(t[0], SIGUSR1);
        pthread_kill(t[1], SIGUSR1);
    }
    else
    {
        printf("\nComando invalido\n");
    }
}while(strcmp(buffer, "sair") != 0)

pthread_join(t[0], NULL);
pthread_join(t[1], NULL);
pthread_mutex_destroy(&mutex);