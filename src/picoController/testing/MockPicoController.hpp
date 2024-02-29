#ifndef ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
#define ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP

#include "../PicoControllerInterface.hpp"
#include <gmock/gmock.h>

class MockPicoController : public PicoControllerInterface {
    std::string request;
    std::string response;

public:
    MockPicoController(std::string &request, std::string &response);

    MOCK_METHOD(void, onError, (), (override));

    void onErrorWithMsg(const char errMsg[]) override {
        std::cout << errMsg;
    }

    MOCK_METHOD(void, assign_read_and_write_to_modbus, (nmbs_platform_conf &platform_conf), (override));

    void mockAssignReadAndWriteToModbus(nmbs_platform_conf &platform_conf);
    };

#endif //ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
