#ifndef ZPP_CYCLOTRON_MODBUSSERVER_H
#define ZPP_CYCLOTRON_MODBUSSERVER_H

#include <cstdio>
#include <map>
#include <string>
#include <memory>
#include "../powerSupply/PowerSupplyInterface.hpp"
#include "../picoController/PicoControllerInterface.hpp"

class ModbusServer {

private:
    std::shared_ptr<PicoControllerInterface> picoController;

    nmbs_t nmbs;

    static nmbs_error handleReadData(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id,
                                     void *arg);

    static nmbs_error
    handleWriteData(uint16_t address, uint16_t quantity, const nmbs_bitfield coils, uint8_t unit_id,
                    void *arg);

public:

    ModbusServer(std::unique_ptr<PowerSupplyInterface> powerSupplyArg,
                 const std::shared_ptr<PicoControllerInterface> &picoController);

    void waitAndHandleRequest();

};


#endif //ZPP_CYCLOTRON_MODBUSSERVER_H
