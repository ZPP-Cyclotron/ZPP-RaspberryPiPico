#ifndef POWERSUPPLY_HPP
#define POWERSUPPLY_HPP

#include "PowerSupplyInterface.hpp"
#include <cstdint>

class PowerSupply : public PowerSupplyInterface {

public:

    uint16_t readCurrent() override {
        return 100;
    }

    uint16_t readVoltage() override {
        return 100;
    }

    bool isRemote() override {
        return 1;
    }

    bool isPowerCircuitOn() override {
        return 0;
    }

    bool readPolarity() override {
        return 0;
    }

    bool readReset() override {
        return 0;
    }

    uint8_t readErrors() override {
        return 0;
    }

    int setCurrent(uint16_t registerIdx) override {
        return 1;
    }

    int setStatus(uint8_t dataType, bool value) override {

        switch (dataType) {
            case 0:
                setPowerCircuit(value);
                break;
            case 1:
                // polarity
                break;
            case 2:
                // reset
                break;
            default:
                return -1;
        }

        return 1;
    }

    void setPowerCircuit(bool st) override {

    }
};

#endif //POWERSUPPLY_HPP
