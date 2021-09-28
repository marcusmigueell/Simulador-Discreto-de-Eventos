/*************************************************** 
 *       TRABALHO 1 ANÁLISE DE DESEMEPENHO         *
 *         Prof.: Flávio Barbieri Gonzaga          *
 *   Marcus Vinicius Garcia Miguel 2017.1.08.039   *
 *          Data de Entrega: 10/09/2021            * 
 *    Para todas as ocupações os valores serão:    *
 *                                                 *
 *        Tamanho do link (kbps): 100              *
 *        Tempo de simulação (s): 100000           *
 *                                                 *
 *    O intervalo médio entre pacotes serão:       *
 *                                                 *                                 
 *       Para ocupação de 80% = 0.00441            *
 *       Para ocupação de 90% = 0.00392            *
 *       Para ocupação de 99% = 0.00356            *
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
                tempo_chegada, tempo_saida, soma_ocupacao = 0.0, tamanho_link, tempo_simulacao;
                
    //Essa próxima váriável é para ser usada para achar o valor para que o tempo de simulação / invervalos_iguais = 100
    double intervalos_iguais = 0;

    long double ocupacao[105], eW[105], eN[105], erroLittle[105];
    int contador_ocupacao = 0;
    int contador_ew_en = 0;
    long double lambda = 0.00;
    
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

    //para a primeira linha gravado no arquivo será o valor médio entre pacotes
    //e cada 100 intervalos estará separado entre a primeira linha o valor médio entre pacotes
    //e a última linha do bloco de 100 intervalo terá vários traços separando os intervalos
    ocupacao[contador_ocupacao] = intervalo_pacotes;
    eW[contador_ew_en] = intervalo_pacotes;
    eN[contador_ew_en] = intervalo_pacotes;
    erroLittle[contador_ew_en] = intervalo_pacotes;

    intervalo_pacotes = 1.0 / intervalo_pacotes; //parâmetro Exponencial

    contador_ew_en++;
    contador_ocupacao++;

    printf("Informe o tamanho do link (kbps): ");
    scanf("%lF", &tamanho_link); 

    //vou converter de kbps para bps
    tamanho_link = tamanho_link * 1000.0;
    //agora vou converter de bps para Bps
    tamanho_link = tamanho_link / 0.8;
    
    printf("Informe o tempo de simulação (segundos): ");
    scanf("%lF", &tempo_simulacao);
    
    intervalos_iguais = tempo_simulacao/100;

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
    while (tempo_decorrido <= tempo_simulacao) {

        while (tempo_decorrido <= (contador_ocupacao*intervalos_iguais)){

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
        ocupacao[contador_ocupacao] = soma_ocupacao / tempo_decorrido;
        eW[contador_ew_en] = (ew_entrada.soma_areas - ew_saida.soma_areas) / ew_entrada.numero_pacotes;
        eN[contador_ew_en] = (en.soma_areas/tempo_decorrido);
        lambda = ew_entrada.numero_pacotes / tempo_decorrido;
        erroLittle[contador_ew_en] = eN[contador_ew_en] - lambda * eW[contador_ew_en];
        contador_ocupacao++;
        contador_ew_en++;
    }  

    en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_pacotes;
    ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_pacotes;
    ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_pacotes;

    long double en_final = (en.soma_areas/tempo_decorrido);
    long double ew_final = (ew_entrada.soma_areas - ew_saida.soma_areas) / ew_entrada.numero_pacotes;
    lambda = ew_entrada.numero_pacotes / tempo_decorrido;

    ocupacao[contador_ocupacao] = soma_ocupacao / tempo_decorrido;
    eW[contador_ew_en] = ew_final;
    eN[contador_ew_en] = en_final;
    erroLittle[contador_ew_en] = en_final - lambda * ew_final;
    

    FILE *arquivo_ocupacao;
        arquivo_ocupacao = fopen("ocupacao.txt", "a");
        for(int i = 0; i < contador_ocupacao; i++){
            fprintf(arquivo_ocupacao, "%Lf\n", ocupacao[i]);    
        }
        //traços indicando o fim da ocupação que está indicado na primeira linha de cada bloco de intervalos
        fprintf(arquivo_ocupacao, "%s\n", "------------------------------------------------------------");  
    
    fclose(arquivo_ocupacao);

    FILE *arquivo_ew;
    FILE *arquivo_en;
    FILE *arquivo_little;

        arquivo_ew = fopen("ew.txt", "a");
        arquivo_en = fopen("en.txt", "a");
        arquivo_little = fopen("erroLittle.txt", "a");

        for(int i = 0; i <= contador_ew_en; i++){
            fprintf(arquivo_ew, "%Lf\n", eW[i]);
            fprintf(arquivo_en, "%Lf\n", eN[i]);
            fprintf(arquivo_little, "%.15Lf\n", erroLittle[i]);
        }
        //traços indicando o fim da ocupação que está indicado na primeira linha de cada bloco de intervalos
        fprintf(arquivo_ew, "%s\n", "------------------------------------------------------------");
        fprintf(arquivo_en, "%s\n", "------------------------------------------------------------");
        fprintf(arquivo_little, "%s\n", "------------------------------------------------------------");

    fclose(arquivo_ew);
    fclose(arquivo_en);
    fclose(arquivo_little);

    printf("Fim da simulação. Maior tamanho de fila: %ld\n", max_fila);
    printf("Ocupação: %lF\n", (soma_ocupacao/tempo_decorrido));

    printf("E[N]: %Lf\n", en_final);
    printf("E[W]: %Lf\n", ew_final);

    //E[N] = lambda * E[W]
    //E[N] - lambda * E[W] ~ 0 (erro little)
    printf("Erro Little: %.15Lf\n", en_final - lambda * ew_final);

    double soma_todos_pacotes = qtd_50_porc + qtd_40_porc + qtd_10_porc;

    printf("Porcentagem da quantidade de pacotes com tamanho 550 Bytes: %.2f\n", (qtd_50_porc*100/soma_todos_pacotes));
    printf("Porcentagem da quantidade de pacotes com tamanho 40 Bytes: %.2f\n", (qtd_40_porc*100/soma_todos_pacotes));
    printf("Porcentagem da quantidade de pacotes com tamanho 1500 Bytes: %.2f\n", (qtd_10_porc*100/soma_todos_pacotes));
    
    exit(0);
}
