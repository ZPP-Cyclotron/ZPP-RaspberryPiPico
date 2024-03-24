#include "PicoController.hpp"
#include <unistd.h>

PicoController::PicoController() {
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}

static int32_t readUart(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, bool first_byte_from_msg, void *arg) {

    for (int i = 0; i < count; i++) {

        if (!first_byte_from_msg && !uart_is_readable_within_us(UART_ID, PicoController::GET_ONE_BYTE_TIMEOUT))
            return i;

        uart_read_blocking(UART_ID, buf + i, 1);
    }
    return count;
}

static int32_t writeUart(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    gpio_put(PicoController::LED_PIN, 1);

    uart_write_blocking(UART_ID, buf, count);

    return count;
}

void PicoController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.read = readUart;

    platform_conf.write = writeUart;
}


