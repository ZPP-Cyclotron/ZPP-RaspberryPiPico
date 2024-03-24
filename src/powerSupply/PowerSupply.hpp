#ifndef POWERSUPPLY_HPP
#define POWERSUPPLY_HPP

#include "PowerSupplyInterface.hpp"
#include "../../config.h"
#include <cstdint>

class PowerSupply : public PowerSupplyInterface {

    const bool PRINT_READ_PARAMETERS = false;

    uint16_t current = 100;
    uint16_t voltage = 100;
    bool remote = true;
    bool isOn = true;
    bool polarity = false;
    bool reset = false;
    uint8_t errors = 0;

public:
    void safeCommunicationWithPS () override{
        return;
    }

    uint16_t readCurrent() override {
        printf("Read power supply parameters:\n");
        if (PRINT_READ_PARAMETERS)
            printf("(1) current, ");
        return current;
    }

    uint16_t readVoltage() override {
        if (PRINT_READ_PARAMETERS)
            printf("(6) voltage, ");
        return voltage;
    }

    bool isRemote() override {
        if (PRINT_READ_PARAMETERS)
            printf("(5) remote, ");
        return remote;
    }

    bool isPowerCircuitOn() override {
        if (PRINT_READ_PARAMETERS)
            printf("(2) on/off, ");
        return isOn;
    }

    bool readPolarity() override {
        if (PRINT_READ_PARAMETERS)
            printf("(3) polarity, ");
        return polarity;
    }

    bool readReset() override {
        if (PRINT_READ_PARAMETERS)
            printf("(4) reset, ");
        return reset;
    }

    uint8_t readErrors() override {
        if (PRINT_READ_PARAMETERS)
            printf("(7) errors\n");
        return errors;
    }


    int setStatus(uint8_t dataType, uint16_t value) override {

    if (ILLEGAL_DATA_VALUE_LINUX_TEST)
            return -1;

        switch (dataType) {
            case 0:
                printf("New on/off value: %d\n", value);
                isOn = value;
                break;
            case 1:
                printf("New polarity value: %d\n", value);
                polarity = value;
                break;
            case 2:
                printf("New reset value: %d\n", value);
                reset = value;
                break;
            case 3:
                printf("New current value: %d\n", value);
                // Converting current unit from 200/(2^16-1) A to 200/(2^12-1) A.
                current = value * ((2 << 11) - 1) / UINT16_MAX;
                break;
            default:
                return -1;
        }

        return 1;
    }
};

#endif //POWERSUPPLY_HPP
