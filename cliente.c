#include "cliente.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(CLIENTE_FIFO);
    unlink(CLIENTE_FIFO_FINAL);
    printf("\nAdeus\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    dataMSG mensagem;
    cliente ctl;
    administrador adm;
    int fd_envio, fd_resposta;
    mensagem.pid = getpid();

    sprintf(ctl.CLIENTE_FIFO_FINAL, CLIENTE_FIFO, getpid());
    if (mkfifo(ctl.CLIENTE_FIFO_FINAL, 0666) == -1)
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
    fgets(ctl.nome, 100, stdin);
    printf("\n");
    fflush(stdin);
    printf("Sintomas: ");
    fgets(ctl.sintomas, 100, stdin);
    printf("\n");

    adm.userType = true;
    adm.clt = ctl;

    int fd_balcao = open(SERVER_FIFO, O_WRONLY);
    int size = write(fd_balcao, &adm, sizeof(adm));

    do
    {
        fd_resposta = open(ctl.CLIENTE_FIFO_FINAL, O_RDONLY);
        int size2 = read(fd_resposta, &mensagem, sizeof(mensagem));
        close(fd_envio);
        fflush(stdin);
        printf("\n");
        printf("Cliente [%5d] - Mensagem: ", mensagem.pid);
        fgets(mensagem.msg, 100, stdin);
        if (strcmp(mensagem.msg, "adeus") == 0)
            break;
        fd_envio = open(ctl.CLIENTE_FIFO_FINAL, O_WRONLY);
        int size = write(fd_envio, &mensagem, sizeof(mensagem));
        if (size2 > 0)
        {
            close(fd_resposta);
            printf("\nResposta: %s\n", mensagem.msg);
        }
    } while (1);

    close(fd_envio);
    unlink(CLIENTE_FIFO_FINAL);
    exit(1);
}
