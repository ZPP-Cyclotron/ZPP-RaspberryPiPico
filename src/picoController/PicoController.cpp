#include "PicoController.hpp"
#include <unistd.h>

static int32_t read_serial_stdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    for (int i = 0; i < count; i++) {

        if (!uart_is_readable_within_us(UART_ID, PicoController::GET_ONE_BYTE_TIMEOUT))
            return i;

        uart_read_blocking(UART_ID, buf + i, 1);
    }
    return count;
}

static int32_t write_serial_stdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    gpio_put(PicoController::LED_PIN, 1);

    uart_write_blocking(UART_ID, buf, count);

    return count;
}

void PicoController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.read = read_serial_stdin;

    platform_conf.write = write_serial_stdout;
}


