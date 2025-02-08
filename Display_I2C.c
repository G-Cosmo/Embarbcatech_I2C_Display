#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "ledMatrix.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9


// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define buttonA 5
#define buttonB 6

uint last_time = 0;

const uint rgb_led[3] = {13,11,12};

void init_rgb(const uint *rgb)
{
    for(int i =0; i<3; i++)
    {
        gpio_init(rgb[i]);
        gpio_set_dir(rgb[i], GPIO_OUT);
        gpio_put(rgb[i], false); //garante que os leds comecem apagados
    }
}

void init_buttons()
{
    gpio_init(buttonA);
    gpio_set_dir(buttonA, GPIO_IN);
    gpio_pull_up(buttonA);

    gpio_init(buttonB);
    gpio_set_dir(buttonB, GPIO_IN);
    gpio_pull_up(buttonB);
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("\n interrupção1");
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {

        last_time = current_time; 

        if(gpio == buttonA)
        {
            printf("\n interrupção em A. Estado do LED verde alternado.");  
            gpio_put(rgb_led[1], !gpio_get(rgb_led[1]));
            print_frame(frame1, 100, 100, 0);
            //falta enviar a mensagem para o display

        }else if (gpio == buttonB)    
        {
            printf("\n interrupção em B. Estado do LED azul alternado.");
            gpio_put(rgb_led[2], !gpio_get(rgb_led[2]));
            print_frame(frame2, 100, 100, 0);
            //falta enviar a mensagem para o display

        }   

    }
}



int main()
{
    stdio_init_all();
    init_rgb(rgb_led);
    init_buttons();

    npInit(LED_PIN);
    npClear();
    npWrite();

    gpio_set_irq_enabled_with_callback(buttonA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(buttonB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
 

    // I2C Initialisation. Using it at 400Khz.
    // i2c_init(I2C_PORT, 400*1000);
    
    // gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    // gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);
    // // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // // Set up our UART
    // uart_init(UART_ID, BAUD_RATE);
    // // Set the TX and RX pins by using the function select on the GPIO
    // // Set datasheet for more information on function select
    // gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // // Use some the various UART functions to send out data
    // // In a default system, printf will also output via the default UART
    
    // // Send out a string, with CR/LF conversions
    // uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart


    while (true) {
        printf("Main loop\n");
        sleep_ms(1000);
    }
}
