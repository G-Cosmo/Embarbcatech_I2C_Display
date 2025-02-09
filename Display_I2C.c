#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "inc/ledMatrix.h"
#include "inc/font.h"
#include "inc/ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

//UART defines
#define UART_ID uart0       //define a uart a ser utilizada, nesse caso, uart0
#define BAUD_RATE 115200    //define o baud rate como 115200
#define UART_TX_PIN 0       //pino 0 para tx da uart0
#define UART_RX_PIN 1       //pino 1 para rx da uart0

#define buttonA 5           //pino do botão A
#define buttonB 6           //pino do botão B

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
    //printf("\n interrupção1");
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {

        last_time = current_time; 

        if(gpio == buttonA)
        {
            uart_puts(UART_ID, "\n\r interrupção em A. Estado do LED verde alternado.");  
            gpio_put(rgb_led[1], !gpio_get(rgb_led[1]));
            //falta enviar a mensagem para o display

        }else if (gpio == buttonB)    
        {
            uart_puts(UART_ID, "\n\r interrupção em B. Estado do LED azul alternado.");
            gpio_put(rgb_led[2], !gpio_get(rgb_led[2]));
            //falta enviar a mensagem para o display

        }   

    }
}



int main()
{
    char c;

    stdio_init_all();       //iniciliaza a lib stdio
    init_rgb(rgb_led);      //inicializa o led rgb
    init_buttons();         //inicializa os botões

    npInit(LED_PIN);        //inicializa matriz de led
    npClear();              //limpa a matriz

    gpio_set_irq_enabled_with_callback(buttonA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(buttonB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
 

    // I2C Initialisation. Using it at 400Khz.
    // i2c_init(I2C_PORT, 400*1000);
    
    // gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    // gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);
    // // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // inicializa a uart
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // // Use some the various UART functions to send out data
    // // In a default system, printf will also output via the default UART
    
    // // Send out a string, with CR/LF conversions
    // uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart
    uart_puts(UART_ID, "\n\rInsira o caracter que deseja imprimir: ");

    while (true) {
        if(uart_is_readable(UART_ID))
        {   
            c = uart_getc(UART_ID);
            uart_puts(UART_ID, "\n\rCaractere Inserido: ");
            uart_putc(UART_ID, c);
            uart_puts(UART_ID, "\n\rInsira o caracter que deseja imprimir: ");


            switch (c)
            {
            case '0':
                print_frame(frame0, 100,0,100);
                break;
            case '1':
                print_frame(frame1, 100,0,100);
                break;
            case '2':
                print_frame(frame2, 100,0,100);
                break;
            case '3':
                print_frame(frame3, 100,0,100);
                break;
            case '4':
                print_frame(frame4, 100,0,100);
                break;
            case '5':
                print_frame(frame5, 100,0,100);
                break;
            case '6':
                print_frame(frame6, 100,0,100);
                break;
            case '7':
                print_frame(frame7, 100,0,100);
                break;
            case '8':
                print_frame(frame8, 100,0,100);
                break;
            case '9':
                print_frame(frame9, 100,0,100);
                break;
            default:
                npClear();
                break;
            }      

        }
    }
}
