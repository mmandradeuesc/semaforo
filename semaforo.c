#include "pico/stdlib.h"
#include "hardware/timer.h"
#include <stdio.h>

// Definição dos pinos do LED RGB (BitDogLab)
const uint RED_PIN = 11;    // GPIO11 - Vermelho
const uint YELLOW_PIN = 12; // GPIO12 - Amarelo
const uint GREEN_PIN = 13;  // GPIO13 - Verde

// Variáveis de controle
volatile int current_state = 0;  // 0=vermelho, 1=amarelo, 2=verde
volatile int elapsed_time = 0;   // Tempo decorrido no estado atual

bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(RED_PIN, 0);
    gpio_put(YELLOW_PIN, 0);
    gpio_put(GREEN_PIN, 0);

    // Atualiza o estado na sequência correta
    current_state = (current_state + 1) % 3;

    // Aciona o LED correspondente ao estado atual
    switch(current_state) {
        case 0:  // Vermelho
            gpio_put(RED_PIN, 1);
            break;
        case 1:  // Amarelo
            gpio_put(YELLOW_PIN, 1);
            break;
        case 2:  // Verde
            gpio_put(GREEN_PIN, 1);
            break;
    }

    elapsed_time = 0;  // Reinicia contagem de tempo
    return true;
}

int main() {
    // Inicializa a comunicação serial
    stdio_init_all();

    // Configuração dos pinos GPIO
    gpio_init(RED_PIN);
    gpio_init(YELLOW_PIN);
    gpio_init(GREEN_PIN);
    
    gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_set_dir(YELLOW_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);

    // Estado inicial vermelho
    gpio_put(RED_PIN, 1);
    printf("Semáforo iniciado - ESTADO VERMELHO\n");

    // Configura o temporizador para 3 segundos (3000ms)
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while(true) {
        // Imprime informações a cada segundo
        printf("Estado atual: %s - Tempo decorrido: %d segundos\n", 
            current_state == 0 ? "VERMELHO" : 
            current_state == 1 ? "AMARELO" : "VERDE",
            elapsed_time);
        
        elapsed_time++;
        sleep_ms(1000);  // Aguarda 1 segundo
    }

    return 0;
}