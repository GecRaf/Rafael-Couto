#include "utils.h"

void handler_sigalrm(int s, siginfo_t *i, void *v)
{
    unlink(SERVER_FIFO);
    printf("\nAdeus\n");
    exit(1);
}

int flg = 0, flg2 = 0;

void *userTypeTrue(void *args)
{
    administrador *adm = (administrador *)args;
    pthread_t t;
    flg2 = 1;
    printf("Cli->%d->%d\n", flg2, flg);

    while (flg == 0)
        ;
    if (mkfifo(Read_Cli, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("FIFO ja existe!\n");
        }
        printf("Erro ao abrir fifo!\n");
        exit(-1);
    }

    int fd_read3 = open(Read_Cli, O_RDONLY);
    int size4 = read(fd_read3, &adm->mdc, sizeof(adm->mdc));
    int fd_write3 = open(Write_Cli, O_WRONLY);
    int size5 = write(fd_write3, &adm->clt, sizeof(adm->clt));
    printf("Med no Cliente: %s\n", adm->mdc.MEDICO_FIFO_FINAL);

    int fd_write = open(adm->mdc.MEDICO_FIFO_FINAL, O_WRONLY);
    int size3 = write(fd_write, &adm->clt, sizeof(adm->clt));
    close(fd_write);

    printf("\nMEDICO_FIFO_FINAL: %s\n", adm->mdc.MEDICO_FIFO_FINAL);
    printf("Entrei cliente!\n");
    if (pthread_create(&t, NULL, &pipeCliente, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Cliente!");
        exit(-1);
    }
}

void *userTypeFalse(void *args)
{
    administrador *adm = (administrador *)args;
    pthread_t t;

    flg = 1;
    printf("->%d->%d\n", flg2, flg);
    while (flg2 == 0)
        ;

    if (mkfifo(Write_Cli, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("FIFO ja existe!\n");
        }
        printf("Erro ao abrir fifo!\n");
        exit(-1);
    }

    int fd_write = open(Read_Cli, O_WRONLY);
    int size4 = write(fd_write, &adm->mdc, sizeof(adm->mdc));
    int fd_read = open(Write_Cli, O_RDONLY);
    int size6 = read(fd_read, &adm->clt, sizeof(adm->clt));
    printf("Cliente no Med: %s\n", adm->clt.CLIENTE_FIFO_FINAL);

    int fd_write2 = open(adm->clt.CLIENTE_FIFO_FINAL, O_WRONLY);
    int size5 = write(fd_write2, &adm->mdc, sizeof(adm->mdc));
    close(fd_write2);
    printf("CLIENTE_FIFO_FINAL: %s", adm->clt.CLIENTE_FIFO_FINAL);
    printf("Entrei medico!\n");
    if (pthread_create(&t, NULL, &pipeMedico, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Medico!\n");
        exit(-1);
    }
}

int main(int argc, char *argv)
{
    pthread_t t[3];
    struct sigaction sa;
    sa.sa_sigaction = handler_sigalrm;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    administrador adm;
    cliente clt;
    medico mdc;
    adm.pidAdm = getpid();
    adm.clt.flag = 0;
    adm.mdc.flag = 0;

    printf("SISTEMA MEDICALSO INICIADO\n");

    // vars ambiente
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

    // criação fifo

    if (mkfifo(SERVER_FIFO, 0666) == -1)
    {
        if (errno == EEXIST)
        {
            printf("FIFO ja existe!\n");
        }
        printf("Erro ao abrir fifo!\n");
        return 1;
    }

    if (pthread_create(&t[0], NULL, &comandos, NULL) != 0)
    {
        printf("Erro ao criar e lancar a thread do Comandos!\n");
        exit(-1);
    }

    int fd_read = open(SERVER_FIFO, O_RDONLY);
    if (fd_read == -1)
    {
        printf("Erro ao abrir o fifo!\n");
        exit(1);
    }
        int sizeClt = read(fd_read, &clt, sizeof(clt));
        adm.clt = clt;
        printf("\nNome Cliente: %s", clt.nome);
        printf("\nSintomas Cliente: %s", clt.sintomas);
        printf("\nFlag Cliente: %d", clt.flag);
        printf("\nFIFO Cliente: %s", clt.CLIENTE_FIFO_FINAL);
        int sizeMdc = read(fd_read, &mdc, sizeof(mdc));
        adm.mdc = mdc;
        printf("\nNome Medico: %s", mdc.nome);
        printf("\nEspecialidade Medico: %s", mdc.especialidade);
        printf("\nFlag Medico: %d", mdc.flag);
        printf("\nFIFO Medico: %s\n", mdc.MEDICO_FIFO_FINAL);

    do
    {
        
        int size = read(fd_read, &adm, sizeof(adm));
        if (size > 0)
        {
            if (adm.userType)
            {
                if (adm.mdc.flag == 1)
                {
                    int fd_writeMedico = open(adm.mdc.MEDICO_FIFO_FINAL, O_WRONLY);
                    int sizeWriteMedico = write(fd_writeMedico, &adm.clt, sizeof(adm.clt));
                    close(fd_writeMedico);
                }
            }
            if (!adm.userType)
            {  
                if (adm.clt.flag == 1)
                {
                    int fd_writeCliente = open(adm.clt.CLIENTE_FIFO_FINAL, O_RDONLY);
                    int sizeWriteCliente = write(fd_writeCliente, &adm.mdc, sizeof(adm.mdc));
                    close(fd_writeCliente);
                }
            }
        }
    } while (1);
    // importante separar as threads para no caso de querer dar kill a um em especifico
    if (pthread_join(t[0], NULL) == -1)
        exit(-1);
    unlink(Read_Cli);
    unlink(Write_Cli);
}