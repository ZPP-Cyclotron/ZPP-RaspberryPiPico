
#include "PowerSupply200AConsts.h"
#include "PowerSupplyInterface.hpp"
#include <cstdint>
#include <algorithm>
extern "C" {
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/spi.h"
}



static_assert(SPI_WORD_LEN == 8);

// do delate after tests =======================
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c - %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')
// ========================================

class PowerSupply : public PowerSupplyInterface {

    uint8_t psBufOut[BUF_LEN_OUT];
    uint8_t psBufIn[BUF_LEN_IN];

    spi_inst_t* PS_spi = spi_default;
    const uint PS_gpio = 22;

    // because it isn't possible to read that from psBufOut.
    // its avrged value of current_read
    float avrgCurrent = 0;
    uint16_t currentSet = 0;

public:
    PowerSupply(){
        printf("PS 200A interface inint \n");
        spi_init(PS_spi, 1 * SPI_CLK_F);

        for (int i = 0; i < BUF_LEN_OUT; i++)
        {
            psBufOut[i] = 0;
        }
        for (int i = 0; i < BUF_LEN_IN; i++)
        {
            psBufIn[i] = 0;
        }

        // Assign SPI functions to the default SPI pins
        gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
        gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
        gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
        gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

        gpio_init(PS_gpio);
        gpio_set_dir(PS_gpio, GPIO_OUT);
    }

    void safeCommunicationWithPS (){
        
        // second Communication is to set write protection on,
        // so thre won't be any unexpected change in set current.
        avrgCurrent =0;
        avrgCurrent += freadCurrent();
        if(!isRemote()){
            comunicateWithPS();
            setPowerCircuit(isPowerCircuitOn());
            comunicateWithPS();
        }
        else{
            comunicateWithPS();
        }
        avrgCurrent += freadCurrent();

        for (int i=2;i< AVRG_COMPONENT_COUNT;i++){
            comunicateWithPS();
            avrgCurrent += freadCurrent();
        }
        avrgCurrent /= std::max(2, AVRG_COMPONENT_COUNT);

        // TO delete===
        printStat();
        printBuffers();
        // /=====
    }

    // ================================
    // for now comunnication with PS is only whet sending data
    // TDO: make sure that's Ok
    uint16_t readCurrent() {
        uint16_t val = avrgCurrent*maxPossibleValue(psOUTCurrentLen)/PS_MAX_CURRENT;
        return val;
    }

    uint16_t getLastSetCurrent() override {
        return readCurrent();
    }


    uint16_t readVoltage() {
        return readUint(psBufIn, psVoltagePos, psVoltageLen) ;
    }

    bool isRemote() {
        return getBit(psBufIn,psStatusRemoteControlPos);
    }

    bool isPowerCircuitOn() {
        return getBit(psBufIn,psStatusPowerCircuitPos);
    }

    bool readPolarity() {
        return getBit(psBufIn,psStatusPolarisation1Pos) && getBit(psBufIn,psStatusPolarisation2Pos);
    }

    bool readReset() override {
        return 0;
    }

    // 2'st bit - External Error (cooling)
    // 1'nd bit - colective Error
    uint8_t readErrors() {
        uint8_t err =0;
        err =  getBit(psBufIn,psStatusErrorExternalPos);
        err = err<<1;
        err += getBit(psBufIn,psStatusErrorColectivePos);
        return err;
    }

    int setCurrent(uint16_t val){
        setCurrentWrite(1);

        setUint(psBufOut,val,psOUTCurrentPos,psOUTCurrentLen,true);
        return 1;
    }
    int setCurrent(float crr){
        if( crr >PS_MAX_CURRENT || crr <0 ){
            return -1;
        }

        uint16_t val = crr*maxPossibleValue(psOUTCurrentLen)/PS_MAX_CURRENT;
        setCurrent(val);
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
            case 3:
                currentSet = value;
                setCurrent(value);
                break;
            default:
                return -1;
        }

        return 1;
    }

    void setPowerCircuit(bool st) {
        setCurrentWrite(1);
        setBit(psBufOut, psOUTPowerCircuitPos, st);
    }

    //TODO: how long it should be 'high'
    void setReset(bool state){
        setCurrentWrite(1);
        setBit(psBufOut, psOUTPsResetPos, state);
    }

    void setElectronicReset(bool state){
        setCurrentWrite(1);
        setBit(psBufOut, psOUTElectronicResetPos, state);
    }

    void setLedGreen(bool state){
        setBit(psBufOut, greenLed, state);
    }
    void setLedRed(bool state){
        setBit(psBufOut, redLed, state);
    }

    void printBuffers(){
        int in_i=0;
        for(int wordNr=0;std::max(BUF_LEN_OUT, BUF_LEN_IN)>wordNr;wordNr++ ){
            in_i = wordNr;
            printf("in %d -> " BYTE_TO_BINARY_PATTERN "\n",in_i,BYTE_TO_BINARY(psBufIn[in_i]));
        }

        int out_i=0;
        in_i=0;
        for(int wordNr=0;std::max(BUF_LEN_OUT, BUF_LEN_IN)>wordNr;wordNr++ ){
            in_i = wordNr;
            out_i = std::max(0,wordNr - (BUF_LEN_IN- BUF_LEN_OUT) );
            printf("out %d -> " BYTE_TO_BINARY_PATTERN "\n",out_i,BYTE_TO_BINARY(psBufOut[out_i]));
        }
    }

    bool getIsOnSet() override {
        return getBit(psBufOut, psOUTPowerCircuitPos);
    }

    bool getPolaritySet() override {
        // This power supply doesn't support setting polarity parameter.
        return false;
    }

private:
    // do delate after tests =======================
    uint16_t readCurrentRaw() {
        return readUint(psBufIn, psCurrentPos, psCurrentLen) ;
    }
    float freadCurrent(){
        return (1.0*PS_MAX_CURRENT*readCurrentRaw()) / maxPossibleValue(psCurrentLen);
    }

    float freadVoltage(){
        return (1.0*PS_MAX_VOLTAGE*readVoltage())/ maxPossibleValue(psVoltageLen);
    }

    uint16_t currentConvert(float crr){
    uint16_t val = crr*maxPossibleValue(psOUTCurrentLen)/PS_MAX_CURRENT;
    return val;
    }

    void printStat(){
    printf("Current: %f\n", freadCurrent());
    printf("Current: %x\n", readCurrent());

    printf("Voltage: %f\n", freadVoltage());
    printf("Voltage: %x\n", readVoltage());

    printf("Is Remote: %s\n", isRemote()?"true":"false");
    printf("Is Power Circuit On: %s\n", isPowerCircuitOn()?"true":"false");
    printf("Polarity: %s\n", readPolarity()?"true":"false");
    printf("Err 1-colective  2-External: %d\n", readErrors());
    }
    // =================

    void comunicateWithPS(){

        int out_i=0;
        int in_i=0;

        gpio_put(PS_gpio,1);
        for(int wordNr=0;std::max(BUF_LEN_OUT, BUF_LEN_IN)>wordNr;wordNr++ ){
            
            in_i = wordNr;
            out_i = std::max(0,wordNr - (BUF_LEN_IN- BUF_LEN_OUT) );

            spi_write_read_blocking(PS_spi, &psBufOut[out_i], &psBufIn[in_i], 1);
        }
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
