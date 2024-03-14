#ifndef ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP
#define ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP

#include <cstdint>
#include "../../config.h"

extern "C" {
#include "../nanomodbus/nanomodbus.h"
}

class PicoControllerInterface {

public:

#ifdef MODBUS_TIMEOUT
    static const int GET_ONE_BYTE_TIMEOUT = MODBUS_TIMEOUT;
#else
    static const int GET_ONE_BYTE_TIMEOUT = 500000; // microseconds
#endif

    virtual ~PicoControllerInterface() = default;

    virtual void onError() = 0;

    virtual void onErrorWithMsg(const char errMsg[]) = 0;

    virtual void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) = 0;

};

#endif //ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP