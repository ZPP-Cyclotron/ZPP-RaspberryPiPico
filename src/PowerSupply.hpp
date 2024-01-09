#ifndef POWERSUPPLY_HPP
#define POWERSUPPLY_HPP

class PowerSupply {

public:

    uint16_t readCurrent() {
        return 100;
    }

    uint16_t readVoltage() {
        return 100;
    }

    bool isRemote() {
        return 1;
    }

    bool isPowerCircuitOn() {
        return 0;
    }

    bool readPolarity() {
        return 0;
    }

    bool readReset() {
        return 0;
    }

    uint8_t readErrors() {
        return 0;
    }

    int setCurrent(uint16_t register) {
        return 1;
    }

    int setStatus(uint8_t dataType, bool value) {

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

    void setPowerCircuit(bool st) {

    }

    static const int currentReadBits = 12;
    static const int voltageReadBits = 12;
};

#endif //POWERSUPPLY_HPP
