#include "../src/modbusServer/ModbusServer.hpp"
#include "../src/picoController/LinuxController.hpp"
#include "../src/powerSupply/testing/MockPowerSupply.hpp"
#include <gtest/gtest.h>
#include <vector>

static const std::string slaveAddr = "01";
static const std::string modbusReadRegisters = "04";

TEST(ModbusSlaveTest, RespondsToCorrectReadRequest) {
    std::unique_ptr<MockPowerSupply> mockPowerSupply;

    uint16_t current = std::stoi("FFF", nullptr, 16);
    uint16_t voltage = std::stoi("FFF", nullptr, 16);
    bool isOn = 1;
    bool polarity = 1;
    bool reset = 1;
    bool remote = 1;
    uint8_t errors = std::stoi("F", nullptr, 16);

    EXPECT_CALL(*mockPowerSupply, readCurrent()).Times(1).WillOnce(testing::Return(current));
    EXPECT_CALL(*mockPowerSupply, readVoltage()).Times(1).WillOnce(testing::Return(voltage));
    EXPECT_CALL(*mockPowerSupply, isPowerCircuitOn()).Times(1).WillOnce(testing::Return(isOn));
    EXPECT_CALL(*mockPowerSupply, readPolarity()).Times(1).WillOnce(testing::Return(polarity));
    EXPECT_CALL(*mockPowerSupply, readReset()).Times(1).WillOnce(testing::Return(reset));
    EXPECT_CALL(*mockPowerSupply, isRemote()).Times(1).WillOnce(testing::Return(remote));
    EXPECT_CALL(*mockPowerSupply, readErrors()).Times(1).WillOnce(testing::Return(errors));

    std::string request = slaveAddr
                    + modbusReadRegisters
                    + "0000" // Address of first register to be read.
                    + "0002" // Number of registers to be read.
                    + "71CB"; // CRC

    std::string response = slaveAddr
                    + modbusReadRegisters
                    + "04" // Byte count
                    + "FFFF" // Register 0
                    + "FFFF" // Register 1
                    + "FA10"; // CRC

//    std::unique_ptr<PowerSupplyInterface> powerSupply = std::move(mockPowerSupply);
//    ModbusServer modbusServer(std::move(powerSupply), std::make_shared<LinuxController>());


    ModbusServer modbusServer(std::move(mockPowerSupply), std::make_shared<LinuxController>());

//    modbusServer.waitAndHandleRequest();
}