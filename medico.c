#include "medico.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(MEDICO_FIFO);
    unlink(MEDICO_FIFO_FINAL);
    printf("\nAdeus\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    char varSair[100];
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    dataMSG mensagem;
    medico mdc;
    administrador adm;
    int fd_envio, fd_resposta;
    mensagem.pid = getpid();

    sprintf(MEDICO_FIFO_FINAL, MEDICO_FIFO, getpid());
    if (mkfifo(MEDICO_FIFO_FINAL, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("\nFIFO ja existe!\n");
        }
        printf("\nErro ao abrir fifo!\n");
        return 1;
    }

    // printf("Cliente [%5d]\n", pergunta.pid); //Maybe not necessary
    printf("\nBem-vindo ao sistema MEDICALso!\n");
    printf("Nome de Medico: ");
    fflush(stdin);
    fgets(mdc.nome, 100, stdin);
    printf("\n");
    fflush(stdin);
    printf("Especialidade: ");
    fgets(mdc.especialidade, 100, stdin);
    printf("\n");

    adm.userType = false;
    adm.mdc = mdc;

    int fd_balcao = open(SERVER_FIFO, O_WRONLY);
    int size = write(fd_balcao, &adm, sizeof(adm));
    close(fd_balcao);
    
    do
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        
        fd_balcao = open(SERVER_FIFO, O_RDONLY);
        int size2 = read(fd_balcao, &adm.clt, sizeof(&adm.clt));
        if (size2 > 0)
        {
            do
            {
                fflush(stdin);
                printf("\n");
                printf("Medico [%5d] - Mensagem: ", mensagem.pid);
                fgets(mensagem.msg, 100, stdin);
                if(strcmp(mensagem.msg, "sair"))
                {
                    strcpy(varSair, mensagem.msg);
                    break;
                }
                if (strcmp(mensagem.msg, "adeus") != 0)
                {
                    fd_envio = open(CLIENTE_FIFO, O_WRONLY);
                    int size = write(fd_envio, &mensagem, sizeof(mensagem));
                    close(fd_envio);
                    fd_resposta = open(MEDICO_FIFO_FINAL, O_RDONLY);
                    int size2 = read(fd_resposta, &mensagem, sizeof(mensagem));
                    if (size2 > 0)
                    {
                        close(fd_resposta);
                        printf("\nResposta: %s\n", mensagem.msg); //%s?
                    }
                }
                else
                {
                    strcpy(mensagem.msg, "A consulta terminou!\n");
                    fd_envio = open(SERVER_FIFO, O_WRONLY);
                    int size = write(fd_envio, &mensagem, sizeof(mensagem));
                    close(fd_envio);
                    unlink(MEDICO_FIFO_FINAL);
                    break; // como dar unlink aos dois fifos com o encerraServidor
                }
            } while (1);
        }
        if (FD_ISSET(0, &read_fds))
        {
            fgets(varSair, 100, stdin);
        }
        
    } while (strcmp(varSair, "sair") != 0);

    unlink(MEDICO_FIFO_FINAL);
    close(fd_balcao);
}