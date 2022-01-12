#include "utils.h"

void quit()
{
    printf("Saiu do sistema!");
    exit(EXIT_SUCCESS);
    sleep(1);
}

void utentes()
{
    printf("Not implemented yet!\n");
}

void especialistas()
{
    printf("Not implemented yet!\n");
}

void delut(int argum)
{
    printf("Not implemented yet!\n");
}

void delesp(int argum)
{
    printf("Not implemented yet!\n");
}

void freq(int argum)
{
    printf("Not implemented yet!\n");
}

void encerraServidor(int fd, char *nameFIFO)
{
    printf("Adeus!\n");
    close(fd);
    unlink(nameFIFO);
    exit(1);
}

void encerraSistema()
{
    char confirm;
    printf("\nTem a certeza que decide terminar o programa? [Y]- Yes [N] - No\n");
    scanf("%c", &confirm);
    if(confirm == 'Y' || confirm == 'y')
    {
    printf("Adeus...\n");
    sleep(3);
    unlink(SERVER_FIFO);
    exit(1);
    }
}

void classificador()
{
    int e[2], r[2]; // escreve || recebe
    int filho;
    int estado, num;
    int prioridade = 0;
    char especialidade[100];
    char temp[100];
    int filas[5] = {0, 0, 0, 0, 0};
    cliente ctl;

    // teste sintomas
    printf("Sintomas %s\n", ctl.sintomas);

    pipe(e); // pipe para classificador
    pipe(r); // pipe de classificador
    filho = fork();
    if (filho == 0)
    {
        close(STDIN_FILENO);
        dup(e[0]);   // duplica ponta de leitura
        close(e[0]); // fecha extremidade de leitura
        close(e[1]); // fecha extremidade de escrita

        close(STDOUT_FILENO);
        dup(r[1]);   // duplica ponta de escrita
        close(r[0]); // fecha extremidade de leitura
        close(r[1]); // fecha extremidade de escrita
        execl("classificador", "classificador", NULL);
    }
    close(e[0]);
    close(r[1]);

    char classificacao[31] = "";
    write(e[1], ctl.sintomas, 20);
    write(e[1], "\n", 1);
    int debug_read = read(r[0], classificacao, 30);
    if (debug_read == -1)
    {
        printf("Erro ao ler do classificador!\n");
        exit(1);
    }

    temp[debug_read] = '\0';

    sscanf(temp, "%s %d", especialidade, &prioridade);

    if (strcmp(especialidade, "geral") == 0 && filas[0] < 5)
    {
        strcpy(ctl.especialidade, especialidade);
        ctl.prioridade = prioridade;
        filas[0]++;
    }
    else
    {
        if (strcmp(especialidade, "ortopedia") == 0 && filas[1] < 5)
        {
            strcpy(ctl.especialidade, especialidade);
            ctl.prioridade = prioridade;
            filas[1]++;
        }
        else
        {
            if (strcmp(especialidade, "estomatologia") == 0 && filas[2] < 5)
            {
                strcpy(ctl.especialidade, especialidade);
                ctl.prioridade = prioridade;
                filas[2]++;
            }
            else
            {
                if (strcmp(especialidade, "neurologia") == 0 && filas[3] < 5)
                {
                    strcpy(ctl.especialidade, especialidade);
                    ctl.prioridade = prioridade;
                    filas[3]++;
                }
                else
                {
                    if (strcmp(especialidade, "oftalmologia") == 0 && filas[4] < 5)
                    {
                        strcpy(ctl.especialidade, especialidade);
                        ctl.prioridade = prioridade;
                        filas[4]++;
                    }
                    else
                    {
                        printf("Especialidade nao existe ou filas cheias!\n");
                    }
                }
            }
        }
    }

    printf("Classificacao: %s", classificacao);

    close(r[0]);
    kill(filho, SIGUSR2);
}

