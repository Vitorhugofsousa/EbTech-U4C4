//include das bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "EBTECH_U4C5.pio.h"

//DEFINIÇÃO DOS PINOS
#define LED_PIN_RED 13 //led vermelho
#define NUM_PIXELS 25 //número de leds na matriz
#define LED_PIN 7 //pino de saída da matriz de led
#define BOTAO_A 5 //pino saida botao a
#define BOTAO_B 6 //pino saida botao b

//VARIÁVEIS GLOBAIS
int static volatile indice = 0; //variável para countrole do índice da matriz de led
uint count = 0; //variável para countrole do countador
uint actual_time = 0; //variável para countrole do tempo
uint valor_led; //variável para countrole do valor do led
uint sm;  //variável para countrole do state machine
PIO pio = pio0;  //variável para countrole da pio

//FUNÇÃO PISCAR LED, FAZ O LED VERMELHO DO PINO 13 PISCAR 5 VEZES POR SEGUNDO
void piscar_led(){ // Função para piscar o LED vermelho
  gpio_put(LED_PIN_RED, true);  // Acende o LED vermelho
  sleep_ms(25); // Aguarda 25 ms
  gpio_put(LED_PIN_RED, false); // Apaga o LED vermelho
  sleep_ms(175);  // Aguarda 175 ms
    }

// MATRIZ DE LEDS
// ROTINA PARA DEFINIÇÃO DA INTENSIDADE DE CORES DO LED
uint matrix_rgb(float r, float g, float b){
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// FUNÇÃO PARA CONVERTER A POSIÇÃO DO MATRIZ PARA UMA POSIÇÃO DO VETOR
int getIndex(int x, int y){
  // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita
  // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda
  if (y % 2 == 0){
    return 24 - (y * 5 + x); // Linha par (esquerda para direita)
  }else{
    return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda)
  }
}

// FUNÇÃO PARA CONTROLAR O ÍNDICE DA MATRIZ
void new_index(){   // Função para controlar o índice da matriz
  if (indice > 10){ // Se o índice for maior que 10, volta para 0
    indice = 0;
  }else if(indice < 0){ // Se o índice for menor que 0, volta para 10
    indice = 10;
  }
}

// FUNCAO PARA DESENHAR A MATRIZ
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
  for (int16_t i = 0; i < NUM_PIXELS; i++)  // Percorre todos os LEDs da matriz
  {
    valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b); // Define a intensidade de cada cor
    pio_sm_put_blocking(pio, sm, valor_led);  // Atualiza o valor do LED
  };
}



//NUMEROS PARA EXIBIR NA MATRIZ DE LED
        double apagar_leds[25] ={   //Apagar LEDs da matriz
              0.0, 0.0, 0.0, 0.0, 0.0,          
              0.0, 0.0, 0.0, 0.0, 0.0, 
              0.0, 0.0, 0.0, 0.0, 0.0,
              0.0, 0.0, 0.0, 0.0, 0.0,
              0.0, 0.0, 0.0, 0.0, 0.0};

        double numero0[25] = {    //Número 0
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 1.0, 1.0,
            1.0, 0.0, 1.0, 0.0, 1.0,
            1.0, 1.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};
        
        double numero1[25] = {    //Número 1
            0.0, 1.0, 1.0, 1.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 1.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0};
          
        double numero2[25] = {    //Número 2
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};
        
        double numero3[25] = {    //Número 3
            1.0, 1.0, 1.0, 1.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double numero4[25] = {    //Número 4
            1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0};
          
        double numero5[25] = {    //Número 5
            1.0, 1.0, 1.0, 1.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double numero6[25] = {    //Número 6
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double numero7[25] = {    //Número 7
            1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double numero8[25] = {    //Número 8
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double numero9[25] = {    //Número 9
            1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 0.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0};

        double *numeros[11] = {apagar_leds, numero0, numero1, numero2, numero3, numero4, numero5, numero6, numero7, numero8, numero9};  // Vetor com os números

// FUNÇÃO DE CALLBACK PARA OS BOTÕES
void callback_button(uint gpio, uint32_t events) {
    uint time = to_ms_since_boot(get_absolute_time());
    if (time - actual_time > 250) { // Condição para evitar múltiplos pressionamentos (debounce)
        actual_time = time;       // Atualiza o tempo
        if (gpio == BOTAO_A) {  // Verifica se o botão A foi pressionado
            indice++;          // Incrementa o índice
            new_index();     // Verifica se o índice está dentro do intervalo
            desenho_pio(numeros[indice], valor_led, pio, sm, 0.5, 0.0, 0.5);  // Desenha o número na matriz
            count++;       // Incrementa o contador
        } else if (gpio == BOTAO_B) { // Verifica se o botão B foi pressionado
            indice--;               // Decrementa o índice
            new_index();       // Verifica se o índice está dentro do intervalo
            desenho_pio(numeros[indice], valor_led, pio, sm, 0.5, 0.0, 0.5);  // Desenha o número na matriz
            count++;    // Incrementa o contador
        }
    }
}
        
// FUNÇÃO PRINCIPAL
int main() {
  bool frequenciaClock; // Variável para verificar se a frequência do clock foi configurada corretamente
  uint16_t i; // Variável para controlar o loop
  float r, g, b;   // Variáveis para controlar a intensidade de cada cor
  pio = pio0; // Seleciona a PIO 0
  uint32_t valor_led = 0; // Inicializa com preto (todos os LEDs apagados)

  // Inicializa o Pico
    frequenciaClock = set_sys_clock_khz(128000, false); // Configura a frequência do clock para 128 MHz
    stdio_init_all(); 
    gpio_init(LED_PIN);
    gpio_init(LED_PIN_RED);
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_pull_up(BOTAO_B);

  // configurações da PIO
    printf("iniciando a transmissão PIO");
    if (frequenciaClock){
      printf("clock set to %ld\n", clock_get_hz(clk_sys));
    }else if(!frequenciaClock){
      printf("erro ao configurar a frequencia do clock");
    }
      uint offset = pio_add_program(pio, &pio_matrix_program);
      sm = pio_claim_unused_sm(pio, true);
      pio_matrix_program_init(pio, sm, offset, LED_PIN);
    
    // Inicializa a matriz de LED
      desenho_pio(apagar_leds, valor_led, pio, sm, r, g, b); // Apaga os LEDs ao iniciar o programa

      // Configuração dos botões
      gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &callback_button);  
      gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &callback_button);  

    // Loop infinito
    while (1) {
      piscar_led(); // Piscar o LED vermelho
    }
    return 0;
}
