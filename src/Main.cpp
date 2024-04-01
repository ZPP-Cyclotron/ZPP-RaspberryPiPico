#include "modbusServer/ModbusServer.hpp"
#include "picoController/PicoController.hpp"
#include <stdio.h>
#include "pico/stdlib.h"
extern "C" {
#include "pico/stdlib.h"
#include "pico/stdio.h"
}
#ifdef PS200A
    #include "powerSupply/PowerSupply200A.hpp"
#else
    #include "powerSupply/PowerSupply.hpp"
#endif

int main() {
    stdio_init_all();

    //delay for usb printf
    sleep_ms(1000);
    printf("start\n");
    
    std::make_unique<PowerSupply>().get()->safeCommunicationWithPS();
    ModbusServer modbusServer(std::make_unique<PowerSupply>(), std::make_shared<PicoController>());

    while (true) {
        modbusServer.waitAndHandleRequest();
    }

    return 0;
}