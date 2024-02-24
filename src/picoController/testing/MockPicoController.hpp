#ifndef ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
#define ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP

#include "../PicoControllerInterface.hpp"
#include <gmock/gmock.h>

class MockPicoController : PicoControllerInterface {
    string request;
    string response;

public:
    MockPicoController(string &request, string &response);

    MOCK_METHOD(void, onError, (), (override));

    MOCK_METHOD(void, assign_read_and_write_to_modbus, (nmbs_platform_conf &platform_conf), (override));

    void mockAssignReadAndWriteToModbus(nmbs_platform_conf &platform_conf);
    };

#endif //ZPP_CYCLOTRON_MOCKPICOCONTROLLER_HPP
