#ifndef ZPP_RASPBERRYPIPICO_LINUXCONTROLLER_HPP
#define ZPP_RASPBERRYPIPICO_LINUXCONTROLLER_HPP

#include "PicoControllerInterface.hpp"
#include <sys/select.h>
#include <cstdio>

class LinuxController : public PicoControllerInterface {
public:

    void onError() override {
        perror("Modbus error\n");
    }

    void onErrorWithMsg(const char errMsg[]) override {
        perror(errMsg);
    }

    void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) override;
};


#endif //ZPP_RASPBERRYPIPICO_LINUXCONTROLLER_HPP
