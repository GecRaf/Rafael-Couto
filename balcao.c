#include "balcao.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(SERVER_FIFO);
    printf("\nAdeus\n");
    exit(1);
}

void *pipeCliente(){
    char nfd;
    fd_set read_fds;
    struct timeval tv;
    char varSair[100];
    dataMSGCTL perguntaCTL;
    dataRPLCTL respostaCTL;

    int fdRecebeCtl = open(SERVER_FIFO, O_RDWR);
    if (fdRecebeCtl == -1)
    {
        printf("Erro!\n");
        exit(1);
    }
    do
    {
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(fdRecebeCtl, &read_fds);
        nfd = select(fdRecebeCtl + 1, &read_fds, NULL, NULL, &tv);
        if(nfd == 0)
        {
            printf("A aguardar informacao... \n");
        }
        //printf("Resposta: %d\n", nfd);
        if (FD_ISSET(0, &read_fds))
        {
            fgets(varSair, 99, stdin);
            if (strcmp(varSair, "encerra\n") == 0)
            {
                encerraServidor(fdRecebeCtl, SERVER_FIFO);
            }
            else
            {
                printf("Comando invalido\n");
            }
        }

        if (FD_ISSET(fdRecebeCtl, &read_fds))
        {
            int size = read(fdRecebeCtl, &perguntaCTL, sizeof(perguntaCTL));
            if (size > 0)
            {
                printf("Cliente [%5d] - %s", perguntaCTL.pid, perguntaCTL.msg);
                printf("\n");
                // Por implementar o envio para o Medico

                //strcpy(resposta.res, "Mensagem do medico"); // A mensagem que recebe e a resposta do medico

                sprintf(CLIENTE_FIFO_FINAL, CLIENTE_FIFO, perguntaCTL.pid);
                int fdRecebeCtl = open(CLIENTE_FIFO_FINAL, O_WRONLY);
                if (fdRecebeCtl == -1 && strcmp(perguntaCTL.msg, "adeus") != 0)
                {
                    printf("Erro!\n");
                }
                int size2 = write(fdRecebeCtl, &respostaCTL, sizeof(respostaCTL));
                close(fdRecebeCtl);
            }
        }
    } while (1);
    encerraServidor(fdRecebeCtl, SERVER_FIFO);
    pthread_exit(NULL);
}

void *pipeMedico(){
    char nfd;
    fd_set read_fds;
    struct timeval tv;
    char varSair[100];
    dataMSGMDC perguntaMDC;
    dataRPLMDC respostaMDC;

    int fdRecebeMdc = open(SERVER_FIFO, O_RDWR);
    if (fdRecebeMdc == -1)
    {
        printf("Erro!\n");
        exit(1);
    }
    do
    {
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(fdRecebeMdc, &read_fds);
        nfd = select(fdRecebeMdc + 1, &read_fds, NULL, NULL, &tv);
        if(nfd == 0)
        {
            printf("A aguardar informacao... \n");
        }
        //printf("Resposta: %d\n", nfd);
        if (FD_ISSET(0, &read_fds))
        {
            fgets(varSair, 99, stdin);
            if (strcmp(varSair, "encerra\n") == 0)
            {
                encerraServidor(fdRecebeMdc, SERVER_FIFO);
            }
            else
            {
                printf("Comando invalido\n");
            }
        }

        if (FD_ISSET(fdRecebeMdc, &read_fds))
        {
            int size = read(fdRecebeMdc, &perguntaMDC, sizeof(perguntaMDC));
            if (size > 0)
            {
                printf("Medico [%5d] - %s", perguntaMDC.pid, perguntaMDC.msg);
                printf("\n");
                // Por implementar o envio para o Medico

                //strcpy(resposta.res, "Mensagem do medico"); // A mensagem que recebe e a resposta do medico

                sprintf(MEDICO_FIFO_FINAL, MEDICO_FIFO, perguntaMDC.pid);
                int fdRecebeMdc = open(MEDICO_FIFO_FINAL, O_WRONLY);
                if (fdRecebeMdc == -1 && strcmp(perguntaMDC.msg, "adeus") != 0)
                {
                    printf("Erro!\n");
                }
                int size2 = write(fdRecebeMdc, &respostaMDC, sizeof(respostaMDC));
                close(fdRecebeMdc);
            }
        }
    } while (1);
    encerraServidor(fdRecebeMdc, SERVER_FIFO);
    pthread_exit(NULL);
}

int main(int argc, char *argv)
{
    pthread_t t[2];
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    if (mkfifo(SERVER_FIFO, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("FIFO ja existe!\n");
        }
        printf("Erro ao abrir fifo!\n");
        return 1;
    }

    printf("SISTEMA MEDICALSO INICIADO\n");

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

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
}