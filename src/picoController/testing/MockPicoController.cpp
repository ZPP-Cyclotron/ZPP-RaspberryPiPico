#include "MockPicoController.hpp"
#include <#include <gtest/gtest.h>
#include <vector>
#include <iostream>

using namespace std;

static int32_t mockReadSerialStdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {

    vector <uint8_t> mockRequest = ((vector <vector<uint8_t>> *) arg)[0];

    for (int i = 0; i < count; i++) {
        if (i == mockRequest.size())
            return i;

        buf[i] = mockRequest[i];
    }

    return count;
}

static int32_t mockWriteSerialStdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    vector <uint8_t> expectedResponse = ((vector <vector<uint8_t>> *) arg)[1];

    std::vector<int> bufVec(buf, buf + count);

    EXPECT_TRUE(bufVec == expectedResponse);
}

vector <uint8_t> hexToVector(string &s) {
    ASSERT_TRUE(s.length() % 2 == 0);

    vector <uint8_t> v(s.length() / 2);
    for (int i = 0; i < s.length(); i += 2)
        v[i / 2] = stoi(s[i] + s[i + 1], nullptr, 16);

    return v;
}

void MockPicoController::mockAssignReadAndWriteToModbus(nmbs_platform_conf &platform_conf) {
    vector <vector<uint8_t>> requestAndResponse = {hexToVector(this->request), hexToVector(this->response)};
    platform_conf.arg = (void *) (&requestAndResponse);

    platform_conf.read = mockReadSerialStdin;
    platform_conf.write = mockWriteSerialStdout;
}


MockPicoController::MockPicoController(string &request, string &response) {
    this->request = request;
    this->response = response;

    ON_CALL(*this, assign_read_and_write_to_modbus(_)).WillByDefault(
            [this](nmbs_platform_conf &platform_conf) { return this->mockAssignReadAndWriteToModbus(platform_conf); });

    ON_CALL(*this, onError()).WillByDefault([this]() {cout<<"Modbus error\n";});
}