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

char classificador(const char *sintomas)
{
    char classificacao[30];

    const char *comando = "./classificador";
    execvp(comando, NULL);
    fgets(sintomas, 100, stdin);
    fclose(classificador);
    fgets(classificacao, 100, stdout);
    //Falta implementar stdin e stdout
    return classificacao;
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

    while (1)
    {

        printf("[%5d][BALCAO] COMANDO:\n");
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