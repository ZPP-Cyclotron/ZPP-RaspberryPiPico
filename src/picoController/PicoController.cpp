#include "PicoController.hpp"
#include <unistd.h>
#include <cstdio>

static int32_t read_serial_stdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    for (int i = 0; i < count; i++) {
        int c = getchar_timeout_us(PicoController::GET_ONE_BYTE_TIMEOUT); // TODO: Consider using stdio_set_chars_available_callback()

        if (c == PICO_ERROR_TIMEOUT || c == PicoController::ENDSTDIN)
            return i;

        buf[i] = c;
    }
    return count;
}

static int32_t write_serial_stdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    fflush(stdout);

    if (write(STDOUT_FILENO, buf, count) < 0)
        return -1;

    return count;
}

void PicoController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.read = read_serial_stdin;

    platform_conf.write = write_serial_stdout;
}


