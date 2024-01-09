#include <pico/stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include "../PowerSupply.hpp"

extern "C" {
#include "nanomodbus.h"
#include "pico/stdio.h"
#include <unistd.h>
}

static const uint8_t REGISTERS_READ_SIZE = 2;
static const uint8_t REGISTERS_WRITE_SIZE = 1;
static const uint8_t COILS_WRITE_COUNT = 3;
static const uint8_t RTU_SERVER_ADDRESS = 1;

nmbs_t nmbs;

class ModbusServer {

public:

    ModbusServer(PowerSupply ps) {
        nmbs_platform_conf platform_conf;
        platform_conf.transport = NMBS_TRANSPORT_RTU;
        platform_conf.read = read_serial;
        platform_conf.write = write_serial;
        platform_conf.arg = NULL;

        nmbs_callbacks callbacks = {0};
        callbacks.read_input_registers = handleReadData;
        callbacks.write_single_register = handleSetCurrent;
        callbacks.write_multiple_coils = handleSetPowerSupplyStatus;

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);

        nmbs_error err = nmbs_server_create(&nmbs, RTU_SERVER_ADDRESS, &platform_conf, &callbacks);

        if (err != NMBS_ERROR_NONE)
            onError();

        powerSupply = ps;
    }


private:

    static int32_t read_serial(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

        for (int i = 0; i < count; i++) {
            buf[i] = getchar_timeout_us(GET_ONE_CHAR_TIMEOUT);

            if (buf[i] == ENDSTDIN)
                return i;
        }
        return count;
    }

    static int32_t write_serial(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
//        if (count != 8)
//            printf("Wrong bytes count: %d\n", count);

        if (write(STDOUT_FILENO, buf, count) < 0)
            return -1;

        return count;
    }

    static nmbs_error handleReadData(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id,
                                void *arg) {
        if (address != 0 || quantity != REGISTERS_READ_SIZE)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        uint16_t current = powerSupply.readCurrent();
        bool isOn = powerSupply.isPowerCircuitOn();
        bool polarity = powerSupply.readPolarity();
        bool remote = powerSupply.isRemote();

        registers_out[0] =
                current + (isOn << powerSupply.currentReadBits) + (polarity << (powerSupply.currentReadBits + 1)) +
                (remote << (powerSupply.currentReadBits + 2));

        uint16_t voltage = powerSupply.readVoltage();
        uint8_t errors = powerSupply.readErrors();

        registers_out[1] = voltage + (errors << powerSupply.voltageReadBits);

        return NMBS_ERROR_NONE;
    }

    static nmbs_error handleSetCurrent(uint16_t address, uint16_t value, uint8_t unit_id, void *arg) {
        if (address >= REGISTERS_WRITE_SIZE)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        if (powerSupply.setCurrent(value) < 0)
            return NMBS_EXCEPTION_ILLEGAL_DATA_VALUE;

        return NMBS_ERROR_NONE;
    }

    static nmbs_error
    handleSetPowerSupplyStatus(uint16_t address, uint16_t quantity, const nmbs_bitfield coils, uint8_t unit_id,
                               void *arg) {

        if (address != 0 || quantity != COILS_WRITE_COUNT)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        bool value = nmbs_bitfield_read(coils, 2);

        uint8_t dataType = (nmbs_bitfield_read(coils, 0) << 1) + nmbs_bitfield_read(coils, 1);

        if (powerSupply.setStatus(dataType, value) < 0)
            return NMBS_EXCEPTION_ILLEGAL_DATA_VALUE;

        return NMBS_ERROR_NONE;
    }

    void onError() {
        gpio_put(LED_PIN, 1);
    }

    static const bool ENABLE = true;
    static const bool DISABLE = false;
    static const uint8_t ENDSTDIN = 255;
    static const int GET_ONE_CHAR_TIMEOUT = 1000;
    static const int LED_PIN = PICO_DEFAULT_LED_PIN;

    uint gpio_in_pin_;

    static PowerSupply powerSupply;
};

int main() {
    stdio_init_all();

    PowerSupply powerSupply;
    ModbusServer modbusServer(powerSupply);

    while (true) {
        nmbs_error err = nmbs_server_poll(&nmbs); // TODO implement interrupts with buffer for received messages.
        if (err != NMBS_ERROR_NONE)
            printf("Error: %s", nmbs_strerror(err));
    }

    return 0;
}