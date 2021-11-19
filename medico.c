#include "medico.h"

int main (int argc, char *argv[]){

    char str[40];
    int aux = 1, pid, son;
    int fd[2];

    if(argc < 3)
    {
        printf("\n\n Faltam parametros \n\n");
        exit(1);
    }
    // Mesma situacao do cliente.c 
    if(pipe(fd) == -1)
    {
        printf("\n\n Nao foi possivel abrir o pipe\n\n");
        exit(2);
    }

    pid = fork();

    if(pid == -1)
    {
        printf("\n\nErro ao criar o filho\n\n");
        exit(3);
    }
    
    if(pid > 0)
    {
        close(STDIN_FILENO);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        execl(argv[2], argv[2], NULL);
    }

    if(pid == 0)// processo filho
    {
        close(STDOUT_FILENO);
        dup(fd[1]);
        close(fd[1]);
        close(fd[0]);
        execl(argv[1], argv[1], NULL);
    }
    return 0;
}

char respondeUtente()
{

    char perguntaResposta;

    printf("Introduza a sua pergunta:\n");
    scanf("%c", &perguntaResposta);

    if (!strcmp(&perguntaResposta, "adeus"))
    {
        return 0;
    }

    return perguntaResposta;
}