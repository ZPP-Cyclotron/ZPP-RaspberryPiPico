#ifndef ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP
#define ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP

class PowerSupplyInterface {

public:

    virtual uint16_t readCurrent() = 0;

    virtual uint16_t readVoltage() = 0;

    virtual bool isRemote() = 0;

    virtual bool isPowerCircuitOn() = 0;

    virtual bool readPolarity() = 0;

    virtual bool readReset() = 0;

    virtual uint8_t readErrors() = 0;

    virtual int setCurrent(uint16_t newCurrent) = 0;

    virtual int setStatus(uint8_t dataType, bool value) = 0;

    virtual void setPowerCircuit(bool st) = 0;

    static const int currentReadBits = 12;
    static const int voltageReadBits = 12;
};

#endif //ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP
