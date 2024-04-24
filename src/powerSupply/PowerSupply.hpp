#ifndef POWERSUPPLY_HPP
#define POWERSUPPLY_HPP

#include "PowerSupplyInterface.hpp"
#include "../../config.h"
#include <cstdint>
#include <cstring>

class PowerSupply : public PowerSupplyInterface {

    const bool PRINT_PROMPTS = !strcmp(LINUX_VERSION, "ON");

    uint16_t current = 100;
    uint16_t voltage = 100;
    bool remote = true;
    bool isOn = true;
    bool polarity = false;
    bool reset = false;
    uint8_t errors = 0;

    uint16_t currentSet = 0;
    bool isOnSet = false;
    bool polaritySet = false;

public:
    // PowerSupply (){
    //     printf("mock PS interface\n");
    // }

    void safeCommunicationWithPS () override{
        return;
    }

    uint16_t readCurrent() override {
        if (PRINT_PROMPTS)
            printf("Read power supply parameters:\n");
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

        if (ILLEGAL_DATA_VALUE_LINUX_TEST)
            return -1;

        switch (dataType) {
            case 0:
                isOnSet = value;
                if (PRINT_PROMPTS)
                    printf("New on/off value: %d\n", value);
                isOn = value;
                break;
            case 1:
                if (PRINT_PROMPTS)
                    printf("New polarity value: %d\n", value);
                polarity = value;
                break;
            case 2:
                if (PRINT_PROMPTS)
                    printf("New reset value: %d\n", value);
                reset = value;
                break;
            case 3:
                if (PRINT_PROMPTS)
                    printf("New current value: %d\n", value);
                // Converting current unit from 200/(2^16-1) A to 200/(2^12-1) A.
                currentSet = value;
                current = value * ((2 << 11) - 1) / UINT16_MAX;
                break;
            default:
                return -1;
        }

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

#endif //POWERSUPPLY_HPP
