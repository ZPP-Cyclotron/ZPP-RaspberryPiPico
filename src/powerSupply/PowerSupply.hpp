#ifndef POWERSUPPLY_HPP
#define POWERSUPPLY_HPP

#include "PowerSupplyInterface.hpp"
#include <cstdint>

class PowerSupply : public PowerSupplyInterface {

    uint16_t current = 100;
    uint16_t voltage = 100;
    bool remote = true;
    bool isOn = true;
    bool polarity = false;
    bool reset = false;
    uint8_t errors = 0;

public:

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


    int setStatus(uint8_t dataType, uint16_t value) override {

        switch (dataType) {
            case 0:
               isOn = value;
                break;
            case 1:
                polarity = value;
                break;
            case 2:
                reset = value;
                break;
            case 3:
                current = value;
                break;
            default:
                return -1;
        }

        return 1;
    }
};

#endif //POWERSUPPLY_HPP
