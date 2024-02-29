#include "MockPicoController.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <string>

static int32_t mockReadSerialStdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    auto * requestResponse = (std::vector <std::vector<uint8_t>> *) arg;
    std::vector<uint8_t> mockRequest = (*requestResponse)[0];

    for (int i = 0; i < count; i++) {
        if (i == mockRequest.size())
            return i;

        buf[i] = mockRequest[i];
    }

    return count;
}

static int32_t mockWriteSerialStdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    auto * requestResponse = (std::vector <std::vector<uint8_t>> *) arg;
    std::vector <uint8_t> expectedResponse = (*requestResponse)[1];

    std::vector<uint8_t> bufVec(buf, buf + count);

    EXPECT_TRUE(bufVec == expectedResponse);
}

std::vector <uint8_t> hexToVector(std::string &s) {
    assert(s.length() % 2 == 0);

    std::vector <uint8_t> v(s.length() / 2);
    for (int i = 0; i < s.length(); i += 2) {
        v[i / 2] = std::stoi(s.substr(i, 2), nullptr, 16);
    }
    return v;
}

void MockPicoController::mockAssignReadAndWriteToModbus(nmbs_platform_conf &platform_conf) {
    std::vector <std::vector<uint8_t>> requestAndResponse = {hexToVector(this->request), hexToVector(this->response)};
    platform_conf.arg = (void *) (&requestAndResponse);

    platform_conf.read = mockReadSerialStdin;
    platform_conf.write = mockWriteSerialStdout;
}


MockPicoController::MockPicoController(std::string &request, std::string &response) {
    this->request = request;
    this->response = response;

    ON_CALL(*this, assign_read_and_write_to_modbus(testing::_)).WillByDefault(
            [this](nmbs_platform_conf &platform_conf) { return this->mockAssignReadAndWriteToModbus(platform_conf); });

    ON_CALL(*this, onError()).WillByDefault([this]() {std::cout<<"Modbus error\n";});
}