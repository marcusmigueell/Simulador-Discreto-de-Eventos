/*************************************************** 
 *       TRABALHO 1 ANÁLISE DE DESEMEPENHO         *
 *         Prof.: Flávio Barbieri Gonzaga          *
 *   Marcus Vinicius Garcia Miguel 2017.1.08.039   *
 *     Gustavo Celestino de Lima 2018.1.08.049     *
 *          Data de Entrega: 10/09/2021            *     
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct little_ {
    unsigned long int numero_pacotes; //numero de elementos
    long double soma_areas; //soma das areas
    double tempo_anterior; //tempo do evento anterior
} little;

void inicia_little(little *l) {
    l->numero_pacotes = 0;
    l->soma_areas = 0.0;
    l->tempo_anterior = 0.0;
}

double aleatorio() {
    double u = rand() / ((double) RAND_MAX + 1);
    //limitando entre (0,1]
    u = 1.0 - u;

    return (u);
}

double tamanho_pacote(){
    double u = aleatorio();
    if(u <= 0.5) //50%
        return 550;
    else if (u <= 0.9) //40%
        return 40;
    else //10%
        return 1500;
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

    double intervalo_pacotes, tempo_decorrido = 0.0,
                tempo_chegada, tempo_saida, soma_ocupacao = 0.0, tamanho_link;

    long int fila = 0;

    //variaveis para o calculo de E[N] e E[W]
    little en;
    little ew_entrada;
    little ew_saida;

    inicia_little(&en);
    inicia_little(&ew_entrada);
    inicia_little(&ew_saida);

    printf("Informe o intervalo médio entre pacotes (segundos): ");
    scanf("%lF", &intervalo_pacotes);
    intervalo_pacotes = 1.0 / intervalo_pacotes; //parâmetro Exponencial

    printf("Informe o tamanho do link (kbps): ");
    scanf("%lF", &tamanho_link); 

    //vou converter de kbps para bps
    tamanho_link = tamanho_link * 1000.0;
    //agora vou converter de bps para Bps
    tamanho_link = tamanho_link / 0.8;

    long int qtd_50_porc = 0;
    long int qtd_40_porc = 0;
    long int qtd_10_porc = 0;
    double recebe_pacote = 0.0;
    
    /****************************
     *    Iniciando semente     *
    ****************************/

    int sementeAleat;
    sementeAleat = time(NULL);
    srand(sementeAleat); //inicia o rand

    /****************************
     *   Tempo chegada 1° req   *
    ****************************/

    tempo_chegada = (-1.0 / intervalo_pacotes) * log(aleatorio());

    /****************************
     *         Execução         *
    ****************************/

    long int max_fila = 0;
    while (tempo_decorrido <= tamanho_link) {
        if (fila == 0) {
            tempo_decorrido = tempo_chegada;
        } else {
            tempo_decorrido = minimo(tempo_chegada, tempo_saida);
        }

        if (tempo_decorrido == tempo_chegada) {

            if (fila == 0) {
                recebe_pacote = tamanho_pacote();
                if(recebe_pacote == 550) {
                    qtd_50_porc++;
                } else if(recebe_pacote == 40) {
                    qtd_40_porc++;
                } else {
                    qtd_10_porc++;
                }
                tempo_saida = tempo_decorrido + recebe_pacote / tamanho_link;
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }

            fila++;

            if (fila > max_fila)
                max_fila = fila;

            tempo_chegada = tempo_decorrido + (-1.0 / intervalo_pacotes) * log(aleatorio());

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_pacotes;
            en.tempo_anterior = tempo_decorrido;
            en.numero_pacotes++;

            ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_pacotes;
            ew_entrada.tempo_anterior = tempo_decorrido;
            ew_entrada.numero_pacotes++;

        } else {
            
            fila--;
            if(fila > 0) {
                recebe_pacote = tamanho_pacote();
                if(recebe_pacote == 550) {
                    qtd_50_porc++;
                } else if(recebe_pacote == 40) {
                    qtd_40_porc++;
                } else {
                    qtd_10_porc++;
                }
                tempo_saida = tempo_decorrido + recebe_pacote / tamanho_link;
                soma_ocupacao += tempo_saida - tempo_decorrido;
            }

            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_pacotes;
            en.tempo_anterior = tempo_decorrido;
            en.numero_pacotes--;

            ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_pacotes;
            ew_saida.tempo_anterior = tempo_decorrido;
            ew_saida.numero_pacotes++;
        }
    }  

    en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_pacotes;
    ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_pacotes;
    ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_pacotes;


    printf("Fim da simulação. Maior tamanho de fila: %ld\n", max_fila);
    printf("Ocupação: %lF\n", (soma_ocupacao/tempo_decorrido));

    long double en_final = (en.soma_areas/tempo_decorrido);
    long double ew_final = (ew_entrada.soma_areas - ew_saida.soma_areas) / ew_entrada.numero_pacotes;
    long double lambda = ew_entrada.numero_pacotes / tempo_decorrido;

    printf("E[N]: %Lf\n", en_final);
    printf("E[W]: %Lf\n", ew_final);

    //E[N] = lambda * E[W]
    //E[N] - lambda * E[W] ~ 0 (erro little)
    printf("Erro Little: %.15Lf\n", en_final - lambda * ew_final);

    double soma_todos_pacotes = qtd_50_porc + qtd_40_porc + qtd_10_porc;

    printf("Quantidade de pacotes com tamanho 550 Bytes: %ld\n", qtd_50_porc);
    printf("Quantidade de pacotes com tamanho 40 Bytes: %ld\n", qtd_40_porc);
    printf("Quantidade de pacotes com tamanho 1500 Bytes: %ld\n", qtd_10_porc);

    printf("Porcentagem da quantidade de pacotes com tamanho 550 Bytes: %.2f\n", (qtd_50_porc*100/soma_todos_pacotes));
    printf("Porcentagem da quantidade de pacotes com tamanho 40 Bytes: %.2f\n", (qtd_40_porc*100/soma_todos_pacotes));
    printf("Porcentagem da quantidade de pacotes com tamanho 1500 Bytes: %.2f\n", (qtd_10_porc*100/soma_todos_pacotes));
    
    exit(0);
}