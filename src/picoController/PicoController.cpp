#include "PicoController.hpp"
#include <unistd.h>

static int32_t read_serial_stdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    for (int i = 0; i < count; i++) {
        buf[i] = getchar_timeout_us(PicoController::GET_ONE_CHAR_TIMEOUT);

        if (buf[i] == PicoController::ENDSTDIN)
            return i;
    }
    return count;
}

static int32_t write_serial_stdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
//        if (count != 8)
//            printf("Wrong bytes count: %d\n", count);

    if (write(STDOUT_FILENO, buf, count) < 0)
        return -1;

    return count;
}

void PicoController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.read = read_serial_stdin;

    platform_conf.write = write_serial_stdout;
}


