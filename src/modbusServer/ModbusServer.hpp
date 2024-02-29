#ifndef ZPP_CYCLOTRON_MODBUSSERVER_H
#define ZPP_CYCLOTRON_MODBUSSERVER_H

#include <cstdio>
#include <map>
#include <string>
#include <memory>
#include "../powerSupply/PowerSupply.hpp"
#include "../picoController/PicoControllerInterface.hpp"

static const uint8_t REGISTERS_READ_SIZE = 2;
static const uint8_t REGISTERS_WRITE_SIZE = 1;
static const uint8_t COILS_WRITE_COUNT = 3;
static const uint8_t RTU_SERVER_ADDRESS = 1;

static std::unique_ptr<PowerSupplyInterface> powerSupply; // TODO find a way to make this variable local.

class ModbusServer {

private:

    static const bool ENABLE = true;
    static const bool DISABLE = false;

    std::shared_ptr<PicoControllerInterface> picoController;

    nmbs_t nmbs;

    static nmbs_error handleReadData(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id,
                                     void *arg) {
        if (address != 0 || quantity != REGISTERS_READ_SIZE)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        uint16_t current = powerSupply->readCurrent();
        bool isOn = powerSupply->isPowerCircuitOn();
        bool polarity = powerSupply->readPolarity();
        bool remote = powerSupply->isRemote();

        registers_out[0] =
                current + (isOn << powerSupply->currentReadBits) + (polarity << (powerSupply->currentReadBits + 1)) +
                (remote << (powerSupply->currentReadBits + 2));

        uint16_t voltage = powerSupply->readVoltage();
        uint8_t errors = powerSupply->readErrors();

        registers_out[1] = voltage + (errors << powerSupply->voltageReadBits);

        return NMBS_ERROR_NONE;
    }

    static nmbs_error handleSetCurrent(uint16_t address, uint16_t value, uint8_t unit_id, void *arg) {
        if (address >= REGISTERS_WRITE_SIZE)
            return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

        if (powerSupply->setCurrent(value) < 0)
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

        if (powerSupply->setStatus(dataType, value) < 0)
            return NMBS_EXCEPTION_ILLEGAL_DATA_VALUE;

        return NMBS_ERROR_NONE;
    }


public:

    ModbusServer(std::unique_ptr<PowerSupplyInterface> powerSupplyArg, const std::shared_ptr <PicoControllerInterface> &picoController);

    void waitAndHandleRequest();

};


#endif //ZPP_CYCLOTRON_MODBUSSERVER_H
