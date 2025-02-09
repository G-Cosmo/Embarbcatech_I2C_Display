#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "inc/ledMatrix.h"
#include "inc/font.h"
#include "inc/ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define address 0x3C


#define buttonA 5           //pino do botão A
#define buttonB 6           //pino do botão B

bool color = true;  //variavel que indica que se o pixel está ligado ou desligado
ssd1306_t ssd; // Inicializa a estrutura do display

uint last_time = 0;

const uint rgb_led[3] = {13,11,12}; //pinos do led rgb
uint r_intensity = 30;
uint g_intensity = 0;
uint b_intensity = 30;

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

        ssd1306_fill(&ssd, !color); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, color, !color); // Desenha um retângulo

        if(gpio == buttonA)
        {
            printf("\n\n\rInterrupção em A.");  
            gpio_put(rgb_led[1], !gpio_get(rgb_led[1]));

            ssd1306_draw_string(&ssd, "LED Verde", 30, 20); // Desenha uma string    
            if(gpio_get(rgb_led[1]))
            {
                ssd1306_draw_string(&ssd, "Ligado", 42, 30);
                printf( "\n\rLED verde ligado.\n");
                printf("\n\rInsira o caracter que deseja imprimir: ");
            }else
            {
                ssd1306_draw_string(&ssd, "Desligado", 30, 30);
                printf( "\n\rLED verde desligado.\n");
                printf("\n\rInsira o caracter que deseja imprimir: ");
            }

        }else if (gpio == buttonB)    
        {
            printf("\n\n\rInterrupção em B.");
            gpio_put(rgb_led[2], !gpio_get(rgb_led[2]));
            ssd1306_draw_string(&ssd, "LED Azul", 30, 20); // Desenha uma string    
            if(gpio_get(rgb_led[2]))
            {
                ssd1306_draw_string(&ssd, "Ligado", 42, 30);
                printf( "\n\rLED azul ligado.\n");
                printf("\n\rInsira o caracter que deseja imprimir: ");
            }else
            {
                ssd1306_draw_string(&ssd, "Desligado", 30, 30);
                printf( "\n\rLED azul desligado.\n");
                printf("\n\rInsira o caracter que deseja imprimir: ");

            }   
        }  

        ssd1306_send_data(&ssd); // Atualiza o display
    }
}


void checkDigit(char c) {

    switch (c)
    {
    case '0':
        print_frame(frame0, r_intensity, g_intensity, b_intensity);
        break;
    case '1':
        print_frame(frame1, r_intensity, g_intensity, b_intensity);
        break;
    case '2':
        print_frame(frame2, r_intensity, g_intensity, b_intensity);
        break;
    case '3':
        print_frame(frame3, r_intensity, g_intensity, b_intensity);
        break;
    case '4':
        print_frame(frame4, r_intensity, g_intensity, b_intensity);
        break;
    case '5':
        print_frame(frame5, r_intensity, g_intensity, b_intensity);
        break;
    case '6':
        print_frame(frame6, r_intensity, g_intensity, b_intensity);
        break;
    case '7':
        print_frame(frame7, r_intensity, g_intensity, b_intensity);
        break;
    case '8':
        print_frame(frame8, r_intensity, g_intensity, b_intensity);
        break;
    case '9':
        print_frame(frame9, r_intensity, g_intensity, b_intensity);
        break;
    default:
        npClear();
        break;
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

    gpio_set_irq_enabled_with_callback(buttonA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   //ativa a interrupção no pino do botão A
    gpio_set_irq_enabled_with_callback(buttonB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   //ativa a interrupção no pino do botão B
 

   // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);


    ssd1306_init(&ssd, WIDTH, HEIGHT, false, address, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
  
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    printf("\n\rInsira o caracter que deseja imprimir: ");

    while (true) { 
            scanf("%c", &c);
            printf("\n\rCaractere Inserido: %c", c);
            printf("\n\n\rInsira o caracter que deseja imprimir: ");
            
            if(c == 'c' || c == 'C')
            {
                ssd1306_fill(&ssd, !color); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, color, !color); // Desenha um retângulo
                ssd1306_draw_char(&ssd, c, 59, 27); // Desenha o caractere digitado
                ssd1306_send_data(&ssd);
                
                sleep_ms(500);

                ssd1306_fill(&ssd, !color); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, color, !color); // Desenha um retângulo
                ssd1306_draw_string(&ssd, "Desligando a", 15, 27);
                ssd1306_draw_string(&ssd, "Tela", 45, 40);
                ssd1306_send_data(&ssd);

                sleep_ms(1000);

                ssd1306_fill(&ssd, !color); // Limpa o display
                ssd1306_send_data(&ssd);
                printf("\n\n\rTela Desligada! ");
                printf("\n\n\rInsira o caracter que deseja imprimir: ");

                checkDigit(c);  // Checa se o caractere digitado foi um digito de 0 a 9 e o desenha na matriz 5x5 ws2812

                continue;
            }

            ssd1306_fill(&ssd, !color); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, color, !color); // Desenha um retângulo
            ssd1306_draw_char(&ssd, c, 59, 27); // Desenha o caractere digitado
            ssd1306_send_data(&ssd);    // Envia os dados para a matriz 
            
            checkDigit(c);  // Checa se o caractere digitado foi um digito de 0 a 9 e o desenha na matriz 5x5 ws2812

        }
}
