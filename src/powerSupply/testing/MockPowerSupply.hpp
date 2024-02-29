#ifndef ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP
#define ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP

#include "../PowerSupplyInterface.hpp"
#include <gmock/gmock.h>

class MockPowerSupply : public PowerSupplyInterface {

public:

    MOCK_METHOD(uint16_t, readCurrent, (), (override));

    MOCK_METHOD(uint16_t, readVoltage, (), (override));

    MOCK_METHOD(bool, isRemote, (), (override));

    MOCK_METHOD(bool, isPowerCircuitOn, (), (override));

    MOCK_METHOD(bool, readPolarity, (), (override));

    MOCK_METHOD(bool, readReset, (), (override));

    MOCK_METHOD(uint8_t, readErrors, (), (override));

    MOCK_METHOD(int, setCurrent, (uint16_t registerId), (override));

    MOCK_METHOD(int, setStatus, (uint8_t dataType, bool value), (override));

    MOCK_METHOD(void, setPowerCircuit, (bool st), (override));
};


#endif //ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP
