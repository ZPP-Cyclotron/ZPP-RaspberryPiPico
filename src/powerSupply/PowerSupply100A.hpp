
#include "PowerSupply100AConsts.hpp"
#include "PowerSupplyInterface.hpp"
#include <cstdint>
#include <algorithm>
extern "C" {
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/stdio.h"
}


int reg_read(  i2c_inst_t *i2c,
                const uint addr,
                const uint8_t reg,
                uint8_t *buf,
                const uint8_t nbytes) {

    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Read data from register(s) over I2C
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);

    return num_bytes_read;
}


static_assert(IC2_WORD_LEN == 8);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

class PowerSupply100A : public PowerSupplyInterface {

    uint8_t psBufOut[BUF_LEN_OUT];
    uint8_t psBufIn[BUF_LEN_IN];

    spi_inst_t* PS_spi = spi_default;
    const uint PS_gpio = 22;

public:
    PowerSupply100A(){
        i2c_init(PCF_I2C, I2C_CLK_F);

        // Initialize I2C pins
        gpio_set_function(I2C_SDA_pin, GPIO_FUNC_I2C);
        gpio_set_function(I2C_SCL_pin, GPIO_FUNC_I2C);

        // TODO : Deine hardware GPIO pins for (input select pin and output select pin)
        // gpio_init(PS_gpio);
        // gpio_set_dir(PS_gpio, GPIO_OUT);
    }

    void safeCommunicationWithPS (){
        comunicateWithPS();
    }

    // ================================
    
    // returned value is 12bit
    uint16_t readCurrent() {
        unsigned int crr_read = 0;
        uint16_t currentValue =0;
        uint8_t address = 0;
        uint16_t buf = 0;

        while (crr_read <  psCurrentLen){
            buf =0;
            address = getPCFaddress(psCurrentPos+ crr_read);
            i2c_read_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false )

            buf  =  buf % (psCurrentLen - crr_read);
            currentValue += (buf<<crr_read);
            crr_read += IC2_WORD_LEN;
        }
        return currentValue;
    }

    uint16_t readVoltage() {
        unsigned int crr_read = 0;
        uint16_t voltageValue =0;
        uint8_t address = 0;
        uint16_t buf = 0;
        while (crr_read <  psVoltageLen){
            buf =0;
            address = getPCFaddress(psVoltagePos+ crr_read);
            i2c_read_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false )

            buf  =  buf % (psVoltageLen - crr_read);
            voltageValue += (buf<<crr_read);
            crr_read += IC2_WORD_LEN;
        }
        return voltageValue;
    }

    bool isRemote() {
        return getOneBit(psStatusRemoteControlPos);
    }

    // bool isPowerCircuitOn() {
    //     uint8_t address = getPCFaddress(ps);
    //     uint8_t buf = 0;
    //     i2c_read_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false)

    //     return getBit(&buf, psStatusRemoteControlPos%8 );
    // }

    // bool readPolarity() {
    //     return getOneBit(ps)
    // }

    bool readReset() override {
        return 0;
    }

    // 2'st bit - External Error (cooling)
    // 1'nd bit - colective Error
    uint8_t readErrors() {
        uint8_t err =0;
        err =  getOneBit(psStatusErrorExternalPos);
        err = err<<1;
        err += getOneBit(psStatusErrorColectivePos);
        return err;
    }

    // NOTE: Currently impementation of setting values might make unwanted changes 
    // (not all bits in send byte might be set correctly)

    // Current implementation of setting values stil need's implementation of enabling input 
    void enableInput(){

    }
    void disableInput(){

    }

    // val is 12 bit number
    int setCurrent(uint16_t val){
        enableInput();
        unsigned int crr_read = 0;
        uint8_t buf = 0;
        uint8_t address = 0;
        while (crr_read <  psCurrentLen){
            buf = (val>>crr_read) % std::max(IC2_WORD_LEN, psCurrentLen - crr_read);
            address = getPCFaddress(psCurrentPos+ crr_read);

            if( i2c_write_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false ) != IC2_WORD_LEN/8){
                disableInput();
                return 0;
            }
            crr_read += IC2_WORD_LEN;
        }
        disableInput();
        return 1;
    }

    int setStatus(uint8_t dataType, uint16_t value) override {

        switch (dataType) {
            case 0:
                setPowerCircuit(value);
                break;
            // case 1:
            //     polarity = 
            //     break;
            // case 2:
            //     reset = value;
            //     break;
            default:
                return -1;
        }

        return 1;
    }

    void setPowerCircuit(bool st) {
        enableInput();
        unsigned int pos = psOUTPwr % IC2_WORD_LEN;
        u_int8_t buf =0;
        setBit(&buf, pos, st);
        u_int8_t address = getPCFaddress(psOUTPwr);
        i2c_write_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false );
        disableInput();
    }

    void setReset(bool state){
        enableInput();
        unsigned int pos = psOUTPsResetPos % IC2_WORD_LEN;
        u_int8_t buf =0;
        setBit(&buf, pos, st);
        u_int8_t address = getPCFaddress(psOUTPsResetPos);
        i2c_write_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false );
        disableInput();
    }

