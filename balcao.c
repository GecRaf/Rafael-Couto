#include "utils.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(SERVER_FIFO);
    printf("\nAdeus\n");
    exit(1);
}

int main(int argc, char *argv)
{
    pthread_t t;
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    administrador adm;
    adm.pidAdm = getpid();

    printf("SISTEMA MEDICALSO INICIADO\n");

    //vars ambiente
    /*int maxClientes = atoi(getenv("MAXCLIENTES"));
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
    }*/

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

    if (pthread_create(&t, NULL, &comandos, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Comandos!\n");
        exit(-1);
    }

    int fd_read = open(SERVER_FIFO, O_RDONLY | O_NONBLOCK);
    if (fd_read == -1)
    {
        printf("Erro ao abrir o fifo!\n");
        exit(1);
    }

    
    do
    {
        int size = read(fd_read, &adm, sizeof(adm));
        if (size > 0)
        {
            if (adm.userType == true)
            {
                int fd_write = open(adm.mdc.MEDICO_FIFO_FINAL, O_WRONLY);
                int size2 = write(fd_write, &adm.clt, sizeof(adm.clt));
                printf("Entrei cliente!\n");
                if (pthread_create(&t, NULL, &pipeCliente, NULL) != 0)
                {
                    printf("Erro ao criar e lancar a thread do Cliente!");
                    exit(-1);
                }
            }
            else
            {
                int fd_write2 = open(adm.clt.CLIENTE_FIFO_FINAL, O_WRONLY);
                int size3 = write(fd_write2, &adm.mdc, sizeof(adm.mdc));
                printf("Entrei medico!\n");
                if (pthread_create(&t, NULL, &pipeMedico, NULL) != 0)
                {
                    printf("Erro ao criar e lancar a thread do Medico!\n");
                    exit(-1);
                }
            }
        }
    } while (1);

    pthread_join(t, NULL);
}