#ifndef PSCONST
#define PSCONST

#include <stdio.h>

// Hardware Dependent
//=====================================================================

#define PS_MAX_CURRENT 200

#define PS_MAX_VOLTAGE 100

//SPI
//=====================================================================

#define SPI_WORD_LEN 8
#define BUF_LEN_IN 6
#define BUF_LEN_OUT 4

#define SPI_CLK_F 1000000

//
////=====================================================================
#define AVRG_COMPONENT_COUNT 6

//READ (from PS)
//=====================================================================
const unsigned int psIdPos = 0;
const unsigned int psIdLen = 8;

const unsigned int psCurrentPos = psIdPos+psIdLen;
const unsigned int psCurrentLen = 12;

const unsigned int psVoltagePos = 8*3;
const unsigned int psVoltageLen = 12;

const unsigned int psStatusErrorColectivePos = 8*5;
const unsigned int psStatusErrorExternalPos = 8*5+1;
// 0-local / 1-remote
const unsigned int psStatusRemoteControlPos = 8*5+2;
// 0-OFF / 1-ON
const unsigned int psStatusPowerCircuitPos = 8*5+3;
//apparently not important
const unsigned int psStatusPolarisation1Pos = 8*5+4;
const unsigned int psStatusPolarisation2Pos = 8*5+5;
// for use in changing porarisation (to determine if it's allowed) TODO: discus how exacly it's suppoused to look like
const unsigned int psStatusIsZeroPowerPos = 8*5+6;


//WRITE (to PS)
//=====================================================================
const unsigned int psOUTCurrentPos = 0;
const unsigned int psOUTCurrentLen = 16;

//unused always 0
const unsigned int psOUTPolarizationPos = 16;
// 0-OFF / 1-ON
const unsigned int psOUTPowerCircuitPos = 16+1;
// for testing
const unsigned int psOUTPsResetPos = 16+2;
//unused always 0
const unsigned int psOUTAdditionalPos = 16+3;
//unused always 0
const unsigned int psOUTElectronicResetPos = 16+4;
const unsigned int psOUTConfirmCurrentWrite1Pos = 16+5;
const unsigned int psOUTConfirmCurrentWrite2Pos = 16+7;

const unsigned int greenLed = 24;
const unsigned int redLed = 24+1;

#endif