private:

    uint8_t getPCFaddress(unsigned int pinPos){
        unsigned int byteNr = pinPos/ 8;
        unsigned int hardwareAddress =  hdAddresses[byteNr];
        return PCFconstAddres + hardwareAddress;
    }

    bool getOneBit(unsigned int pinPos){
        uint8_t address = getPCFaddress(pinPos);
        uint8_t buf = 0;
        i2c_read_blocking(PCF_I2C,address, &buf, IC2_WORD_LEN/8, false)

        return getBit(&buf, pinPos%8 );
    }

    void comunicateWithPS(){

        int out_i=0;
        int in_i=0;

        gpio_put(PS_gpio,1);
        for(int wordNr=0;std::max(BUF_LEN_OUT, BUF_LEN_IN)>wordNr;wordNr++ ){
            
            in_i = wordNr;
            out_i = std::max(0,wordNr - (BUF_LEN_IN- BUF_LEN_OUT) );

            // TODO: check if correct number of bytes has been written
            spi_write_read_blocking(PS_spi, &psBufOut[out_i], &psBufIn[in_i], 1);
            // spi_write_blocking(PS_spi, &psBufOut[out_i], 1);
            // printf("out %d -> "BYTE_TO_BINARY_PATTERN"\n",out_i,BYTE_TO_BINARY(psBufOut[out_i]));
            // printf("in %d -> "BYTE_TO_BINARY_PATTERN"\n",in_i,BYTE_TO_BINARY(psBufIn[in_i]));
        }
        // spi_write_blocking(PS_spi, psBufOut, 4);
        //DELAY??
        gpio_put(PS_gpio,0);

        setCurrentWrite(0);
    }

    bool getBit(uint8_t *buf, int pos)
    {
    int i = 0;
    while (pos > 7)
    {
        pos -=8;
        i++;
    }

    return buf[i]& (1<<pos);
    }
    void setBit(uint8_t *buf, int pos, bool value)
    {
    int i = 0;
    while (pos > 7)
    {
        pos -=8;
        i++;
    }

    bool crrVal = buf[i]& (1<<pos);
    if (crrVal != value) buf[i] = buf[i] ^(1<<pos);

    }

    void revarseBits(uint8_t* buf, int pos, int len){
        bool swp; 
        for(int i=0;len/2>i;i++){
            swp = getBit(buf, pos+i);
            setBit(buf, pos+i, getBit(buf, pos+len-1-i));
            setBit(buf, pos+len-1-i, swp);
        }
    }

    uint16_t readUint(uint8_t* buf, int pos, int len){
        // if(revarse) revarseBits(buf, pos, len);  revarseBits permanently reverses order so it can't be used in read

        uint16_t val=0;
        for(int i=0;len>i;i++){
            val = val<<1;
            val += getBit(buf, pos+len-1-i);
        }
        return val;
    }
    void setUint(uint8_t* buf, uint16_t val, int pos, int len, bool revarse){

        unsigned int crr=1;
        for(int i=0;len>i;i++){
            if((val & crr) != 0 ){
                setBit(buf,pos+i, 1);
            } else{
                setBit(buf,pos+i, 0);
            }
            crr = crr<<1;
        }

        if(revarse){
            revarseBits(buf, pos, 8);
            revarseBits(buf, pos+8, 8);
        }
    }

    float maxPossibleValue(int len){
        return (1<<len)-1;
    }

    //=====================================================================
    //st=1->Enable / st=0->Disable
    void setCurrentWrite(bool st){
        setBit(psBufOut, psOUTConfirmCurrentWrite1Pos, st);
        setBit(psBufOut, psOUTConfirmCurrentWrite2Pos, st);
    }
};
