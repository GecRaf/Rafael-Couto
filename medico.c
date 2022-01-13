#include "utils.h"

administrador adm;

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(MEDICO_FIFO);
    unlink(adm.mdc.MEDICO_FIFO_FINAL);
    printf("\nAdeus\n");
    exit(1);
}

void *leitura(void *args)
{
    printf("\nFifo leitura: %s\n", adm.mdc.MEDICO_FIFO_FINAL);
    char buffer[100];
    do
    {
        int fd_read = open(adm.mdc.MEDICO_FIFO_FINAL, O_RDONLY | O_NONBLOCK);
        int size = read(fd_read, &buffer, sizeof(buffer));
        if (size > 0)
        {
            close(fd_read);
            printf("Medico [%5d] - Mensagem: %s", adm.mdc.pid, buffer);
            strcpy(buffer, "");
        }
    } while (1);
}

void *escrita(void *args)
{
    printf("\nFifo escrita: %s\n", adm.clt.CLIENTE_FIFO_FINAL);
    do
    {
        printf("Medico [%5d] - Mensagem: ", adm.mdc.pid);
        fgets(adm.mensagem.msg, 100, stdin);
        if (strcmp(adm.mensagem.msg, "adeus") == 0)
            printf("fazer cebas");
        int fd_write = open(adm.clt.CLIENTE_FIFO_FINAL, O_WRONLY);
        int size = write(fd_write, &adm.mensagem.msg, sizeof(adm.mensagem.msg));
        close(fd_write);
    } while (1);
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
    int fd_envio, fd_resposta;
    adm.userType = false;
    adm.mdc.flag = 1;
    adm.mdc.pid = getpid();
    pthread_t t;

    sprintf(adm.mdc.MEDICO_FIFO_FINAL, MEDICO_FIFO, getpid());
    if (mkfifo(adm.mdc.MEDICO_FIFO_FINAL, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("\nFIFO ja existe!\n");
        }
        printf("\nErro ao abrir fifo!\n");
        return 1;
    }

    printf("\nBem-vindo ao sistema MEDICALso!\n");
    printf("Nome de Medico: ");
    fflush(stdin);
    fgets(mdc.nome, 100, stdin);
    printf("\n");
    fflush(stdin);
    printf("Especialidade: ");
    fgets(mdc.especialidade, 100, stdin);
    printf("\n");

    //strcpy(adm.mdc.nome, mdc.nome);
    //strcpy(adm.mdc.especialidade, mdc.especialidade);

    int fd_balcao = open(SERVER_FIFO, O_WRONLY);
    int size = write(fd_balcao, &mdc, sizeof(mdc));
    close(fd_balcao);
    printf("galinha\n");
    //int fd_read = open(adm.mdc.MEDICO_FIFO_FINAL, O_RDONLY);
    //int size2 = read(fd_read, &adm.clt, sizeof(adm.clt));
    printf("maiu\n");
    
    /*do
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        
        fd_balcao = open(SERVER_FIFO, O_RDONLY | O_NONBLOCK);
        int size2 = read(fd_balcao, &adm.clt, sizeof(&adm.clt));
        if (size2 > 0)
        {
            do
            {
                fflush(stdin);
                printf("\n");
                printf("Medico [%5d] - Mensagem: ", adm.mdc.pid);
                fgets(adm.mensagem.msg, 100, stdin);
                if(strcmp(adm.mensagem.msg, "sair"))
                {
                    strcpy(varSair, adm.mensagem.msg);
                    break;
                }
                if (strcmp(adm.mensagem.msg, "adeus") != 0)
                {
                    fd_envio = open(CLIENTE_FIFO, O_WRONLY);
                    int size = write(fd_envio, &adm.mensagem, sizeof(adm.mensagem));
                    close(fd_envio);
                    fd_resposta = open(adm.mdc.MEDICO_FIFO_FINAL, O_RDONLY);
                    int size2 = read(fd_resposta, &adm.mensagem, sizeof(adm.mensagem));
                    if (size2 > 0)
                    {
                        close(fd_resposta);
                        printf("\nResposta: %s\n", adm.mensagem.msg); //%s?
                    }
                }
                else
                {
                    strcpy(adm.mensagem.msg, "A consulta terminou!\n");
                    fd_envio = open(SERVER_FIFO, O_WRONLY);
                    int size = write(fd_envio, &adm.mensagem, sizeof(adm.mensagem));
                    close(fd_envio);
                    unlink(adm.mdc.MEDICO_FIFO_FINAL);
                    break; // como dar unlink aos dois fifos com o encerraServidor
                }
            } while (1);
        }
        if (FD_ISSET(0, &read_fds))
        {
            fgets(varSair, 100, stdin);
        }
        
    } while (strcmp(varSair, "sair") != 0);*/

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
    unlink(adm.mdc.MEDICO_FIFO_FINAL);
    //close(fd_read);
    close(fd_balcao);
}