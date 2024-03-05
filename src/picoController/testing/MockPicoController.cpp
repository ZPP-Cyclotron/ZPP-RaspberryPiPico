#include "MockPicoController.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

static int32_t mockReadSerialStdin(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    auto requestAndResponsePtr = (struct requestAndResponse *) arg;
    auto start = requestAndResponsePtr->requestIdx;

    for (int i = start; i < count + start; i++) {
        if (i == requestAndResponsePtr->requestLength)
            return i;

        buf[i - start] = requestAndResponsePtr->request[i];
        (requestAndResponsePtr->requestIdx)++;
    }

    return count;
}

void checkResponse(const uint8_t response[], uint16_t count, const uint8_t expectedResponse[], uint16_t expectedCount) {
    ASSERT_TRUE(expectedCount == count);
    for (int i = 0; i < count; i++)
        ASSERT_TRUE(response[i] == expectedResponse[i]);
}

static int32_t mockWriteSerialStdout(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg) {
    auto requestAndResponsePtr = (struct requestAndResponse *) arg;

    checkResponse(buf, count, requestAndResponsePtr->response, requestAndResponsePtr->responseLength);

    return count;
}

void hexToArray(std::string &s, uint8_t *arr) {
    assert(s.length() % 2 == 0);

    for (int i = 0; i < s.length(); i += 2) {
        arr[i / 2] = std::stoi(s.substr(i, 2), nullptr, 16);
    }
}

void MockPicoController::assign_read_and_write_to_modbus(nmbs_platform_conf &platform_conf) {
    platform_conf.arg = &this->requestAndResponse;

    platform_conf.read = mockReadSerialStdin;
    platform_conf.write = mockWriteSerialStdout;
}


MockPicoController::MockPicoController(std::string &request, std::string &response) {

    assert(request.length() / 2 <= MSG_MAX_LENGTH && response.length() / 2 <= MSG_MAX_LENGTH);

    hexToArray(request, this->requestAndResponse.request);
    hexToArray(response, this->requestAndResponse.response);

    if (this->requestAndResponse.request[0] != 1)
        printf("REQUEST ERROR\n");

    this->requestAndResponse.requestLength = (int) request.length() / 2;
    this->requestAndResponse.responseLength = (int) response.length() / 2;

//    ON_CALL(*this, assign_read_and_write_to_modbus(testing::_)).WillByDefault(
//            [this](nmbs_platform_conf &platform_conf) { return this->mockAssignReadAndWriteToModbus(platform_conf); });

    ON_CALL(*this, onError()).WillByDefault([this]() { std::cout << "Modbus error\n"; });
}