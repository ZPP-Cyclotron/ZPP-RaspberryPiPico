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

const std::string correctWriteOneBitValueResponse = slaveAddr
                                               + modbusWriteCoils
                                               + "0000" // Address of first coil to be written.
                                               + "0003" // Quantity of coils.
                                               + "15CA";

class ModbusServerReadTest
: public testing::TestWithParam<std::tuple<std::string, std::string, bool, bool, bool, bool, std::string, std::string>> {
};

TEST_P(ModbusServerReadTest, RespondsToCorrectReadRequest) {

    auto [currentHex, voltageHex, isOn, polarity, reset, remote, errorsHex, response] = ModbusServerReadTest::GetParam();

    uint16_t current = std::stoi(currentHex, nullptr, 16);
    uint16_t voltage = std::stoi(voltageHex, nullptr, 16);
    uint8_t errors = std::stoi(errorsHex, nullptr, 16);

    std::unique_ptr<MockPowerSupply> mockPowerSupply = std::make_unique<MockPowerSupply>(current, voltage, isOn,
                                                                                         polarity, reset, remote,
                                                                                         errors);
    std::string request = correctReadRequest;

    auto mockPicoController = std::make_shared<MockPicoController>(request, response);

    std::unique_ptr<ModbusServer> modbusServer(new ModbusServer(std::move(mockPowerSupply), mockPicoController));

    modbusServer->waitAndHandleRequest();
}

INSTANTIATE_TEST_SUITE_P(
        ModbusServerTest, ModbusServerReadTest,
        testing::Values(
                std::make_tuple("FFF", "FFF", 1, 1, 1, 1, "F",
                                slaveAddr
                                + modbusReadRegisters
                                + "04" // Byte count
                                + "FFFF" // Register 0
                                + "FFFF" // Register 1
                                + "FA10" // CRC
                                ),
                std::make_tuple("0", "0", 0, 0, 0, 0, "0",
                                slaveAddr
                                + modbusReadRegisters
                                + "04" // Byte count
                                + "0000" // Register 0
                                + "0000" // Register 1
                                + "FB84" // CRC
                ),
                std::make_tuple("123", "456", 0, 1, 0, 1, "1",
                                slaveAddr
                                + modbusReadRegisters
                                + "04" // Byte count
                                + "A123" // Register 0
                                + "1456" // Register 1
                                + "A68C" // CRC
                ),
                std::make_tuple("ABC", "DEF", 1, 0, 1, 0, "A",
                                slaveAddr
                                + modbusReadRegisters
                                + "04" // Byte count
                                + "5ABC" // Register 0
                                + "ADEF" // Register 1
                                + "15A4" // CRC
                )
        ));

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
                std::make_tuple(PowerSupplyParameter::CURRENT, std::stoi("0000", nullptr, 16),
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0012" // Quantity of coils.
                                + "03" // Byte count.
                                + "030000" // Coils' values.
                                + "2875" // CRC
                        , correctWriteCurrentResponse),
                std::make_tuple(PowerSupplyParameter::CURRENT, 36101,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0012" // Quantity of coils.
                                + "03" // Byte count.
                                + "173402" // Coils' values.
                                + "FF70" // CRC
                        , correctWriteCurrentResponse),
                std::make_tuple(PowerSupplyParameter::IS_ON, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "04" //Coils' values.
                                + "8E94" // CRC
                        , correctWriteOneBitValueResponse),
                std::make_tuple(PowerSupplyParameter::IS_ON, 0,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "00" //Coils' values.
                                + "8F57" // CRC
                        , correctWriteOneBitValueResponse),
                std::make_tuple(PowerSupplyParameter::POLARITY, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "05" //Coils' values.
                                + "4F54" // CRC
                        , correctWriteOneBitValueResponse),
                std::make_tuple(PowerSupplyParameter::POLARITY, 0,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "01" //Coils' values.
                                + "4E97" // CRC
                        , correctWriteOneBitValueResponse),
                std::make_tuple(PowerSupplyParameter::RESET, 1,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "06" //Coils' values.
                                + "0F55" // CRC
                        , correctWriteOneBitValueResponse),
                std::make_tuple(PowerSupplyParameter::RESET, 0,
                                slaveAddr
                                + modbusWriteCoils
                                + "0000" // Address of first coil to be written.
                                + "0003" // Quantity of coils.
                                + "01" // Byte count.
                                + "02" //Coils' values.
                                + "0E96" // CRC
                        , correctWriteOneBitValueResponse)
        ));


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}