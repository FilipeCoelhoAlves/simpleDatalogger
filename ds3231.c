#include "ds3231.h"

uint8_t ds3231ReadRegister(uint8_t addr)
{
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(addr);
    i2cStartCondition();
    i2cSend(DS3231_ADDR_R);
    uint8_t temp=i2cReadNACK();
    i2cStopCondition();
    return temp;
}

void ds3231Start(){
    uint8_t data=ds3231ReadRegister(RTC_SECONDS);
    data=(data&0x7F);
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_SECONDS);
    i2cSend(data);
    i2cStopCondition();
}

void ds3231UpdateStruct()
{
    uint8_t buffer[7];
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_SECONDS);
    i2cStartCondition();
    i2cSend(DS3231_ADDR_R);
    buffer[0]=i2cReadACK();
    buffer[1]=i2cReadACK();
    buffer[2]=i2cReadACK();
    buffer[3]=i2cReadACK();
    buffer[4]=i2cReadACK();
    buffer[5]=i2cReadACK();
    buffer[6]=i2cReadNACK();
    i2cStopCondition();
    RTC.second=((buffer[0]>>4))*10+(buffer[0]&0x0F);
    RTC.minute=((buffer[1]&0b01110000)>>4)*10+(buffer[1]&0x0F);
    if((buffer[2]>>5)&&1)
    {
        RTC.hour=((buffer[2]&0x0F)+20);
    }
    if((buffer[2]>>4)&&1)
    {
        RTC.hour=((buffer[2]&0x0F)+10);
    }
    else
    {
        RTC.hour=buffer[2]&0x0F;
    }
    RTC.weekday=buffer[3];
    RTC.monthday=(buffer[4]>>4)*10+(buffer[4]&0x0F);
    RTC.month=((buffer[5]>>4)&1)*10+(buffer[5]&0x0F);
    RTC.year=2000+(buffer[6]>>4)*10+(buffer[6]&0x0F);
}


void ds3231setClock(uint8_t seconds,uint8_t minutes,uint8_t hour)
{
    uint8_t temp_seconds=((seconds/10)<<4|seconds%10);
    uint8_t temp_minutes=((minutes/10)<<4|minutes%10);
    uint8_t temp_hour=0;
    if(hour<10)
    {
        temp_hour=hour;
    }
    if(hour>=10&&hour<20)
    {
        temp_hour=(1<<4)|(hour%10);
    }
    if(hour>=20)
    {
        temp_hour=(1<<5)|(hour%10);
    }
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_SECONDS);
    i2cSend(temp_seconds);
    i2cSend(temp_minutes);
    i2cSend(temp_hour);
    i2cStopCondition();
}

void ds3231setCalendar(uint8_t day,uint8_t month,uint8_t year)
{
    uint8_t temp_day=((day/10)<<4|day%10);
    uint8_t temp_month=((month/10)<<4|month%10);
    uint8_t temp_year=(year/10)<<4|(year%10);
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_MONTH_DAY);
    i2cSend(temp_day);
    i2cSend(temp_month);
    i2cSend(temp_year);
    i2cStopCondition();
}

uint8_t ds3231ReadHour()
{
    uint8_t temp_hour=ds3231ReadRegister(RTC_HOUR);
    if((temp_hour>>5)&&1)
    {
        return ((temp_hour&0x0F)+20);
    }
    if((temp_hour>>4)&&1)
    {
        return ((temp_hour&0x0F)+10);
    }
    else
    {
        return temp_hour&0x0F;
    }
}

uint8_t ds3231ReadMinute()
{
    uint8_t temp_minutes=ds3231ReadRegister(RTC_MINUTES);
    RTC.minute=((temp_minutes&0b01110000)>>4)*10+(temp_minutes&0b00001111);
    return RTC.minute;
}

uint8_t ds3231ReadSeconds()
{
    uint8_t temp_minutes=ds3231ReadRegister(RTC_SECONDS);
    RTC.second=((temp_minutes>>4))*10+(temp_minutes&0b00001111);
    return RTC.second;
}


uint8_t ds3231ReadWeekDay(void)
{
    RTC.weekday=ds3231ReadRegister(RTC_WEEK_DAY);
    return RTC.weekday;
}

uint8_t ds3231ReadMonthDay(void)
{
    uint8_t temp=ds3231ReadRegister(RTC_MONTH_DAY);
    RTC.monthday=(temp>>4)*10+(temp&0b00001111);
    return RTC.monthday;
}

uint8_t ds3231ReadMonth(void)
{
    uint8_t temp=ds3231ReadRegister(RTC_MONTH);
    RTC.month=((temp>>4)&1)*10+(temp&0b00001111);
    return RTC.month;
}

uint8_t ds3231ReadYear(void)
{
    uint8_t temp=ds3231ReadRegister(RTC_YEAR);
    RTC.year=(temp>>4)*10+(temp&0b00001111);
    return RTC.year;
}

void   ds3231SetAlarm1Min()
{
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_ALARM_1_SECONDS);
    i2cSend(0x0);
    i2cSend(0x0);
    i2cSend(0x0);
    i2cSend(0x0);
    i2cSend(0x80);
    i2cSend(0x80);
    i2cSend(0x80);
    i2cStopCondition();
    i2cStartCondition();
    i2cSend(DS3231_ADDR_W);
    i2cSend(RTC_CONTROL_1);
    i2cSend(0x1E);
    i2cStopCondition();
}

void ds3231ClearInterrupt()
{
    uint8_t x=ds3231ReadRegister(RTC_CONTROL_2);
    if(x&&0x02)
    {
        i2cStartCondition();
        i2cSend(DS3231_ADDR_W);
        i2cSend(RTC_CONTROL_2);
        i2cSend(x&0xFC);
        i2cStopCondition();

    }
}
