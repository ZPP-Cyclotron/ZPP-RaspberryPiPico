#include <pico/stdlib.h>
#include <stdio.h>
#include <string>

class InterruptExample {

public:

    InterruptExample(uint gpio_in_pin) : gpio_in_pin_(gpio_in_pin) {
        uint32_t event_mask = GPIO_IRQ_EDGE_RISE;
        gpio_set_irq_enabled_with_callback(gpio_in_pin, event_mask, ENABLE, gpio_irq_callback);
    }


private:

    static auto gpio_irq_callback(uint gpio, uint32_t event_mask) -> void {

        printf("Interrupt callback\n");
    }


    static const bool ENABLE = true;

    uint gpio_in_pin_;
};

int main()
{
    static const uint OUT_PIN = 0; // 14
    static const uint IN_PIN  = 1; // 15

    stdio_init_all();

    InterruptExample interruptExample(IN_PIN);

    while (true);
    return 0;
}