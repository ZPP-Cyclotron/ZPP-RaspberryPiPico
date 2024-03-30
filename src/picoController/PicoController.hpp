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

    PicoController();

    void onError() override {
        gpio_put(LED_PIN, 1);
    }

    void onErrorWithMsg(const char errMsg[]) override {}

    void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) override;

};


#endif //ZPP_CYCLOTRON_PICOCONTROLLER_HPP
