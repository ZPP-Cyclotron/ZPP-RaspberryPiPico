#include "ModbusServer.hpp"

using namespace std;

ModbusServer::ModbusServer(unique_ptr <PowerSupplyInterface> powerSupplyArg,
                           shared_ptr <PicoControllerInterface> picoController) {
    powerSupply = move(powerSupplyArg);
    this->picoController = picoController;
    nmbs_platform_conf platform_conf;
    platform_conf.transport = NMBS_TRANSPORT_RTU;

    picoController->assign_read_and_write_to_modbus(platform_conf);

    nmbs_callbacks callbacks = {0};
    callbacks.read_input_registers = handleReadData;
    callbacks.write_single_register = handleSetCurrent;
    callbacks.write_multiple_coils = handleSetPowerSupplyStatus;

    nmbs_error err = nmbs_server_create(&nmbs, RTU_SERVER_ADDRESS, &platform_conf, &callbacks);

    if (err != NMBS_ERROR_NONE)
        picoController->onError();

}

void ModbusServer::waitAndHandleRequest() {
    nmbs_error err = nmbs_server_poll(&nmbs); // TODO implement interrupts with buffer for received messages.
    if (err != NMBS_ERROR_NONE)
        printf("Error: %s", nmbs_strerror(err));
}
