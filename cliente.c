#include "utils.h"

administrador adm;

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(CLIENTE_FIFO);
    unlink(adm.clt.CLIENTE_FIFO_FINAL);
    printf("\nAdeus\n");
    exit(1);
}

void *leitura(void *args)
{
    printf("Nome fifo: %s", adm.clt.CLIENTE_FIFO_FINAL);
    char buffer[100];
    do
    {
        int fd_read = open(adm.clt.CLIENTE_FIFO_FINAL, O_RDONLY | O_NONBLOCK);
        int size = read(fd_read, &buffer, sizeof(buffer));
        if (size > 0)
        {
            close(fd_read);
            printf("Cliente [%5d] - Mensagem: %s", adm.clt.pid, buffer);
            strcpy(buffer, "");
        }
    } while (1);
}

void *escrita(void *args)
{
    printf("Nome fifo: %s\n", adm.mdc.MEDICO_FIFO_FINAL);
    do
    {
        printf("Cliente [%5d] - Mensagem: ", adm.clt.pid);
        fgets(adm.mensagem.msg, 100, stdin);
        if (strcmp(adm.mensagem.msg, "adeus") == 0)
            printf("fazer cebas");
        int fd_write = open(adm.mdc.MEDICO_FIFO_FINAL, O_WRONLY);
        int size = write(fd_write, &adm.mensagem.msg, sizeof(adm.mensagem.msg));
        close(fd_write);
    } while (1);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    dataMSG mensagem;
    cliente clt;
    int fd_envio, fd_resposta;
    adm.userType = true;
    adm.clt.pid = getpid();
    pthread_t t;

    sprintf(adm.clt.CLIENTE_FIFO_FINAL, CLIENTE_FIFO, getpid());
    if (mkfifo(adm.clt.CLIENTE_FIFO_FINAL, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("\nFIFO ja existe!\n");
        }
        printf("\nErro ao abrir fifo!\n");
        return 1;
    }

    printf("\nBem-vindo ao sistema MEDICALso!\n");
    printf("Nome de Utente: ");
    fflush(stdin);
    fgets(clt.nome, 100, stdin);
    printf("\n");
    fflush(stdin);
    printf("Sintomas: ");
    fgets(clt.sintomas, 100, stdin);
    printf("\n");

    strcpy(adm.clt.nome, clt.nome);
    strcpy(adm.clt.sintomas, clt.sintomas);

    int fd_balcao = open(SERVER_FIFO, O_WRONLY);
    int size = write(fd_balcao, &adm, sizeof(adm));
    close(fd_balcao);

    int fd_read = open(adm.clt.CLIENTE_FIFO_FINAL, O_RDONLY);
    int size2 = read(fd_read, &adm.clt, sizeof(adm.clt));

    if(pthread_create(&t, NULL, &escrita, NULL))
    {
        printf("Erro ao criar e lancar a thread de escrita!");
        exit(-1);
    }

    if(pthread_create(&t, NULL, &leitura, NULL))
    {
        printf("Erro ao criar e lancar a thread de leitura!");
        exit(-1);
    }

    pthread_join(t, NULL);

    /*do
    {
        fd_resposta = open(clt.CLIENTE_FIFO_FINAL, O_RDONLY | O_NONBLOCK);
        int size2 = read(fd_resposta, &adm.mensagem, sizeof(adm.mensagem));
        close(fd_envio);
        fflush(stdin);
        printf("\n");
        printf("Cliente [%5d] - Mensagem: ", adm.clt.pid);
        fgets(adm.mensagem.msg, 100, stdin);
        if (strcmp(adm.mensagem.msg, "adeus") == 0)
            break;
        fd_envio = open(clt.CLIENTE_FIFO_FINAL, O_WRONLY);
        int size = write(fd_envio, &adm.mensagem, sizeof(adm.mensagem));
        if (size2 > 0)
        {
            close(fd_resposta);
            printf("\nResposta: %s\n", adm.mensagem.msg);
        }
    } while (1);*/

    close(fd_envio);
    unlink(adm.clt.CLIENTE_FIFO_FINAL);
    exit(1);
}
