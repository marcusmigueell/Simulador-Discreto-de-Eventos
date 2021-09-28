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

double minimo(double n1, double n2) {
    if (n1 < n2)
        return n1;
    return n2;
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

    double intervalo_requisicao, tempo_atendimento, tempo_simulacao, tempo_decorrido = 0.0,
                tempo_chegada, tempo_saida, soma_ocupacao = 0.0;

    long int fila = 0;

    printf("Informe o intervalo médio entre requisições (segundos): ");
    scanf("%lF", &intervalo_requisicao);
    intervalo_requisicao = 1.0 / intervalo_requisicao; //parâmetro Exponencial

    printf("Informe o tempo médio de atendimento (segundos): ");
    scanf("%lF", &tempo_atendimento);
    tempo_atendimento = 1.0 / tempo_atendimento; //parâmetro Exponencial

    printf("Informe o tempo de simulação (segundos): ");
    scanf("%lF", &tempo_simulacao);
    
    /****************************
     *    Iniciando semente     *
    ****************************/

    int sementeAleat;
    sementeAleat = time(NULL);
    srand(sementeAleat); //inicia o rand

    /****************************
     *   Tempo chegada 1° req   *
    ****************************/

    tempo_chegada = (-1.0 / intervalo_requisicao) * log(aleatorio());

    /****************************
     *         Execução         *
    ****************************/

    long int max_fila = 0;
    while (tempo_decorrido <= tempo_simulacao) {
        if (fila == 0) {
            tempo_decorrido = tempo_chegada;
        } else {
            tempo_decorrido = minimo(tempo_chegada, tempo_saida);
        }

        //printf("tempo_decorrido: %lF\n", tempo_decorrido);
        if (tempo_decorrido == tempo_chegada) {
            //tratando a chegada de requisição
            //printf("Evento de chegada!\n");

            if (fila == 0) {
                tempo_saida = tempo_decorrido + (-1.0 / tempo_atendimento) * log(aleatorio());
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }

            fila++;

            if (fila > max_fila)
                max_fila = fila;

            tempo_chegada = tempo_decorrido + (-1.0 / intervalo_requisicao) * log(aleatorio());
            //printf("tamanho da fila: %ld\n", fila);
            //printf("------------------------\n");
        } else {
            //tratando a saida de requisição
            //printf("Evento de saída!\n");
            fila--;
            if(fila > 0) {
                tempo_saida = tempo_decorrido + (-1.0 / tempo_atendimento) * log(aleatorio());
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }
            //printf("tamanho da fila: %ld\n", fila);
            //printf("------------------------\n");
        }
    }  
    printf("Fim da simulação. Maior tamanho de fila: %ld\n", max_fila);
    printf("Ocupação: %lF\n", (soma_ocupacao/tempo_decorrido));
    getchar();
    getchar();
    
    exit(0);
}