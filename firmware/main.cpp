#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <pico/stdlib.h>

#include "ook.pio.h"


int main() {
    PIO pio = pio0;
    uint const ook_gpio = 0;
    uint const ook_microseconds_per_bit = 625;
    uint const debug_gpio = 1;

    stdio_init_all();
    sleep_ms(3*1000);
    printf("booted\n");

    gpio_init(debug_gpio);
    gpio_set_function(ook_gpio, GPIO_FUNC_SIO);
    gpio_set_dir(debug_gpio, true);  // output
    gpio_put(debug_gpio, 0);

    gpio_init(ook_gpio);
    gpio_set_function(ook_gpio, GPIO_FUNC_PIO0);
    gpio_set_dir(ook_gpio, true);  // output

    uint ook_offset = pio_add_program(pio, &ook_program);
    printf("loaded at %u\n", ook_offset);

    ook_init(pio, ook_offset, ook_gpio, ook_microseconds_per_bit);

    uint32_t data = 0x1234567f;
    for (;;) {
        printf("sending data: 0x%08lx\n", data);
        gpio_put(debug_gpio, 1);
        ook_send(data);
        gpio_put(debug_gpio, 0);
        ++data;
    }
}
