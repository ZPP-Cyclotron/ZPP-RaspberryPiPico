#include "../src/modbusServer/ModbusServer.hpp"
#include "../src/picoController/testing/MockPicoController.hpp"
#include "../src/powerSupply/testing/MockPowerSupply.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace std;

TEST(ModbusSlaveTest, RespondsToCorrectReadRequest) {
    MockPowerSupply mockPowerSupply;

    uint16_t current = std::stoi("FFF", nullptr, 16);
    uint16_t voltage = std::stoi("FFF", nullptr, 16);
    bool isOn = 1;
    bool polarity = 1;
    bool reset = 1;
    bool remote = 1;
    uint8_t errors = std::stoi("F", nullptr, 16);

    EXPECT_CALL(mockPowerSupply, readCurrent()).Times(1).WillOnce(Return(current));
    EXPECT_CALL(mockPowerSupply, readVoltage()).Times(1).WillOnce(Return(voltage));
    EXPECT_CALL(mockPowerSupply, isPowerCircuitOn()).Times(1).WillOnce(Return(isOn));
    EXPECT_CALL(mockPowerSupply, readPolarity()).Times(1).WillOnce(Return(polarity));
    EXPECT_CALL(mockPowerSupply, readReset()).Times(1).WillOnce(Return(reset));
    EXPECT_CALL(mockPowerSupply, isRemote()).Times(1).WillOnce(Return(remote));
    EXPECT_CALL(mockPowerSupply, readErrors()).Times(1).WillOnce(Return(errors));

    string request = "01" // Slave address.
                    + "04" // Modbus function.
                    + "0000" // Address of first register to be read.
                    + "0002" // Number of registers to be read.
                    + "71CB"; // CRC

    string response = "01" // Slave address.
                    + "04" // Modbus function.
                    + "04" // Byte count
                    + "FFFF" // Register 0
                    + "FFFF" // Register 1
                    + "FA10"; // CRC

    MockPicoController mockPicoController(request, response);

    ModbusServer modbusServer(make_unique<MockPowerSupply>(mockPowerSupply), make_shared<MockPicoController>(mockPicoController));

    modbusServer.waitAndHandleRequest();
}