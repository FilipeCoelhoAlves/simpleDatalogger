#ifndef DS3231_H_INCLUDED
#define DS3231_H_INCLUDED
//Lib para manipulação do rtc maxim ds3231
//Filipe Emmanuel Coelho Alves

#include "core328p/i2c.h"

#define DS3231_ADDR_W 0xD0
#define DS3231_ADDR_R 0xD1

//REGISTRADORES DD/MM/YY HH:MM:SS
#define RTC_SECONDS   0x00
#define RTC_MINUTES   0x01
#define RTC_HOUR      0x02
#define RTC_WEEK_DAY  0x03
#define RTC_MONTH_DAY 0x04
#define RTC_MONTH     0x05
#define RTC_YEAR      0x06

//REGISTRADORES DO ALARME

#define RTC_ALARM_1_SECONDS 0x07
#define RTC_ALARM_1_MINUTES 0x08
#define RTC_ALARM_1_HOURS   0x09
#define RTC_ALARM_1_DAY     0x0A

#define RTC_ALARM_2_MINUTES 0x0B
#define RTC_ALARM_2_HOURS   0x0C
#define RTC_ALARM_2_DAY     0x0D

//REGISTRADORES DE CONTROLE

#define RTC_CONTROL_1     0x0E
#define RTC_CONTROL_2     0x0F
#define RTC_AGING_OFFSET  0x10
#define RTC_TEMP_MSB      0x11
#define RTC_TEMP_LSB      0x12

//BITS RTC CONTROL_1

#define EOSC  7 // ENABLE OSCILATOR 0==ON
#define BBSQW 6 // Battery-Backed Square-Wave Enable
#define CONV  5 // Convert Temperature força uma nova medidade de temperatura para atualizar a capacitancia do oscilador
#define RS2   4 //(RS2 and RS1). These bits control the frequency of the square-wave output when the square wave has been enabled
#define RS1   3 // 0=1Hz,1=1024Hz, 2= 4096Hz,3=8192Hz
#define INTCN 2 // Interrupt Control When the INTCN bit is set to logic 0, a square wave is output on the INT/SQW pin. When the
//INTCN bit is set to logic 1, then a match between the timekeeping registers and either of the alarm registers
//activates the INT/SQW output (if the alarm is also enabled).
#define A2IE  1 //Alarm 2 Interrupt Enable
#define A1IE  0 //Alarm 1 Interrupt Enable

//BITS RTC CONTROL_2

#define OSF     7 //Oscillator Stop Flag
#define EN32kHz 3 //Enable 32kHz Output
#define BSY     2 //This bit indicates the device is busy executing TCXO functions.
#define A2F     1 //. A logic 1 in the alarm 2 flag bit indicates that the time matched the alarm 2 registers
#define A1F     0 // A logic 1 in the alarm 2 flag bit indicates that the time matched the alarm 2 registers

#define SUNDAY    1
#define MONDAY    2
#define TUESDAY   3
#define WEDNESDAY 4
#define THURSDAY  5
#define FRIDAY    6
#define SATURDAY  7


struct{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t monthday;
    uint8_t weekday;
    uint8_t month;
    uint16_t year;
} RTC;

uint8_t ds3231ReadRegister(uint8_t addr);
void    ds3231Start();
void    ds3231UpdateStruct();
void    ds3231setClock(uint8_t seconds,uint8_t minutes,uint8_t hour);
void    ds3231setCalendar(uint8_t day,uint8_t month,uint8_t year);
uint8_t ds3231ReadHour();
uint8_t ds3231ReadMinute();
uint8_t ds3231ReadSeconds(void);
uint8_t ds3231ReadWeekDay(void);
uint8_t ds3231ReadMonthDay(void);
uint8_t ds3231ReadMonth(void);
uint8_t ds3231ReadYear(void);
void    ds3231SetAlarm1Min(void);
void    ds3231ClearInterrupt(void);

#endif
