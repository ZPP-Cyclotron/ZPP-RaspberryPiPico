#ifndef ZPP_CYCLOTRON_PICOCONTROLLER_HPP
#define ZPP_CYCLOTRON_PICOCONTROLLER_HPP

#include "PicoControllerInterface.hpp"

extern "C" {
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
}

#define UART_ID uart0
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

class PicoController : public PicoControllerInterface {

public:
    static const int LED_PIN = PICO_DEFAULT_LED_PIN;

    PicoController() {
        // Set up our UART with a basic baud rate.
        uart_init(UART_ID, 2400);

        // Set the TX and RX pins by using the function select on the GPIO
        // Set datasheet for more information on function select
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

        // Actually, we want a different speed
        // The call will return the actual baud rate selected, which will be as close as
        // possible to that requested
        int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

        // Set UART flow control CTS/RTS, we don't want these, so turn them off
        uart_set_hw_flow(UART_ID, false, false);

        // Set our data format
        uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

        // Turn off FIFO's - we want to do this character by character
        uart_set_fifo_enabled(UART_ID, false);

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        gpio_put(LED_PIN, 0);
    }

    void onError() override {
//        gpio_put(LED_PIN, 1);
    }

    void onErrorWithMsg(const char errMsg[]) override {}

    void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) override;

};


#endif //ZPP_CYCLOTRON_PICOCONTROLLER_HPP
