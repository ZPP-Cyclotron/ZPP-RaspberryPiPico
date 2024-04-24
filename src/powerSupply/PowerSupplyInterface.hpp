#ifndef ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP
#define ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP

enum class PowerSupplyParameter { IS_ON = 0, POLARITY = 1, RESET = 2, CURRENT = 3 };

class PowerSupplyInterface {

public:
    virtual ~PowerSupplyInterface() = default;

    virtual void safeCommunicationWithPS ()=0;

    virtual uint16_t readCurrent() = 0;

    virtual uint16_t readVoltage() = 0;

    virtual bool isRemote() = 0;

    virtual bool isPowerCircuitOn() = 0;

    virtual bool readPolarity() = 0;

    virtual bool readReset() = 0;

    virtual uint8_t readErrors() = 0;

    virtual int setStatus(uint8_t dataType, uint16_t value) = 0;

    virtual uint16_t getLastSetCurrent() = 0;

    virtual bool getIsOnSet() = 0;

    virtual bool getPolaritySet() = 0;

    static const int currentReadBits = 12;
    static const int voltageReadBits = 12;
    static const int errorReadBits = 2;

};

#endif //ZPP_CYCLOTRON_POWERSUPPLYINTERFACE_HPP
