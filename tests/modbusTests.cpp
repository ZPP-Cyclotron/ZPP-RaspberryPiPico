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
                                       + "0003" // Number of registers to be read.
                                       + "B00B"; // CRC

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
        : public testing::TestWithParam<std::tuple<std::string, std::string, bool, bool, bool, bool, std::string, std::string, std::string>> {
};

TEST_P(ModbusServerReadTest, RespondsToCorrectReadRequest) {

    auto [currentHex, voltageHex, isOn, polarity, reset, remote, errorsHex, currentSetHex, response] = ModbusServerReadTest::GetParam();

    uint16_t current = std::stoi(currentHex, nullptr, 16);
    uint16_t voltage = std::stoi(voltageHex, nullptr, 16);
    uint8_t errors = std::stoi(errorsHex, nullptr, 16);
    uint16_t currentSet = std::stoi(currentSetHex, nullptr, 16);

    auto mockPowerSupply = std::make_unique<MockPowerSupply>(current, voltage, isOn,
                                                             polarity, reset, remote,
                                                             errors, currentSet);
    std::string request = correctReadRequest;

    auto mockPicoController = std::make_shared<MockPicoController>(request, response);

    std::unique_ptr<ModbusServer> modbusServer(new ModbusServer(std::move(mockPowerSupply), mockPicoController));

    modbusServer->waitAndHandleRequest();
}

INSTANTIATE_TEST_SUITE_P(
        ModbusServerTest, ModbusServerReadTest,
        testing::Values(
                std::make_tuple("FFF", "FFF", 1, 1, 1, 1, "F", "AAAA",
                                slaveAddr
                                + modbusReadRegisters
                                + "06" // Byte count
                                + "FFFF" // Register 0
                                + "FFFF" // Register 1
                                + "AAAA" // Register 2
                                + "9E73" // CRC
                ),
                std::make_tuple("0", "0", 0, 0, 0, 0, "0", "0000",
                                slaveAddr
                                + modbusReadRegisters
                                + "06" // Byte count
                                + "0000" // Register 0
                                + "0000" // Register 1
                                + "0000" // Register 2
                                + "6093" // CRC
                ),
                std::make_tuple("123", "456", 0, 1, 0, 1, "1", "BCDE",
                                slaveAddr
                                + modbusReadRegisters
                                + "06" // Byte count
                                + "A123" // Register 0
                                + "1456" // Register 1
                                + "BCDE" // Register 2
                                + "A95D" // CRC
                ),
                std::make_tuple("ABC", "DEF", 1, 0, 1, 0, "A", "1918",
                                slaveAddr
                                + modbusReadRegisters
                                + "06" // Byte count
                                + "5ABC" // Register 0
                                + "ADEF" // Register 1
                                + "1918" // Register 2
                                + "2771" // CRC
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
                std::make_tuple(PowerSupplyParameter::CURRENT, 41137,
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


class ModbusServerIncorrectRequestTest
        : public testing::TestWithParam<std::tuple<std::string, std::string>> {
};

TEST_P(ModbusServerIncorrectRequestTest, RespondsWithErrorCode) {
    auto [request, response] = ModbusServerIncorrectRequestTest::GetParam();

    auto mockPowerSupply = std::make_unique<MockPowerSupply>(-1, 0);

    auto mockPicoController = std::make_shared<MockPicoController>(request, response);

    std::unique_ptr<ModbusServer> modbusServer(new ModbusServer(std::move(mockPowerSupply), mockPicoController));

    modbusServer->waitAndHandleRequest();
}

INSTANTIATE_TEST_SUITE_P(
        ModbusServerTest, ModbusServerIncorrectRequestTest,
        testing::Values(
                std::make_tuple(
                        "AB" // Random byte.
                        , ""
                ),
                std::make_tuple(
                        "010200000016F9C4" // Modbus function that is not handled.
                        , "0182018160"
                ),
                std::make_tuple(
                        slaveAddr
                        + modbusWriteCoils
                        + "0001" // Invalid address of first coil to be written.
                        + "0012" // Quantity of coils.
                        + "03" // Byte count.
                        + "FFFF03" // Coils' values.
                        + "F974" // CRC
                        , "018F02C5F1"
                ),
                std::make_tuple(
                        slaveAddr
                        + modbusWriteCoils
                        + "0000" // Address of first coil to be written.
                        + "0002" // Invalid quantity of coils.
                        + "01" // Byte count.
                        + "00" // Coils' values.
                        + "DE97" // CRC
                        , "018F02C5F1"
                ),
                std::make_tuple(
                        slaveAddr
                        + modbusWriteCoils
                        + "0000" // Address of first coil to be written.
                        + "0012" // Quantity of coils.
                        + "01" // Invalid byte count.
                        + "FFFF03" // Coils' values.
                        + "E80C" // CRC
                        , "" // Because CRC is invalid then.
                ),
                std::make_tuple(
                        slaveAddr
                        + modbusReadRegisters
                        + "0001" // Invalid address of first register to be read.
                        + "0003" // Number of registers to be read.
                        + "E1CB" // CRC
                        , "018402C2C1"
                ),
                std::make_tuple(
                        slaveAddr
                        + modbusReadRegisters
                        + "0001" // Address of first register to be read.
                        + "0000" // Invalid number of registers to be read.
                        + "A1CA" // CRC
                        , "0184030301"
                )
        ));

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}