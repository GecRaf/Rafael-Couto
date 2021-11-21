#include "balcao.h"


int main(int argc, char *argv)
{
    /*char* maxClientesChar = getenv("MAXCLIENTES");
    int maxClientes = atoi(maxClientesChar);

    char* maxMedicosChar = getenv("MAXMEDICOS");
    int maxMedicos = atoi(maxMedicosChar);
    
    char* maxEspecialidadesChar = getenv("MAXESPECIALIDADES");
    int maxEspecialidades = atoi(maxEspecialidadesChar);

    char* maxLugaresFilaChar = getenv("MAXLUGARESFILA");
    int maxLugaresFila = atoi(maxLugaresFilaChar);*/ //Segmentation fault code

    /*if(0 == system("pidof -x balcao > /dev/null")) {
        printf("O processo balcao ja esta em execucao!\n");
      //A process having name PROCESS is running.
   }
    if(1 == system("pidof -x balcao > /dev/null")) {
       printf("O processo balcao nao esta em execucao!\n");
      //A process having name PROCESS is NOT running.
   }*/// Por implementar

    printf("SISTEMA MEDICALSO INICIADO\n");

    //comandos();

    const char *sintomas = "dor de cabeca"; //teste sintomas
    classificador(sintomas);

    return 0;
}

