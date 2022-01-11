#include "medico.h"
#include "cliente.h"
#include "balcao.h"

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

void delut()
{
    printf("Not implemented yet!\n");
}

void delesp()
{
    printf("Not implemented yet!\n");
}

void freq()
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
    printf("Not implemented yet!\n");
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

void shutdown(int pid)
{
    printf("\nSistema a terminar...\n");
    fflush(stdout);
    kill(pid, SIGUSR1);
    sleep(3);
}

char comandos()
{
    char comando[50], cmd[15], argum[30];
    int pid = getpid();

    while (1)
    {

        printf("[%5d]COMANDO: ", pid);
        scanf("%49[^\n]", comando);
        sscanf(comando, "%14s %29[^\n]", cmd, argum);

        //Chama classificador

        if(strcmp(cmd, "classificador") == 0)
        {
            classificador();
        }

        //Lista Utentes
        if (strcmp(cmd, "utentes") == 0)
        {
            //utentes(); //fazer funcao utentes!
        }

        //Lista Especialistas
        else if (strcmp(cmd, "especialistas") == 0)
        {
            //especialistas(); //fazer funcao especialistas!
        }

        // Remove Utente
        else if (strcmp(cmd, "delut %c") == 0)
        {
           //delut(); //fazer funcao delut!
        }

        //Remove Especialista
        else if (strcmp(cmd, "delesp %c") == 0)
        {
            //delesp(); //fazer funcao delesp!
        }

        //Frequencia de refrescamento
        else if (strcmp(cmd, "freq %d") == 0)
        {
            //freq(); //fazer funcao freq!
        }

        else if (strcmp(cmd, "encerra") == 0)
        {
            signal(SIGALRM, shutdown);
            //encerraSistema();//Necessário avisar os clientes, médicos e classificador!
            printf("SISTEMA MEDICALSO TERMINADO\n");
        }

        fflush(stdout);
        pthread_exit(NULL);
        sleep(1);
    }
}