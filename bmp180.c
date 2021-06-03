#include "bmp180.h"

int AC1;
int AC2;
int AC3;
int AC3;
unsigned int AC4;
unsigned int AC5;
unsigned int AC6;
int B1;
int B2;
int MB;
int MC;
int MD;
uint32_t B5;

void bmpStartTempConversion(){
    i2cStartCondition();
    i2cSend(BMP085_W);
    i2cSend(BMP085_CR);
    i2cSend(BMP085_UT);
    i2cStopCondition();
    _delay_ms(5);
// A conversão AD dura pelo menos 4.5 ms ento configurar delay.
}

void bmpStartPressureConversion(){
    i2cStartCondition();
    i2cSend(BMP085_W);
    i2cSend(BMP085_CR);
    i2cSend(BMP085_UP+(BMP_OSS<<6));
    i2cStopCondition();
    _delay_ms(2+(3<<BMP_OSS));
}

int bmpReadInt(uint8_t address){
    int data;
    i2cStartCondition();
    i2cSend(BMP085_W);
    i2cSend(address);
    i2cStartCondition();
    i2cSend(BMP085_R);
    data=(i2cReadACK()<<8)|i2cReadNACK();
    i2cStopCondition();
    return data;
}

void bmpReadCallibrationData(){
  AC1 = bmpReadInt(0xAA);
  AC2 = bmpReadInt(0xAC);
  AC3 = bmpReadInt(0xAE);
  AC4 = bmpReadInt(0xB0);
  AC5 = bmpReadInt(0xB2);
  AC6 = bmpReadInt(0xB4);
  B1 = bmpReadInt(0xB6);
  B2 = bmpReadInt(0xB8);
  MB = bmpReadInt(0xBA);
  MC = bmpReadInt(0xBC);
  MD = bmpReadInt(0xBE);
}

int bmpReadDataTemperature(){
    int uncalibratedData;
    i2cStartCondition();
    i2cSend(BMP085_W);
    i2cSend(BMP085_DR);
    i2cStartCondition();
    i2cSend(BMP085_R);
    uncalibratedData=(i2cReadACK()<<8)|i2cReadNACK();
    i2cStopCondition();
    return uncalibratedData;
}

uint32_t bmpReadDataPressure(){
    unsigned long uncalibratedData;
    i2cStartCondition();
    i2cSend(BMP085_W);
    i2cSend(BMP085_DR);
    i2cStartCondition();
    i2cSend(BMP085_R);
    uncalibratedData=(((unsigned long)i2cReadACK())<<16);
    uncalibratedData|=((unsigned long)i2cReadACK()<<8);
    uncalibratedData|=i2cReadNACK();
    uncalibratedData=(uncalibratedData>>(8-BMP_OSS));
    i2cStopCondition();
    return uncalibratedData;
}

uint16_t bmpTrueTemperature(uint16_t data){
  long X1, X2;
  X1=(((long)data-(long)AC6)*(long)AC5)>>15;
  X2=((long)MC<<11)/(X1+MD);
  B5=X1+X2;
  return ((B5+8)>>4);
}

uint32_t bmpTruePressure(unsigned long data){
  long X1, X2, X3, B3, B6, P;
  unsigned long B4, B7;

  B6=B5-4000;

  X1=(B2*(B6*B6)>>12)>>11;
  X2=(AC2*B6)>>11;
  X3=X1+X2;
  B3=(((((long)AC1)*4+X3)<<BMP_OSS)+2)>>2;

  X1=(AC3*B6)>>13;
  X2=(B1*((B6*B6)>>12))>>16;
  X3=((X1+X2)+2)>>2;
  B4=(AC4*(unsigned long)(X3+32768))>>15;
  B7=((unsigned long)(data-B3)*(50000>>BMP_OSS));

  if (B7<0x80000000){
    P=(B7<<1)/B4;
    }else{
    P=(B7/B4)<<1;
    }
  X1=(P>>8)*(P>>8);
  X1=(X1*3038)>>16;
  X2=(-7357*P)>>16;
  P+=(X1+X2+3791)>>4;

  return P;
}

void bmpData(BMP *p){
    bmpReadCallibrationData();
    bmpStartTempConversion();
    p->temperature=bmpTrueTemperature(bmpReadDataTemperature());
    bmpStartPressureConversion();
    p->pressure=bmpTruePressure(bmpReadDataPressure());
}
