#include <pico/stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

extern "C" {
#include "nanomodbus.h"
}

static const uint8_t COILS_ADDR_MAX = 100;
static const uint8_t REGS_ADDR_MAX = 32;
static const uint8_t FILE_SIZE_MAX = 32;
static const uint8_t RTU_SERVER_ADDRESS = 1;

nmbs_t nmbs;

// A single nmbs_bitfield variable can keep 2000 coils
nmbs_bitfield server_coils = {0};
uint16_t server_registers[REGS_ADDR_MAX] = {0};
uint16_t server_file[FILE_SIZE_MAX];

class ModbusServer {

public:

    ModbusServer(uint gpio_in_pin): gpio_in_pin_(gpio_in_pin) {

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);

        nmbs_platform_conf platform_conf;
        platform_conf.transport = NMBS_TRANSPORT_RTU;
        platform_conf.read = read_serial;
        platform_conf.write = write_serial;
        platform_conf.arg = NULL;

        nmbs_callbacks callbacks = {0};
        callbacks.read_coils = handle_read_coils;

        nmbs_error err = nmbs_server_create(&nmbs, RTU_SERVER_ADDRESS, &platform_conf, &callbacks);
        if (err != NMBS_ERROR_NONE) {
            onError();
        }

        int led = LED_PIN;
        nmbs_set_platform_arg(&nmbs, &led);

    }


private:

    static int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg) {

        for (int i = 0; i < count; i++) {
            buf[i] = getchar_timeout_us(GET_ONE_CHAR_TIMEOUT);

            if (buf[i] == ENDSTDIN)
                return i;
        }
        return count;
    }

    static int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg) {
        printf("Responseaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");

        gpio_put(LED_PIN, 1);

        return count;
    }


    static nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void* arg) {

        if (address + quantity > COILS_ADDR_MAX + 1)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        for (int i = 0; i < quantity; i++) {
            bool value = nmbs_bitfield_read(server_coils, address + i);
            nmbs_bitfield_write(coils_out, i, value);
        }

        return NMBS_ERROR_NONE;
    }

    void onError() {
//        gpio_put(LED_PIN, 1);
    }

    static const bool ENABLE = true;
    static const bool DISABLE = false;
    static const uint8_t ENDSTDIN = 255;
    static const int GET_ONE_CHAR_TIMEOUT = 1000;
    static const int LED_PIN = PICO_DEFAULT_LED_PIN;

    uint gpio_in_pin_;
};

int main() {
    static const uint OUT_PIN = 0; // 14
    static const uint IN_PIN  = 1; // 15

    stdio_init_all();

    ModbusServer modbusServer(IN_PIN);

    while (true) {
        nmbs_error err = nmbs_server_poll(&nmbs);
    }

    return 0;
}