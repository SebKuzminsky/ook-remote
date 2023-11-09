#include <cstdio>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"

#include "ook.pio.h"


// Rising edge on this GPIO indicates beginning of a transmission,
// used to trigger a scope for debugging.
uint const debug_gpio = 1;


// Process commands received from the computer via USB serial.
// Valid input strings:
//     'txN': N is a 32-bit hex string.  Transmit the hex string.
void process_input(char const * in) {
    if (strncasecmp("tx", in, 2) == 0) {
        uint32_t data = 0;
        sscanf(&in[2], "%lx", &data);
        printf("tx 0x%08lx\n", data);
        gpio_put(debug_gpio, 1);
        ook_send(data);
        gpio_put(debug_gpio, 0);
    } else {
        printf("unknown input '%s'\n", in);
    }
}


void read_serial(void) {
    static char in[16];
    static int index = 0;

    // Block waiting for an input character.
    in[index] = getchar();
    if (isspace(in[index])) {
        in[index] = '\0';
        process_input(in);
        index = 0;
        return;
    }
    ++index;
    if (index == sizeof(in)) {
        printf("input buffer overflow, discarding input '%s'\n", in);
        index = 0;
    }
}


int main() {
    PIO pio = pio0;
    uint const ook_gpio = 0;
    uint const ook_microseconds_per_bit = 625;

    stdio_init_all();
    clocks_init();

    gpio_init(debug_gpio);
    gpio_set_function(ook_gpio, GPIO_FUNC_SIO);
    gpio_set_dir(debug_gpio, true);  // output
    gpio_put(debug_gpio, 0);

    gpio_init(ook_gpio);
    gpio_set_function(ook_gpio, GPIO_FUNC_PIO0);
    gpio_set_dir(ook_gpio, true);  // output

    uint ook_offset = pio_add_program(pio, &ook_program);
    ook_init(pio, ook_offset, ook_gpio, ook_microseconds_per_bit);

    for (;;) {
        read_serial();
    }
}
