#include <stdio.h>
#include "pico/stdlib.h"

#define LED_VERDE 10
#define LED_VERMELHO 11
#define LED_AZUL 12
#define BUZZER 21

//mapa de teclas
char KEY_MAP[16] = {
    '1', '2' , '3', 'A',
    '4', '5' , '6', 'B',
    '7', '8' , '9', 'C',
    '*', '0' , '#', 'D'
};

//define os pinos do teclado com as portas GPIO
uint columns[4] = {4, 3, 2, 1}; 
uint rows[4] = {8, 7, 6, 5};


// Função para inicializar o teclado
void init_teclado() {
    // Configura as colunas como saída
    for (int i = 0; i < 4; i++) {
        gpio_init(columns[i]);  //inicializa as colunas
        gpio_set_dir(columns[i], GPIO_OUT); // coloca as colunas como saída 
        gpio_put(columns[i], 1); // Inicializa como HIGH = alto (1)
    }

    // Configura as linhas como entrada com pull-up
    for (int i = 0; i < 4; i++) {
        gpio_init(rows[i]); // inicializa as linhas
        gpio_set_dir(rows[i], GPIO_IN); // coloca as linhas como entrada 
        gpio_pull_up(rows[i]);
    }
}

// Função para verificar teclas pressionadas
char detectar_tecla() {
    for (int col = 0; col < 4; col++) {
        // Ativa a coluna atual
        gpio_put(columns[col], 0);

        // Verifica as linhas
        for (int row = 0; row < 4; row++) {
            if (gpio_get(rows[row]) == 0) { // Tecla pressionada
                // Desativa a coluna antes de retornar
                gpio_put(columns[col], 1);
                return KEY_MAP[row * 4 + col];
            }
        }

        // Desativa a coluna atual
        gpio_put(columns[col], 1);
    }

    // Nenhuma tecla pressionada
    return '\0';
}

// Função para gerar som com uma frequência específica
void tocar_som(uint buzzer_pin, uint frequencia, uint duracao_ms) {
    uint periodo_us = 1000000  / frequencia; // Período em microssegundos

    uint metade_periodo = periodo_us / 2;   // Meio período para alternar o estado
    uint ciclos = (frequencia * duracao_ms) / 1000; // Quantos ciclos gerar no tempo

    for (uint i = 0; i < ciclos; i++) {
        gpio_put(buzzer_pin, 1); // Liga o buzzer
        sleep_us(metade_periodo);

        gpio_put(buzzer_pin, 0); // Desliga o buzzer
        sleep_us(metade_periodo);
    }
}

int main() {
    stdio_init_all();

    // Inicializa o teclado
    init_teclado();

    printf("Teclado inicializado.\n");

    char tecla;

    // Inicializa os Leds e o Buzzer 
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    while (true) {
        //mantém os Leds desligados
        gpio_put(LED_VERDE, 0); 
        gpio_put(LED_VERMELHO, 0);
        gpio_put(LED_AZUL, 0);

        // Detecta a tecla pressionada
        tecla = detectar_tecla();
        if (tecla != '\0') {
          printf("Tecla pressionada: %c\n", tecla);
          if(tecla == 'A'){
            gpio_put(LED_VERDE, 1);
            printf("Led verde ligado\n");

          }else if(tecla == 'B'){
            gpio_put(LED_VERMELHO, 1);
            printf("Led vermelho ligado\n");

          } else if(tecla == 'C'){
            gpio_put(LED_AZUL, 1);
            printf("Led azul ligado\n");

          }else if(tecla == 'D'){
            gpio_put(LED_VERDE, 1);
            gpio_put(LED_VERMELHO, 1);
            gpio_put(LED_AZUL, 1);
            printf("Todos leds ligados\n");

          } else if(tecla == '#'){
              tocar_som(BUZZER, 261, 500); 
              sleep_ms(1000); 
          }
        }

        sleep_ms(100);
    }
}