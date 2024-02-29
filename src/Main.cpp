#include "modbusServer/ModbusServer.hpp"
#include "picoController/PicoController.hpp"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();

    ModbusServer modbusServer(std::make_unique<PowerSupply>(), std::make_shared<PicoController>());

    while (true) {
        modbusServer.waitAndHandleRequest();
    }

    return 0;
}