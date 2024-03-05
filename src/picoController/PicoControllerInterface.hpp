#ifndef ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP
#define ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP

#include <cstdint>

extern "C" {
#include "../nanomodbus/nanomodbus.h"
}

typedef int32_t (*read_function)(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg);
typedef int32_t (*write_function)(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg);

class PicoControllerInterface {

public:

    virtual ~PicoControllerInterface() = default;

    virtual void onError() = 0;

    virtual void onErrorWithMsg(const char errMsg[]) = 0;

    virtual void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) = 0;

};

#endif //ZPP_CYCLOTRON_PICOCONTROLLERINTERFACE_HPP