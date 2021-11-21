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
    
}

void especialistas()
{

}

void delut()
{

}

void delesp()
{

}

void freq()
{

}

void encerra()
{

}

void classificador(const char *sintomas)
{ 
    int e[2], r[2]; //escreve || recebe
    int filho;
    int estado, num;
    
    pipe(e);    //pipe para classificador
    pipe(r);    //pipe de classificador
    filho = fork();
    if (filho == 0) {
        close(STDIN_FILENO);
        dup(e[0]);  //duplica ponta de leitura
        close(e[0]);    //fecha extremidade de leitura
        close(e[1]);    //fecha extremidade de escrita

        close(STDOUT_FILENO);
        dup(r[1]);  //duplica ponta de escrita
        close(r[0]);    //fecha extremidade de leitura
        close(r[1]);    //fecha extremidade de escrita
        execl("classificador", "classificador",NULL);
    }
    close(e[0]);
    close(r[1]);

    char classificacao[31] = "";
    write(e[1], sintomas, 20);
    write(e[1], "\n", 1);
    read(r[0], classificacao, 30);

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

        //Lista Utentes
        if (strcmp(cmd, "utentes") == 0)
        {
            utentes(); //fazer funcao utentes!
        }

        //Lista Especialistas
        else if (strcmp(cmd, "especialistas") == 0)
        {
            especialistas(); //fazer funcao especialistas!
        }

        // Remove Utente
        else if (strcmp(cmd, "delut %c") == 0)
        {
            delut(); //fazer funcao delut!
        }

        //Remove Especialista
        else if (strcmp(cmd, "delesp %c") == 0)
        {
            delesp(); //fazer funcao delesp!
        }

        //Frequencia de refrescamento
        else if (strcmp(cmd, "freq %d") == 0)
        {
            freq(); //fazer funcao freq!
        }

        else if (strcmp(cmd, "encerra") == 0)
        {
            signal(SIGALRM, shutdown);
            encerra();//Necessário avisar os clientes, médicos e classificador!
            printf("SISTEMA MEDICALSO TERMINADO\n");
        }

        fflush(stdout);
        pthread_exit(NULL);
        sleep(1);
    }
}

char criaUtente()
{

    char utente;
    printf("Introduza o seu nome e o(s) seu(s) sintomas (separado por espaços):\n");
    scanf("%c", &utente);

    /*if(!strcmp(&nome,"ficheirocomosnomes")){
		printf("Nome ja existente! Tente outro");

		printf("Introduza o seu nome:\n");
        	scanf("%c", &nome);
	}*/
    //Isto não é código para estúpidos, não esperamos que o user meta o mesmo nome 2x

    if (!strcmp(&utente, "adeus"))
    {
        return 0;
    }
    return utente;
} // Envia dados para o balcao.c

char respondeMedico()
{

    char perguntaResposta;

    printf("Introduza a sua resposta:\n");
    scanf("%c", &perguntaResposta);

    return perguntaResposta;
}

char fichaUtente(char *especialidade, char *prioridade, int nUtentes, int nEspecialistas)
{
    if (especialidade == NULL || prioridade == NULL)
    {
        printf("Ocorreu um erro na receção de parâmetros!\n");
        return -1;
    }
    else
    {
        printf("Especialidade: %s\nPrioridade: %s\nnUtentes: %d\nnEspecialistas: %d\n", especialidade, prioridade, nUtentes, nEspecialistas);
    }
    return 0;
} // Recebe os parâmetros do balcao.c