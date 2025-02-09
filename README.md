# Embarcatech_I2C_Display
Repositório criado para a tarefa relacionada a comunicação serial com uart, i2c e display de leds ssd1306.


O programa implementa uma comunicação serial via UART com o computador e via I2C com o display SSD1306.
Além disso, também foi implementada uma rotina de interrupção para os botões A e B, que alterna os estados de dois canais do LED RGB.
Por fim, a matriz 5x5 de LEDs WS2812 é ativada caso os caracteres informados sejam dígitos de 0 a 9.

# Instruções de compilação

Para compilar o código, são necessárias as seguintes extensões: 

*Wokwi Simulator*

*Raspberry Pi Pico*

*Cmake*

Após instalá-las basta buildar o projeto pelo CMake. A partir daí, abra o arquivo 
diagram.json e clique no botão verde para iniciar a simulação.

Enquanto na simulação, o usuário pode interagir com o programa utilizando o terminal serial do Wokwi.
É possível informar dígitos de 0 a 9 e os caracteres de 'a' a 'z', tanto em minúsculas quanto em maiúsculas.
Cada vez que um caractere é digitado, ele é mostrado tanto no terminal do Wokwi quanto no display de LEDs presente no circuito.

Também é possível interagir com os botões para alternar os canais do LED RGB. O botão inferior altera o estado do canal verde, e o botão superior altera o estado do canal azul.

Caso o usuário informe algum dígito de 0 a 9, a matriz 5x5 será ativada, exibindo o dígito correspondente ao digitado.

Por fim, o caractere 'C', maiúsculo ou minúsculo, faz com que tanto a matriz quanto o display sejam apagados após um breve delay.

# Vídeo demonstrativo

https://www.youtube.com/shorts/v2tnc7p4qKM
