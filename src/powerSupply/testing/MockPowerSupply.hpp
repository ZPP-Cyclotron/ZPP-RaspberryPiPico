#ifndef ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP
#define ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP

#include "../PowerSupplyInterface.hpp"
#include <gmock/gmock.h>

class MockPowerSupply : public PowerSupplyInterface {

private:

    uint16_t current = 0;
    uint16_t voltage = 0;
    bool isOn = 0;
    bool polarity = 0;
    bool reset = 0;
    bool remote = 0;
    uint8_t errors = 0;
    uint16_t currentSet = 0;
    bool isOnSet = false;
    bool polaritySet = false;

    int toBeChanged = -1;
    uint16_t newValue = 0;

public:

    MockPowerSupply(uint16_t curr, uint16_t volt, bool on, bool pol, bool rst, bool rem, uint8_t err, bool polaritySet, bool isOnSet,
                    uint16_t setCurrent)
            : current(curr), voltage(volt), isOn(on), polarity(pol), reset(rst), remote(rem),
              errors(err) {
        this->currentSet = setCurrent;
        this->isOnSet = isOnSet;
        this->polaritySet = polaritySet;
    }

    MockPowerSupply(int dataType, uint16_t value) : toBeChanged(dataType), newValue(value) {}

    void safeCommunicationWithPS() override {
        return;
    }

    void checkToBeChanged() const {
        ASSERT_TRUE(toBeChanged < 0);
    }

    ~MockPowerSupply() override {
        std::cout << "Mock power supply destructor\n";
        checkToBeChanged();
    }

    uint16_t readCurrent() override {
        return current;
    }

    uint16_t readVoltage() override {
        return voltage;
    }

    bool isRemote() override {
        return remote;
    }

    bool isPowerCircuitOn() override {
        return isOn;
    }

    bool readPolarity() override {
        return polarity;
    }

    bool readReset() override {
        return reset;
    }

    uint8_t readErrors() override {
        return errors;
    }

    void checkSetStatus(uint8_t dataType, uint16_t value) const {
        ASSERT_TRUE(toBeChanged >= 0 && dataType == toBeChanged);
        ASSERT_TRUE(value == newValue);
    }

    int setStatus(uint8_t dataType, uint16_t value) override {

        checkSetStatus(dataType, value);
        toBeChanged = -1;

        return 1;
    }

    uint16_t getLastSetCurrent() override {
        return currentSet;
    }

    bool getIsOnSet() override {
        return isOnSet;
    }

    bool getPolaritySet() override {
        return polaritySet;
    }
};


#endif //ZPP_CYCLOTRON_MOCKPOWERSUPPLY_HPP
