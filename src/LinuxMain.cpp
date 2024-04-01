#include "modbusServer/ModbusServer.hpp"
#include "picoController/LinuxController.hpp"
#include "powerSupply/PowerSupply.hpp"

int main() {

    ModbusServer modbusServer(std::make_unique<PowerSupply>(), std::make_shared<LinuxController>());

    while (true) {
        modbusServer.waitAndHandleRequest();
    }

    return 0;
}