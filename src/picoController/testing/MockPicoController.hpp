#ifndef ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
#define ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP

#include "../PicoControllerInterface.hpp"
#include <gmock/gmock.h>

static const int MSG_MAX_LENGTH = 20;

struct requestAndResponse {
    uint8_t request[MSG_MAX_LENGTH];
    uint8_t response[MSG_MAX_LENGTH];
    int requestLength;
    int requestIdx = 0;
    int responseLength;
};


class MockPicoController : public PicoControllerInterface {

    struct requestAndResponse requestAndResponse{};

public:
    MockPicoController(std::string &request, std::string &response);

    MOCK_METHOD(void, onError, (), (override));

    void onErrorWithMsg(const char errMsg[]) override {
        std::cout << errMsg;
    }

//    MOCK_METHOD(void, assign_read_and_write_to_modbus, (nmbs_platform_conf & platform_conf), (override));

    void assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) override;
};

#endif //ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
