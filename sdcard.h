#ifndef SDCARD_H_INCLUDED
#define SDCARD_H_INCLUDED

#define   DRV_MMC    0
#include "ff14b/source/ff.h"     //Fat by ELM CHAN
#include "ff14b/source/diskio.h" //Disk Functions

void fatfsStatusToSerial(uint8_t r);      //USART output for the return to the fatfs functions
//DWORD get_fattime (void);                 //Converts the rtc data on a 32bit number http://elm-chan.org/fsw/ff/doc/fattime.html

#endif // SDCARD_H_INCLUDED

