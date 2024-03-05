#include "../src/modbusServer/ModbusServer.hpp"
#include "../src/picoController/testing/MockPicoController.hpp"
#include "../src/powerSupply/testing/MockPowerSupply.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <memory>


const std::string slaveAddr = "01";
const std::string modbusReadRegisters = "04";
const std::string modbusWriteCoils = "0F";

const std::string correctReadRequest = slaveAddr
                                       + modbusReadRegisters
                                       + "0000" // Address of first register to be read.
                                       + "0002" // Number of registers to be read.
                                       + "71CB"; // CRC

const std::string correctWriteCurrentResponse = slaveAddr
                                                + modbusWriteCoils
                                                + "0000" // Address of first coil to be written.
                                                + "0012" // Quantity of coils.
                                                + "D5C6"; // CRC

const std::string correctSwitchOnOffResponse = slaveAddr
                                               + modbusWriteCoils
                                               + "0000" // Address of first coil to be written.
                                               + "0003" // Quantity of coils.
                                               + "15CA";

TEST(ModbusServerTest, RespondsToCorrectReadRequest) {
    uint16_t current = std::stoi("FFF", nullptr, 16);
    uint16_t voltage = std::stoi("FFF", nullptr, 16);
    bool isOn = 1;
    bool polarity = 1;
    bool reset = 1;
    bool remote = 1;
    uint8_t errors = std::stoi("F", nullptr, 16);

    std::unique_ptr<MockPowerSupply> mockPowerSupply = std::make_unique<MockPowerSupply>(current, voltage, isOn,
                                                                                         polarity, reset, remote,
                                                                                         errors);
    std::string response = slaveAddr
                           + modbusReadRegisters
                           + "04" // Byte count
                           + "FFFF" // Register 0
                           + "FFFF" // Register 1
                           + "FA10"; // CRC

    std::string request = correctReadRequest;

    auto mockPicoController = std::make_shared<MockPicoController>(request, response);

    std::unique_ptr<ModbusServer> modbusServer(new ModbusServer(std::move(mockPowerSupply), mockPicoController));

    modbusServer->waitAndHandleRequest();
}

class ModbusServerWriteTest
        : public testing::TestWithParam<std::tuple<PowerSupplyParameter, int, std::string, std::string>> {
};

TEST_P(ModbusServerWriteTest, RespondsToCorrectWriteRequest) {
    auto [dataType, value, request, response] = ModbusServerWriteTest::GetParam();

    std::unique_ptr<MockPowerSupply> mockPowerSupply = std::make_unique<MockPowerSupply>(static_cast<int>(dataType),
                                                                                         value);

    auto mockPicoController = std::make_shared<MockPicoController>(request, response);

    std::unique_ptr<ModbusServer> modbusServer(new ModbusServer(std::move(mockPowerSupply), mockPicoController));

    modbusServer->waitAndHandleRequest();
}

INSTANTIATE_TEST_SUITE_P(
        ModbusServerTest, ModbusServerWriteTest,
        testing::Values(
                std::make_tuple(PowerSupplyParameter::CURRENT, std::stoi("FFFF", nullptr, 16),
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0012" // Quantity of coils.
                                + "03" // Byte count.
                                + "FFFF03" // Coils' values.
                                + "E9B4" // CRC
                        , correctWriteCurrentResponse),
                std::make_tuple(PowerSupplyParameter::IS_ON, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "04" //Coils' values.
                                + "8E94" // CRC
                        , correctSwitchOnOffResponse),
                std::make_tuple(PowerSupplyParameter::POLARITY, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "05" //Coils' values.
                                + "4F54" // CRC
                        , correctSwitchOnOffResponse),
                std::make_tuple(PowerSupplyParameter::RESET, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "06" //Coils' values.
                                + "0F55" // CRC
                        , correctSwitchOnOffResponse)
        ));


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}