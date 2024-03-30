#include "ModbusServer.hpp"

static const uint8_t REGISTERS_READ_SIZE = 2;
static const uint8_t WRITTEN_DATA_TYPE_COILS = 2;
static const uint8_t COILS_WRITE_MIN_COUNT = WRITTEN_DATA_TYPE_COILS + 1;
static const uint8_t RTU_SERVER_ADDRESS = 1;

static std::unique_ptr<PowerSupplyInterface> powerSupply;

ModbusServer::ModbusServer(std::unique_ptr<PowerSupplyInterface> powerSupplyArg,
                           const std::shared_ptr<PicoControllerInterface> &picoController) {
    powerSupply = std::move(powerSupplyArg);
    this->picoController = picoController;
    nmbs_platform_conf platform_conf;
    platform_conf.transport = NMBS_TRANSPORT_RTU;

    picoController->assign_read_and_write_to_modbus(platform_conf);

    nmbs_callbacks callbacks = {};
    callbacks.read_input_registers = handleReadData;
    callbacks.write_multiple_coils = handleWriteData;

    nmbs_error err = nmbs_server_create(&nmbs, RTU_SERVER_ADDRESS, &platform_conf, &callbacks);

    if (err != NMBS_ERROR_NONE)
        picoController->onError();

}

nmbs_error ModbusServer::handleReadData(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id,
                                        void *arg) {
    if (address != 0 || quantity != REGISTERS_READ_SIZE)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    uint16_t current = powerSupply->readCurrent();
    bool isOn = powerSupply->isPowerCircuitOn();
    bool polarity = powerSupply->readPolarity();
    bool reset = powerSupply->readReset();
    bool remote = powerSupply->isRemote();

        registers_out[0] =
            current + (isOn << powerSupply->currentReadBits) + (polarity << (powerSupply->currentReadBits + 1)) +
            (reset << (powerSupply->currentReadBits + 2)) + (remote << (powerSupply->currentReadBits + 3));

    uint16_t voltage = powerSupply->readVoltage();
    uint8_t errors = powerSupply->readErrors();

    registers_out[1] = voltage + (errors << powerSupply->voltageReadBits);

    return NMBS_ERROR_NONE;
}

nmbs_error
ModbusServer::handleWriteData(uint16_t address, uint16_t quantity, const nmbs_bitfield coils, uint8_t unit_id,
                              void *arg) {

    if (address != 0 || quantity < COILS_WRITE_MIN_COUNT)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    uint8_t dataType = (nmbs_bitfield_read(coils, 1) << 1) + nmbs_bitfield_read(coils, 0);

    uint16_t value = 0 ;
    
    for (int i = WRITTEN_DATA_TYPE_COILS; i < quantity; i++) {
        value += nmbs_bitfield_read(coils, i) << (quantity - i - 1);
    }

    if (powerSupply->setStatus(dataType, value) < 0)
        return NMBS_EXCEPTION_ILLEGAL_DATA_VALUE;

    return NMBS_ERROR_NONE;
}


void ModbusServer::waitAndHandleRequest() {
    nmbs_error err = nmbs_server_poll(&nmbs);

    if (err == NMBS_FIRST_BYTE_TIMEOUT)
        powerSupply->safeCommunicationWithPS();

    if (err != NMBS_ERROR_NONE)
        ModbusServer::picoController->onErrorWithMsg(nmbs_strerror(err));

}
