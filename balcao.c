#include "balcao.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(SERVER_FIFO);
    printf("\nAdeus\n");
    exit(1);
}

int main(int argc, char *argv)
{
    pthread_t t[3];
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    printf("SISTEMA MEDICALSO INICIADO\n");

    //vars ambiente
    int maxClientes = atoi(getenv("MAXCLIENTES"));
    int maxMedicos = atoi(getenv("MAXCLIENTES"));

    if(getenv("MAXCLIENTES") == NULL)
    {
        printf("Variavel de ambiente MAXCLIENTES nao existe!\n");
        return 0;
    }

    if(getenv("MAXMEDICOS") == NULL)
    {
        printf("Variavel de ambiente MAXMEDICOS nao existe!\n");
        return 0;
    }


    //criação fifo

    if (mkfifo(SERVER_FIFO, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("FIFO ja existe!\n");
        }
        printf("Erro ao abrir fifo!\n");
        return 1;
    }

    //criacao de threads
    if(pthread_create(&t[0], NULL, &pipeCliente, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Cliente!");
        exit(-1);
    }

    if(pthread_create(&t[1], NULL, &pipeMedico, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Medico!\n");
        exit(-1);
    }

    if(pthread_create(&t[2], NULL, &comandos, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Comandos!\n");
        exit(-1);
    }

    int fdRecebeMdc = open(SERVER_FIFO, O_RDONLY);
    if (fdRecebeMdc == -1)
    {
        printf("Erro1mdc!\n");
        exit(1);
    }

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);
}