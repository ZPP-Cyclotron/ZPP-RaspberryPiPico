#ifndef ZPP_CYCLOTRON_PICOCONTROLLER_HPP
#define ZPP_CYCLOTRON_PICOCONTROLLER_HPP

#include "PicoControllerInterface.hpp"

extern "C" {
#include "pico/stdlib.h"
#include "pico/stdio.h"
}

class PicoController : public PicoControllerInterface {

private:

    uint gpio_in_pin_;

public:
    static const int LED_PIN = PICO_DEFAULT_LED_PIN;

    static const uint8_t ENDSTDIN = 255;

    PicoController() {
        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        gpio_put(LED_PIN, 1);
    }

    void onError() override {
//        gpio_put(LED_PIN, 1);
    }

    void onErrorWithMsg(const char errMsg[]) override {}

    void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) override;

};


#endif //ZPP_CYCLOTRON_PICOCONTROLLER_HPP
