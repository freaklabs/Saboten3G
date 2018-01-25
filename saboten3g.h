#pragma once

/************************************************************************/
// 
//
//
/************************************************************************/
#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <avr/pgmspace.h>
#include <limits.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SdFat.h>
#include "ds3231.h"

#define RESP_SZ 500

// for SIM5320 modem type
enum
{
    SIM5320A,
    SIM5320E,
    SIM5320J
};


// for alarm
enum
{
    EVERY_SECOND,
    EVERY_MINUTE,
    EVERY_HOUR     ,
    EVERY_DAY    ,
    EVERY_WEEK    ,
    EVERY_MONTH    
};

enum
{
    NO_ALARM,
    MINUTE_ALARM,
    HOUR_ALARM
};

class Saboten3G 
{
public:
    // inputs
    const uint8_t pinPwrStatus     = 15;
    const uint8_t pinRI            = 23;
    const uint8_t pinVsolSense     = 28;
    const uint8_t pinNwkStatus     = 29;
    const uint8_t pinVbatSense     = 31;
    const uint8_t pinCdet          = 18;

    // outputs
    const uint8_t pinWake          = 14;
    const uint8_t pinPwrButton     = 21;
    const uint8_t pinDtr           = 22;
    const uint8_t pinFlightMode    = 30;
    const uint8_t pinRtcIntp       = 6;
    const uint8_t pinSdSeln        = 19;
    const uint8_t pinLevelSensor   = 24;
    const uint8_t pinGpsEnb        = 25;
    const uint8_t pinLevelEnb      = 27;

    char respBuf[RESP_SZ];
    uint8_t rssi;
    boolean sleeping = false;
    uint8_t retries = 0;
    SdFat sd;
    SdFile file;

    Saboten3G();
    boolean begin(HardwareSerial *port, SoftwareSerial *lvl, HardwareSerial *debug);
    uint8_t getIntp();
    static void rtcIntp();
    boolean rtcIntpRcvd();

    void drvrSend(const char* command);
    boolean drvrCheckResp(const char *expected, uint32_t timeout);
    boolean drvrCheckOK(uint32_t timeout);
    void drvrDumpResp();
    void drvrFlushSerInput();
    void drvrSleepMcu();
    uint32_t drvrElapsedTime(uint32_t);
    void drvrCmdEcho(boolean enable);
    void drvrPowerOn();
    boolean drvrPowerOff();
    boolean drvrReset();
    void drvrFlightMode(boolean enable);
    float drvrGetVbat();
    float drvrGetVsol();

    void mgmtGetInfo();
    int8_t mgmtGetRSSI();

    boolean httpOpen(const char *url, uint16_t port);
    boolean httpSend(const char *url, const char *data, uint16_t len);
    boolean httpResp(const char *resp);

//    void httpStart();
//    void httpOpenSession(const char *url, uint16_t port, boolean ssl);
//    void httpSend(const char *data, uint32_t timeout);
//    void httpRecv(uint16_t size);

    boolean sdBegin(uint8_t csPin);
    boolean sdOpen(const char *filename, uint8_t mode = O_RDWR | O_CREAT | O_APPEND);
    void sdLs();
    boolean sdMkDir(const char *filepath);
    boolean sdExists(const char *filepath);
    boolean sdChDir(const char *filepath);    
    int16_t sdRead();
    boolean sdWrite(const char *data);
    boolean sdClose();
    boolean sdRemove(const char *filename);
    uint32_t sdAvailable();
    static void sdDateTime(uint16_t *date, uint16_t *time);

    void rtcSetTime(int yr, int month, int day, int hour, int min, int sec);
    static struct ts rtcGetTime();
    void rtcPrintTime(char *datetime);
    void rtcPrintDate(char *datetime);
    void rtcPrintTimeAndDate(char *datetime);
    void rtcPrintFullTime(char *datetime);
    void rtcSetAlarm(uint8_t alarm, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t repeat);
    void rtcGetAlarm(uint8_t alarm, uint8_t *data);
    uint8_t rtcGetStatus();
    uint8_t rtcGetControl();
    void rtcSetStatus(uint8_t reg);
    void rtcClearAlarm(uint8_t alarm);
    void rtcEnableAlarm(uint8_t alarm);
    void rtcDisableAlarm(uint8_t alarm);
    float rtcGetTemp();

    void levelOn();
    void levelOff();
    uint32_t levelRead();

    void gpsEnable();
    void gpsDisable();
    void gpsRead(char *data);
    void gpsQuiet();
    void gpsRadioOn();
    void gpsRadioOff();
    void gpsFlush();
    void gpsPoll();
    void gpsSendUBX(uint8_t *msg, uint8_t len);
    void gpsPowerSaveMode();

    HardwareSerial *ser;
    HardwareSerial *dbg;
    SoftwareSerial *gps;
};