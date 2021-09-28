#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double aleatorio() {
    double u = rand() / ((double) RAND_MAX + 1);
    //limitando entre (0,1]
    u = 1.0 - u;

    return (u);
}

int main() {
    /****************************
     * Declaração das variáveis *
     ****************************/

    /* Gerar números pseudoaleatórios
     * com média igual a 5.
     *
     * Na exponencial, E[X] = 1/l
     * 
     * E[X] = 5
     * 1/l = 5
     * l = 1/5
     * l = 0.2
     * 
     */

    double l;
    printf("Informe tempo médio (ms): ");
    scanf("%lF", &l);

    l = 1.0/l; //parâmetro Exponencial

    double soma = 0.0;
    int qtdExecucoes = 100000;
    int i = 0;

    /****************************
     *    Iniciando semente     *
     ****************************/

    int sementeAleat;
    sementeAleat = time(NULL);
    srand(sementeAleat); //inicia o rand

    /****************************
     *         Execução         *
     ****************************/

    for (i = 0; i < qtdExecucoes; i++){
        double valor = (-1.0 / l) * log(aleatorio());
        printf("%lF\n", valor);
        //getchar();
        soma += valor;
    }

    printf("media: %lf\n", (soma / qtdExecucoes));
    exit(0);    

}