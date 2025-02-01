//include das bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "EBTECH_U4C5.pio.h"

//definição dos pinos
#define LED_PIN_RED 13 //led vermelho
#define NUM_PIXELS 25 //número de leds na matriz
#define LED_PIN 7 //pino de saída da matriz de led
#define BOTAO_A 5 //pino saida botao a
#define BOTAO_B 6 //pino saida botao b
#define BUZZER_A 10 //pino de saida do buzzer a

//variáveis globais
int static volatile indice = 0; //variável para countrole do índice da matriz de led
uint count = 0; //variável para countrole do countador
uint actual_time = 0; //variável para countrole do tempo
uint valor_led; //variável para countrole do valor do led
uint sm;  //variável para countrole do state machine
PIO pio = pio0;  //variável para countrole da pio

//função piscar led, faz o led vermelho do pino 13 piscar 5 vezes por segundo
    void piscar_led(){
      gpio_put(LED_PIN_RED, true);
      sleep_ms(25);
      gpio_put(LED_PIN_RED, false);
      sleep_ms(175);
    }

// MATRIZ DE LEDS
// rotina para definição da intensidade de cores do led
uint matrix_rgb(float r, float g, float b){
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y){
  // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
  // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
  if (y % 2 == 0){
    return 24 - (y * 5 + x); // Linha par (esquerda para direita).
  }else{
    return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
  }
}

// Função para controlar o índice da matriz
void new_index(){
  if (indice > 9){
    indice = 0;
  }else if(indice < 0){
    indice = 9;
  }
}

// Funcao para desenhar a matriz
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
  for (int16_t i = 0; i < NUM_PIXELS; i++)
  {
    valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
    pio_sm_put_blocking(pio, sm, valor_led);
  };
}

// Função para acionar o buzzer
void acionar_buzzer(int interval)
{
  gpio_set_function(BUZZER_A, GPIO_FUNC_PWM); // Configura pino como saída PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_A); // Obter o slice do PWM
    pwm_set_clkdiv(slice_num, 125.0);                  
    pwm_set_wrap(slice_num, 500);                      
    pwm_set_gpio_level(BUZZER_A, 150);              
    pwm_set_enabled(slice_num, true); // Ativar o PWM
    sleep_ms(interval);// Manter o som pelo intervalo
    pwm_set_enabled(slice_num, false);// Desativar o PWM  
}

//numeros para exibir na matriz de led
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

        double *numeros[10] = {numero0, numero1, numero2, numero3, numero4, numero5, numero6, numero7, numero8, numero9};


void callback_button(uint gpio, uint32_t events) {
    uint time = to_ms_since_boot(get_absolute_time());
    if (time - actual_time > 250) { // Condição para evitar múltiplos pressionamentos (debounce)
        actual_time = time;
        if (gpio == BOTAO_A) {
            indice++;
            new_index();
            desenho_pio(numeros[indice], valor_led, pio, sm, 0.5, 0.0, 0.5);
            count++;
        } else if (gpio == BOTAO_B) {
            indice--;
            new_index();
            desenho_pio(numeros[indice], valor_led, pio, sm, 0.5, 0.0, 0.5);
            count++;
        }
    }
}
        
int main() {
  bool frequenciaClock; // Variável para verificar se a frequência do clock foi configurada corretamente
  uint16_t i; // Variável para controlar o loop
  float r, g, b;   // Variáveis para controlar a intensidade de cada cor
  pio = pio0; // Seleciona a PIO 0
  uint32_t valor_led = 0; // Inicializa com preto (todos os LEDs apagados)

  // Inicializa o Pico
  frequenciaClock = set_sys_clock_khz(128000, false);
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
    
      desenho_pio(apagar_leds, valor_led, pio, sm, r, g, b); // Apaga os LEDs ao iniciar o programa
      gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &callback_button);  
      gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &callback_button);  
    // Loop infinito
    while (1) {
      piscar_led(); // Piscar o LED vermelho
    }
    return 0;
}
