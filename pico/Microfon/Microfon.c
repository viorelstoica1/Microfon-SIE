#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/multicore.h"

#define MARIME_BUFFER 1000
uint8_t buffer_adc[MARIME_BUFFER];
int index_curent = 0;

void core1_main(){
    while(1){
        if(index_curent == MARIME_BUFFER){
            index_curent = 0;
            for(int i = 0; i < MARIME_BUFFER; i++){
                printf("%c", buffer_adc[i]);
            }
            printf("\n");
        }
    }
}

int main()
{
    stdio_init_all();

    //multicore_launch_core1(core1_main);
    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Example to turn on the Pico W LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    adc_init();
    adc_select_input(2);

    bool ok = 0;
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, ok);
        ok = 1 - ok;
        buffer_adc[index_curent] = adc_read()/16; //0 -> 4096 0 -> 256
        sleep_us(40);
        index_curent++;
        if(index_curent == MARIME_BUFFER){
            index_curent = 0;
            for(int i = 0; i < MARIME_BUFFER; i++){
                printf("%c", buffer_adc[i]);
            }
            printf("\n");
        }
    }
}
