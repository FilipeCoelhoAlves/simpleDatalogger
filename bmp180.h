#ifndef BMP085_H_INCLUDED
#define BMP085_H_INCLUDED

#include "core328p/hal.h"


#define BMP085_W 0xEE
#define BMP085_R 0xEF
#define BMP085_UT 0x2E
#define BMP085_UP 0x34
#define BMP085_CR 0xF4
#define BMP085_DR 0xF6 //F7
#define BMP_OSS 0      // Oversampling configuration:0,1,2,3,4

typedef struct{
    uint32_t pressure;
    uint16_t    temperature;
} BMP;

void          bmpStartTempConversion();
void          bmpStartPressureConversion();
int           bmpReadInt(uint8_t address);
void          bmpReadCallibrationData();
int           bmpReadDataTemperature();
uint32_t      bmpReadDataPressure();
uint16_t      bmpTrueTemperature(uint16_t data);
uint32_t      bmpTruePressure(unsigned long data);
void          bmpData(BMP *p);


#endif // BMP085_H_INCLUDED
