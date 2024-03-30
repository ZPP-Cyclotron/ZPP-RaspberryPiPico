#include "modbusServer/ModbusServer.hpp"
#include "picoController/PicoController.hpp"
#include "powerSupply/PowerSupply200A.hpp"
#include "powerSupply/PowerSupply.hpp"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();

    std::unique_ptr<PowerSupplyInterface> ps;

    switch (POWER_SUPPLY) {
        case 200:
            ps = std::make_unique<PowerSupply200A>();
            break;
        case 100:
            // ps = std::make_unique<PowerSupply100A>();
            // break;
        default:
            ps = std::make_unique<PowerSupply>();
    }

    ModbusServer modbusServer(std::move(ps), std::make_shared<PicoController>());

    while (true) {
        modbusServer.waitAndHandleRequest();
    }

    return 0;
}