void *pipeCliente()
{
    char nfd;
    fd_set read_fds;
    struct timeval tv;
    char varSair[100];
    cliente clt;
    dataMSG mensagem;
    administrador adm;
    int fdRecebeCtl = open(SERVER_FIFO, O_RDONLY);
    if (fdRecebeCtl == -1)
    {
        printf("Erro1ctl!\n");
        exit(1);
    }
    do
    {    
            int size = read(fdRecebeCtl, &adm, sizeof(adm));
            if (size > 0)
            {
                printf("Cliente [%5d] - %s", adm.clt.pid, adm.mensagem.msg);
                printf("\n");
                sprintf(adm.clt.CLIENTE_FIFO_FINAL, CLIENTE_FIFO, adm.clt.pid);
                printf("\n%s\n", adm.clt.CLIENTE_FIFO_FINAL);
                /*int fdRecebeCtl = open(CLIENTE_FIFO_FINAL, O_WRONLY);
                if (fdRecebeCtl == -1 && strcmp(adm.mensagem.msg, "adeus") != 0)
                {
                    printf("fdRecebe = %d\n", fdRecebeCtl);
                    printf("Erro2ctl!\n");
                }
                int size2 = write(fdRecebeCtl, &adm.mensagem, sizeof(adm.mensagem.msg));
                close(fdRecebeCtl);
                fflush(stdin);*/
            }
        
    } while (1);
    classificador();
    printf("Especialidade: %s | Prioridade %d\n", clt.especialidade, clt.prioridade);
    encerraServidor(fdRecebeCtl, SERVER_FIFO);
    pthread_exit(NULL);
}

void *pipeMedico()
{
    char nfd;
    fd_set read_fds;
    struct timeval tv;
    char varSair[100];
    dataMSG mensagem;
    medico mdc;
    administrador adm;
    int fdRecebeMdc = open(SERVER_FIFO, O_RDONLY);
    if (fdRecebeMdc == -1)
    {
        printf("Erro1mdc!\n");
        exit(1);
    }
    do
    {
            int size = read(fdRecebeMdc, &adm, sizeof(adm));
            if (size > 0)
            {
                printf("Medico [%5d] - %s", adm.mdc.pid, adm.mensagem.msg);
                printf("\n");
                sprintf(adm.mdc.MEDICO_FIFO_FINAL, MEDICO_FIFO, adm.mdc.pid);
                int fdRecebeMdc = open(adm.mdc.MEDICO_FIFO_FINAL, O_WRONLY);
                if (fdRecebeMdc == -1 && strcmp(adm.mensagem.msg, "adeus") != 0)
                {
                    printf("fdRecebe = %d\n", fdRecebeMdc);
                    printf("Erro2mdc!\n");
                }
                int size2 = write(fdRecebeMdc, &adm.mensagem, sizeof(adm.mensagem));
                close(fdRecebeMdc);
                fflush(stdin);
            }
    } while (1);
    encerraServidor(fdRecebeMdc, SERVER_FIFO);
    pthread_exit(NULL);
}

void *comandos()
{
    char nfd;
    fd_set read_fds;
    struct timeval tv;
    char comando[50], cmd[15];
    int argum;
    int pid = getpid();
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);

    do 
    {
        printf("[%5d]COMANDO: ", pid);
        if (FD_ISSET(0, &read_fds))
        {
            fgets(comando, 100, stdin);
        }
        sscanf(comando, "%14s %29d[^\n]", cmd, &argum);

        // Chama classificador

        if (strcmp(cmd, "classificador") == 0)
        {
            classificador();
        }

        // Lista Utentes
        else if (strcmp(cmd, "utentes") == 0)
        {
            utentes(); //fazer funcao utentes!
        }

        // Lista Especialistas
        else if (strcmp(cmd, "especialistas") == 0)
        {
            especialistas(); //fazer funcao especialistas!
        }

        // Remove Utente
        else if (strcmp(cmd, "delut") == 0)
        {
            delut(argum); //fazer funcao delut!
        }

        // Remove Especialista
        else if (strcmp(cmd, "delesp") == 0)
        {
            delesp(argum); //fazer funcao delesp!
        }

        // Frequencia de refrescamento
        else if (strcmp(cmd, "freq") == 0)
        {
            freq(argum); //fazer funcao freq!
        }

        else if (strcmp(cmd, "encerra") == 0)
        {
            encerraSistema();//Necessário avisar os clientes, médicos e classificador!
            printf("SISTEMA MEDICALSO TERMINADO\n");
        }
        else
        {
            printf("Comando invalido!\n");
        }
    }
    while(1);
    fflush(stdout);
    pthread_exit(NULL);
    sleep(1);
}