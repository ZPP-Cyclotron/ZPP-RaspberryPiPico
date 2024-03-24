#ifndef PS1000CONST
#define PS100CONST

#include <stdio.h>
#include <stdint.h>

extern "C" {
#include "hardware/i2c.h"
}
// Hardware Dependent
//=====================================================================

#define PS_MAX_CURRENT 100

#define PS_MAX_VOLTAGE 100

//SPI
//=====================================================================

#define I2C_WORD_LEN 8
#define BUF_LEN_IN 6
#define BUF_LEN_OUT 4

#define I2C_SDA_pin 16
#define I2C_SCL_pin 17
#define I2C_CLK_F 400 * 1000

const i2c_inst_t * PCF_I2C = i2c0;
// PCF8574 constant address https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf
//  7. Functional description
const uint8_t PCFconstAddres = (1<<5);
const unsigned int PCFHardwareDefinedAddressLen = 3;

// In case hardware defined addresses arent in order
const uint8_t hdAddresses[8] = {0,1,2,3,4,5,6,7};


// COMMON
//=====================================================================
const unsigned int psCurrentPos = 0;
const unsigned int psCurrentLen = 12;

//READ (from PS)
//=====================================================================
const unsigned int psVoltagePos = 16;
const unsigned int psVoltageLen = 12;

const unsigned int psStatusErrorColectivePos = 12;
const unsigned int psStatusErrorExternalPos = 13;
const unsigned int psStatusRemoteControlPos = 14;
// const unsigned int psStatusPowerCircuitPos = 8*5+3;
// //apparently not important
// const unsigned int psStatusPolarisation1Pos = 8*5+4;
// // for use in changing porarisation (to determine if it's allowed) TODO: discus how exacly it's suppoused to look like
// const unsigned int psStatusIsZeroPowerPos = 8*5+6;


//WRITE (to PS)
//=====================================================================
const unsigned int psOUTPolarizationPos = 13;
const unsigned int psOUTPwr = 12;
const unsigned int psOUTPsResetPos = 14;
// //unused always 0
// const unsigned int psOUTAdditionalPos = 16+3;
// //unused always 0
// const unsigned int psOUTElectronicResetPos = 16+4;
// const unsigned int psOUTConfirmCurrentWrite1Pos = 16+5;
// const unsigned int psOUTConfirmCurrentWrite2Pos = 16+7;

#endif