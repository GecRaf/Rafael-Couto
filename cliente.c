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
    dataMSGCTL pergunta;
    dataRPLCTL resposta;
    cliente ctl;
    int fd_envio, fd_resposta;
    pergunta.pid = getpid();

    sprintf(CLIENTE_FIFO_FINAL, CLIENTE_FIFO, getpid());
    if (mkfifo(CLIENTE_FIFO_FINAL, 0666) == -1)
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
    printf("Nome de Utente: ");
    fflush(stdin);
    fgets(ctl.nome,100, stdin);
    //scanf("%s100[^\n]", ctl.nome);
    printf("\n");
    fflush(stdin);
    printf("Sintomas: ");
    fgets(ctl.sintomas,100, stdin);
    //scanf("%s100[^\n]", ctl.sintomas);
    printf("\n");

    do
    {
        fflush(stdin);
        printf("\n");
        printf("Cliente [%5d] - Mensagem: ", pergunta.pid);
        fgets(pergunta.msg,100, stdin);
        //scanf("%s100[^\n]", pergunta.msg);
        if (strcmp(pergunta.msg, "adeus") != 0)
        {
            fd_envio = open(SERVER_FIFO, O_WRONLY);
            int size = write(fd_envio, &pergunta, sizeof(pergunta));
            close(fd_envio);
            fd_resposta = open(CLIENTE_FIFO_FINAL, O_RDONLY);
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
            unlink(CLIENTE_FIFO_FINAL);
            exit(1); // como dar unlink aos dois fifos com o encerraServidor
        }

    } while (1);

    unlink(CLIENTE_FIFO_FINAL);

    /*printf("[%5d]COMANDO: ", pergunta.pid);
    fgets(str, 100, stdin);

    while (!strcmp(str, "adeus"))
    {
        if (pipe(fd) == -1)
        {
            printf("\n\n Nao foi possivel abrir o pipe\n\n");
            exit(2);
        }

        pid = fork();

        if (pid == -1)
        {
            printf("\n\nErro ao criar o filho\n\n");
            exit(3);
        }

        if (pid > 0)
        {
            pipe(fd[1]);
            close(STDOUT_FILENO);
            dup(fd[1]);
            close(fd[1]);
            close(fd[0]);
            execl(argv[1], argv[1], NULL);

        }

        if (pid == 0) // processo filho
        {
            pipe(fd[0]);
            close(STDIN_FILENO);
            dup(fd[0]);
            close(fd[0]);
            close(fd[1]);
            execl(argv[2], argv[2], NULL);
        }
    }
    printf("Adeus!\n");
    exit(1);*/

    // fichaUtente(especialidade, prioridade, nUtentes, nEspecialistas);
    // criaUtente();
    // respondeMedico();

    return 0;
}
