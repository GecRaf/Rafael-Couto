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
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    dataMSGMDC pergunta;
    dataRPLMDC resposta;
    medico mdc;
    int fd_envio, fd_resposta;
    pergunta.pid = getpid();

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
    fgets(mdc.nome,100, stdin);
    //scanf("%s100[^\n]", ctl.nome);
    printf("\n");
    fflush(stdin);
    printf("Especialidade: ");
    fgets(mdc.especialidade,100, stdin);
    //scanf("%s100[^\n]", ctl.sintomas);
    printf("\n");

    do
    {
        fflush(stdin);
        printf("\n");
        printf("Medico [%5d] - Mensagem: ", pergunta.pid);
        fgets(pergunta.msg,100, stdin);
        //scanf("%s100[^\n]", pergunta.msg);
        if (strcmp(pergunta.msg, "adeus") != 0)
        {
            fd_envio = open(SERVER_FIFO, O_WRONLY);
            int size = write(fd_envio, &pergunta, sizeof(pergunta));
            close(fd_envio);
            fd_resposta = open(MEDICO_FIFO_FINAL, O_RDONLY);
            int size2 = read(fd_resposta, &resposta, sizeof(resposta));
            if (size2 > 0)
            {
                close(fd_resposta);
                printf("\nResposta: %s\n", resposta.res); //%s?
            }  
        }
        else
        {
            fd_envio = open(SERVER_FIFO, O_WRONLY);
            int size = write(fd_envio, &pergunta, sizeof(pergunta));
            close(fd_envio);
            unlink(MEDICO_FIFO_FINAL);
            exit(1); // como dar unlink aos dois fifos com o encerraServidor
        }

    } while (1);

    unlink(MEDICO_FIFO_FINAL);

    return 0;
}