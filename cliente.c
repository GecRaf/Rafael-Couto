#include "cliente.h"

int main(int argc, char *argv[])
{
    char str[40];
    int aux = 1, son;
    int fd[2];
    int pid = getpid();

    printf("[%5d]COMANDO: ", pid);
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
    exit(1);
    //Falta implementar fork e exec para comunicacao entre medico e cliente
    /*if(argc < 3)
    {
        printf("\n\n Faltam parametros \n\n");
        exit(1);
    }*/

    /*while (aux) //Para rever
    {
        if (id == 0)
        {
            close(fd[0]);
            printf(" [%5d]Resposta: ", pid);
            fflush(stdout);
            scanf("%s", str);
            if(write(fd[1], &str, sizeof(str)) == -1) {return 1;}
            close(fd[1]);
            if (strcmp(str, "adeus") != 0)
            {
                son = fork();
                if (son == 0)
                {
                    pid = getpid();
                }
            }
            else
            {
                int strChild;
                aux = 0;
                close(fd[1]);
                if(read(fd[0], &strChild, sizeof(strChild)) == -1){return 2;}
                close(fd[0]);
            }
        }
    }*/

    //fichaUtente(especialidade, prioridade, nUtentes, nEspecialistas);
    criaUtente();
    respondeMedico();

    return 0;
}



