#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"


#define LED_PIN_RED 13 //led verde
#define NUM_PIXELS 25 //número de leds na matriz
#define LED_PIN 7 //pino de saída da matriz de led
#define BOTAO_A 5 //pino saida botao a
#define BOTAO_B 6 //pino saida botao b
#define BUZZER_A 10 //pino de saida do buzzer a

//função piscar led, faz o led vermelho do pino 13 piscar 5 vezes por segundo
    void piscar_led() {
      gpio_put(LED_PIN_RED, true);
      sleep_ms(25);
      gpio_put(LED_PIN_RED, false);
      sleep_ms(175);
    }

int main() {
 stdio_init_all(); // Inicialização da comunicação serial
    gpio_init(LED_PIN_RED);
    gpio_init(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_put(LED_PIN_RED, 1); // Inicia o programa com o LED vermelho aceso
    
    bool botao_pressionado = true;
    // Loop infinito
    while (1) {
      piscar_led();
    }
}