#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct little_ {
    unsigned long int numero_eventos; //numero de elementos
    long double soma_areas; //soma das areas
    double tempo_anterior; //tempo do evento anterior
} little;

void inicia_little(little *l) {
    l->numero_eventos = 0;
    l->soma_areas = 0.0;
    l->tempo_anterior = 0.0;
}

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

    //variaveis para o calculo de E[N] e E[W]
    little en;
    little ew_entrada;
    little ew_saida;

    inicia_little(&en);
    inicia_little(&ew_entrada);
    inicia_little(&ew_saida);

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

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
            en.tempo_anterior = tempo_decorrido;
            en.numero_eventos++;

            ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_eventos;
            ew_entrada.tempo_anterior = tempo_decorrido;
            ew_entrada.numero_eventos++;
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

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
            en.tempo_anterior = tempo_decorrido;
            en.numero_eventos--;

            ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_eventos;
            ew_saida.tempo_anterior = tempo_decorrido;
            ew_saida.numero_eventos++;
            //printf("tamanho da fila: %ld\n", fila);
            //printf("------------------------\n");
        }
    }  

    en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
    ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_eventos;
    ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_eventos;


    printf("Fim da simulação. Maior tamanho de fila: %ld\n", max_fila);
    printf("Ocupação: %lF\n", (soma_ocupacao/tempo_decorrido));

    long double en_final = (en.soma_areas/tempo_decorrido);
    long double ew_final = (ew_entrada.soma_areas - ew_saida.soma_areas) / ew_entrada.numero_eventos;
    long double lambda = ew_entrada.numero_eventos / tempo_decorrido;

    printf("E[N]: %Lf\n", en_final);
    printf("E[W]: %Lf\n", ew_final);

    //E[N] = lambda * E[W]
    //E[N] - lambda * E[W] ~ 0 (erro little)
    printf("Erro Little: %.15Lf\n", en_final - lambda * ew_final);
    getchar();
    getchar();
    
    exit(0);
}