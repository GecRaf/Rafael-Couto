#include "balcao.h"


int main(int argc, char *argv)
{

    /*if(0 == system("pidof -x balcao > /dev/null")) {
        printf("O processo balcao ja esta em execucao!\n");
      //A process having name PROCESS is running.
   }
    if(1 == system("pidof -x balcao > /dev/null")) {
       printf("O processo balcao nao esta em execucao!\n");
      //A process having name PROCESS is NOT running.
   }*/

    printf("SISTEMA MEDICALSO INICIADO\n");

    //comandos();

    const char *sintomas = "dor de cabeca";

    classificador(sintomas);

    return 0;
}

