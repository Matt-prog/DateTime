
#ifndef Time_h
#define Time_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ESP8266 //code only for ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
//#include <time.h>
#endif

/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif

//#define DateTime_SAVE_FLASH //UNCOMMENT TO SAVE FLASH MEMORY (Warning: some functions might not work)
//#define DateTime_USE_TIMESPAN //UNCOMMENT IF YOU WANT TO USE getTimeSpan() OR setTimeSpan() IN DateTime CLASS

#define DateTime_USE_ALARM //UNCOMMENT TO USE ALARM CLASS
#define DateTime_USE_MILLISTIMER //UNCOMMENT TO USE MILLIS TIMER CLASS
  
#define NTP_SERVER_URL_SIZE 60

#define null_time -32768

#ifdef ESP8266 //code only for ESP8266
#define NTP_PACKET_SIZE 48
#endif

#define WD_CALIB 1 //Day of week calibration value //TODO calibrate

#define DAYS_IN_400_YRS 146097L
#define DAYS_IN_100_YRS 36524L //without last leap day
#define DAYS_IN_4_YRS 1461L //last year is leap year
#define DAYS_IN_YEAR 365L //without leap day

#define MILLISECOND   1
#define SECOND        1000
#define MINUTE        60000L
#define HOUR          3600000L
#define DAY           86400000L
#define TIMEZONE_REPAIR1 100.00
#define TIMEZONE_REPAIR2 36000L
#ifndef DateTime_SAVE_FLASH
#define WEEK          604800000LL
#define MONTH_31      2678400000LL
#define MONTH_30      2592000000LL
#define MONTH_29      2505600000LL
#define MONTH_28      2419200000LL
#define MONTH         MONTH_30
#define YEAR          31536000000LL
#define LEAP_YEAR     31622400000LL
#endif

#define MILLIS_IN_DAY   DAY
#define HOUR_IN_MILLIS  HOUR

//getWriteOne def:
#define GWO_HOUR 1
#define GWO_MINUTE 2
#define GWO_SECOND 3
#define GWO_MILLIS 4
#define GWO_YEAR 5
#define GWO_MONTH 6
#define GWO_DAY 7
#define GWO_WEEKDAY 8

#define FORMAT_12HOUR 0
#define FORMAT_24HOUR 1

#define AM 0
#define PM 1
  
#define time_base 59926608000000LL //1.1.1900 0:0:0:0 time base for NTP server epoch
#define time_base1970 62135596800000LL //1.1.1970 0:0:0:0 time base for UNIX time

#ifndef DateTime_SAVE_FLASH
//WARNING: DD_MM_YYYY = 01.12.2019 or 15.01.896, but D_M_YY = 1.12.19 or 15.1.96
//every even format have to has short month
#define DD_MM_YYYY  1
#define DD_M_YYYY   2
#define D_MM_YYYY   3
#define D_M_YYYY    4
#define MM_YYYY     5
#define M_YYYY      6
#define YYYY_MM_DD  7
#define YYYY_M_DD   8
#define YYYY_MM_D   9
#define YYYY_M_D    10
#define YYYY_MM     11
#define YYYY_M      12
#define DD_MM_YY    13
#define DD_M_YY     14
#define D_MM_YY     15
#define D_M_YY      16
#define MM_YY       17
#define M_YY        18
#define YY_MM_DD    19
#define YY_M_DD     20
#define YY_MM_D     21
#define YY_M_D      22
#define YY_MM       23
#define YY_M        24
#define DD_MM       25
#define DD_M        26
#define D_MM        27
#define D_M         28
#define MM_DD       29
#define M_DD        30
#define MM_D        31
#define M_D         32

#define HH_MM_SS_mmm  100
#define H_M_S_m       101
#define HH_MM_SS      102
#define H_M_S         103
#define HH_MM         104
#define H_M           105
#define M_S           106
#define MM_SS         107
#define M_S_m         108
#define MM_SS_mmm     109
#define S_m           110
#define SS_mmm        111

#endif

enum{ //days of weak
  WD_SUNDAY = 1,
  WD_MONDAY = 2,
  WD_TUESDAY = 3,
  WD_WEDNESDAY = 4,
  WD_THURSDAY = 5,
  WD_FRIDAY = 6,
  WD_SATURDAY = 7
};
//#endif

#ifdef ESP8266 //code only for ESP8266

enum{ //NTP errors
  NTP_NONE = 0,
  NTP_OK = 1,
  NTP_NOT_FOUND = 2,
  NTP_TIMEOUT = 3,
  NTP_BAD_RESPONSE = 4,
  NTP_NO_INTERNET = 5
};

#endif


typedef struct{ //time structure to hold time
  //time_s(int hour,int minute, int second, int milliseconds, int year, int month, int day, int weekday) : hour(hour), minute(minute), second(second), milliseconds(milliseconds), year(year), month(month), day(day), weekday(weekday) {};
  //time_s() : hour(null_time), minute(null_time), second(null_time), milliseconds(null_time), year(null_time), month(null_time), day(null_time), weekday(null_time) {};
  short hour = null_time;
  short minute = null_time;
  short second = null_time;
  short milliseconds = null_time;
  short year = null_time;
  short month = null_time;
  short day = null_time;
  short weekday = null_time;
}time_s;



extern String toStr64(int64_t val);
//extern String toStr64(uint64_t val);
extern int32_t daysFromYearZero(int32_t year);
extern void yearFromMillis(int64_t &mil_in, short &year, long &days_in_year);
extern bool isLeapYear(short year);
extern byte daysInMonth(byte month, short year = null_time);
extern uint16_t dayInYear(byte day, byte month, short year = null_time);
extern byte getWeekday(int64_t &mil_in);


#ifdef DateTime_USE_MILLISTIMER
class MillisTimer
{
  public:
  //constructor:
  MillisTimer();
  MillisTimer(unsigned long millis_interval);

  void setInterval(unsigned long millis_interval);
  unsigned long getInterval();

  unsigned long remainingTime();
  
  void onHandle(void(*callback)());
  void handleTime();

  void reset();

  bool enabled();
  void enable(bool _en = true);
  bool repeat();
  void repeat(bool _rep);

  private:
  void (*_onHandle)();
  unsigned long next_mil = 0;
  bool en;
  bool rep;
  unsigned long interval = 0;
};
#endif


#ifndef DateTime_SAVE_FLASH
class TimeSpan
{
  public:
  //constructors:
  TimeSpan();
  TimeSpan(int64_t days, long hours = 0, long minutes = 0, long seconds = 0, long milliseconds = 0);

  void set(int64_t days = 0, long hours = 0, long minutes = 0, long seconds = 0, long milliseconds = 0);
  void get(long &days, long &hours, long &minutes, long &seconds, long &milliseconds);
  void get(long &days, long &hours, long &minutes, long &seconds);
  void get(long &days, long &hours, long &minutes);
  void get(long &days, long &hours);
  void get(long &days);

  long days(long _days = null_time);
  long hours(long _hours = null_time);
  long minutes(long _minutes = null_time);
  long seconds(long _seconds = null_time);
  long milliseconds(long _mil = null_time);

  void raw(int64_t _raw);
  int64_t raw();

  //operators:
  operator int64_t /*const*/ (){
    return raw_time;
  }
  
  TimeSpan& operator()(int64_t days = 0, long hours = 0, long minutes = 0, long seconds = 0, long milliseconds = 0){
    set(days,hours,minutes,seconds,milliseconds);
    return *this;
  }

  TimeSpan& operator=(/*const*/ int64_t _raw) {
    raw_time = _raw;
    return *this;
  }

  /*int64_t operator+(const int64_t _raw) {
    return raw_time + _raw;
  }*/

  /*int64_t operator+(DateTime& dt2) {
    return raw_time + dt2.raw();
  }*/

  int64_t operator+(TimeSpan& ts2) {
    return raw_time + ts2.raw();
  }

  /*int64_t operator-(int64_t _raw) {
    return raw_time - _raw;
  }*/

  /*int64_t operator-(DateTime& dt2) {
    return raw_time - dt2.raw();
  }*/

  int64_t operator-(TimeSpan& ts2) {
    return raw_time - ts2.raw();
  }

  TimeSpan& operator+=(/*const*/ int64_t _raw) {
    raw_time += _raw;
    return *this;
  }

  /*TimeSpan& operator+=(DateTime& dt2) {
    raw_time += dt2.raw();
    return *this;
  }*/

  TimeSpan& operator+=(TimeSpan& ts2) {
    raw_time += ts2.raw();
    return *this;
  }

  TimeSpan& operator-=(/*const*/ int64_t _raw) {
    raw_time -= _raw;
    return *this;
  }

  /*TimeSpan& operator-=(DateTime& dt2) {
    raw_time -= dt2.raw();
    return *this;
  }*/

  TimeSpan& operator-=(TimeSpan& ts2) {
    raw_time -= ts2.raw();
    return *this;
  }

  bool operator==(/*const*/ TimeSpan &ts2) /*const*/ {
    return raw_time == ts2.raw();
  }

  bool operator==(int64_t mil) /*const*/ {
    return raw_time == mil;
  }

  bool operator>=(/*const*/ TimeSpan &ts2) /*const*/ {
    return raw_time >= ts2.raw();
  }

  bool operator>=(int64_t mil) /*const*/ {
    return raw_time >= mil;
  }

  bool operator<=(/*const*/ TimeSpan &ts2) /*const*/ {
    return raw_time <= ts2.raw();
  }

  bool operator<=(int64_t mil) /*const*/ {
    return raw_time <= mil;
  }

  bool operator<(/*const*/ TimeSpan &ts2) /*const*/ {
    return raw_time < ts2.raw();
  }

  bool operator<(int64_t mil) /*const*/ {
    return raw_time < mil;
  }

  bool operator>(/*const*/ TimeSpan &ts2) /*const*/ {
    return raw_time > ts2.raw();
  }

  bool operator>(int64_t mil) /*const*/ {
    return raw_time > mil;
  }

  private:
  int64_t raw_time = 0;
};

#endif


class DateTime
{
  public:
  //constructors:
  DateTime();
  DateTime(DateTime &dt);
  DateTime(int64_t mil);
  DateTime(byte hour, byte minute, byte second);
  DateTime(byte hour, byte minute, byte second, uint16_t mil);
  DateTime(byte hour, byte minute, byte second, uint16_t mil, int16_t year, byte month, byte day);

  void raw(int64_t _raw, bool UTC = false);
  int64_t raw(bool UTC = false);

  /*operator int64_t const (){
    if(opUTC) return raw_time;
    return raw_time + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;
  }*/

  operator int64_t (){
    synchNow();
    return raw(opUTC);
  }

  DateTime& operator() (short hour, short minute = null_time, short second = null_time, short mil = null_time,short year = null_time, short month = null_time, short day = null_time){
    set(hour,minute,second,mil,year,month,day);
    return *this;
  }

  DateTime& operator=(int64_t _raw) {
    raw(_raw,opUTC);
    if(synchEN)synch_millis = millis(); //set new interval timer value, because this is same as synchronization
    return *this;
  }

  /*int64_t operator+(int64_t _raw) {
    if(opUTC) return raw_time + _raw;
    return raw_time + _raw + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;
  }*/

  int64_t operator+(DateTime& dt2) {
    return raw(opUTC) + dt2.raw();
    /*if(opUTC) return raw_time + dt2.raw();
    return raw_time + dt2.raw(opUTC) + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;*/
  }

  #ifndef DateTime_SAVE_FLASH
  int64_t operator+(TimeSpan &ts2) {
    return raw(opUTC) + ts2.raw();
    /*if(opUTC) return raw_time + ts2.raw();
    return raw_time + ts2.raw() + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;*/
  }
  #endif

  /*int64_t operator-(int64_t _raw) {
    if(opUTC) return raw_time - _raw;
    return raw_time - _raw + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;
  }*/

  int64_t operator-(DateTime& dt2) {
    return raw(opUTC) + dt2.raw();
    /*if(opUTC) return raw_time - dt2.raw();
    return raw_time - dt2.raw(opUTC) + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;*/
  }

  #ifndef DateTime_SAVE_FLASH
  int64_t operator-(TimeSpan &ts2) {
    return raw(opUTC) + ts2.raw();
    /*if(opUTC) return raw_time - ts2.raw();
    return raw_time - ts2.raw() + TIMEZONE_REPAIR2*(long)(timezone*TIMEZONE_REPAIR1) + shift*MINUTE;*/
  }
  #endif

  DateTime operator+=(int64_t _raw) {
    raw_time += _raw;
    return *this;
  }

  DateTime operator+=(DateTime& dt2) {
    raw_time += dt2.raw(opUTC);
    return *this;
  }

  #ifndef DateTime_SAVE_FLASH
  DateTime operator+=(TimeSpan &ts2) {
    raw_time += ts2.raw();
    return *this;
  }
  #endif

  DateTime operator-=(int64_t _raw) {
    raw_time -= _raw;
    return *this;
  }

  DateTime operator-=(DateTime& dt2) {
    raw_time += dt2.raw(opUTC);
    return *this;
  }

  #ifndef DateTime_SAVE_FLASH
  DateTime operator-=(TimeSpan &ts2) {
    raw_time += ts2.raw();
    return *this;
  }
  #endif

  bool operator==(DateTime& dt2) {
    return raw(opUTC) == dt2.raw(opUTC);
  }

  bool operator==(int64_t mil) {
    return raw(opUTC) == mil;
  }

  bool operator>=(DateTime& dt2) {
    return raw(opUTC) >= dt2.raw(opUTC);
  }

  bool operator>=(int64_t mil) {
    return raw(opUTC) >= mil;
  }

  bool operator<=(DateTime& dt2) {
    return raw(opUTC) <= dt2.raw(opUTC);
  }

  bool operator<=(int64_t mil){
    return raw(opUTC) <= mil;
  }

  bool operator<(DateTime& dt2) {
    return raw(opUTC) < dt2.raw(opUTC);
  }

  bool operator<(int64_t mil) {
    return raw(opUTC) < mil;
  }

  bool operator>(DateTime& dt2) {
    return raw(opUTC) > dt2.raw(opUTC);
  }

  bool operator>(int64_t mil){
    return raw(opUTC) > mil;
  }

  uint16_t millisecondsUTC(uint16_t mil = null_time);
  byte hourUTC(short hr = null_time, bool pm = true);
  byte minuteUTC(short mi = null_time);
  byte secondUTC(short se = null_time);
  int16_t yearUTC(short ye = null_time);
  byte monthUTC(short mo = null_time);
  byte dayUTC(short da = null_time);
  byte weekdayUTC();

  uint16_t milliseconds(uint16_t mil = null_time);
  byte hour(short hr = null_time, bool pm = true);
  byte minute(short mi = null_time);
  byte second(short se = null_time);
  int16_t year(short ye = null_time);
  byte month(short mo = null_time);
  byte day(short da = null_time);
  byte weekday();

  long daysUTC();
  long days();

  #if defined(DateTime_SAVE_FLASH) || defined(DateTime_USE_TIMESPAN)
  void setTimeSpan(int64_t days, long hours = 0, long minutes = 0, long seconds = 0, long milliseconds = 0);
  void getTimeSpan(long &days, long &hours, long &minutes, long &seconds, long &milliseconds);
  void getTimeSpan(long &days, long &hours, long &minutes, long &seconds);
  void getTimeSpan(long &days, long &hours, long &minutes);
  void getTimeSpan(long &days, long &hours);
  void getTimeSpan(long &days);
  #endif

  void setUNIX(uint32_t tim, short ms = 0);
  uint32_t getUNIX();
  void setUNIX_UTC(uint32_t tim, short ms = 0);
  uint32_t getUNIX_UTC();

  void setUTC(time_s time){setUTC(time.hour,time.minute,time.second,time.milliseconds,time.year,time.month,time.day);}
  void setUTC(short hour, short minute = null_time, short second = null_time, short mil = null_time,short year = null_time, short month = null_time, short day = null_time);
  void setDateUTC(short year, short month = null_time, short day = null_time);

  void set(time_s time){set(time.hour,time.minute,time.second,time.milliseconds,time.year,time.month,time.day);}
  void set(short hour, short minute = null_time, short second = null_time, short mil = null_time,short year = null_time, short month = null_time, short day = null_time);
  void setDate(short year, short month = null_time, short day = null_time);

  time_s getUTC();
  void getUTC(short &hour, short &minute, short &second, short &mil,short &year, short &month, short &day);
  void getUTC(short &hour, short &minute, short &second, short &mil);
  void getUTC(short &hour, short &minute, short &second);
  void getDateUTC(short &year, short &month, short &day);


  time_s get();
  void get(short &hour, short &minute, short &second, short &mil,short &year, short &month, short &day);
  void get(short &hour, short &minute, short &second, short &mil);
  void get(short &hour, short &minute, short &second);
  void getDate(short &year, short &month, short &day);

  void operatorsUseUTC(bool _en);
  bool operatorsUseUTC(){return opUTC;};

  void setTimezone(float _timezone);
  float getTimezone();

  //shift is in minutes
  void DST(bool _en, int8_t _shift = 60);
  int8_t DST(){return shift;} //Returns DST shift in minutes
  bool isDST(){return shift != 0;} //Returns true if DST is enabled

  void format(bool _form); //In this version 12-hour format works only with functions hour and hourUTC and all to String functions
  bool format(){return time_format;} //Returns hour format
  bool isPM();
  bool isAM();

  void copy(DateTime &dt, bool cpy_time = true, bool cpy_TZ = true, bool cpy_DST = true, bool cpy_format = true, bool cpy_operatorsUseUTC = true, bool cpy_synch = true, bool cpy_NTP = true);
  
  //DO NOT USE TWO FUNCTIONS BELLOW
  void synchCPY(void(*callback)(time_s*),bool *onSynchUTC_, bool *swr_, unsigned int *synch_interval_, unsigned long *synch_millis_);
  #ifdef ESP8266 //code only for ESP8266
  void NTPsynchCPY(void (*callback)(byte), byte *ntp_err_, int *TZDST_err_, char ntp_server_url_[], bool *ntp_en_, unsigned int *ntp_synch_int_, WiFiUDP *ntp_, char clck_id_[5], bool *prepared_, unsigned long *TZDST_mil_, unsigned long *ntp_mil_);
  #endif
  //////////////////////////////////

  #ifndef DateTime_SAVE_FLASH
  String toLongTimeString(byte _form, const char *separator1, const char *separator2, const char *separator3, bool UTC = false);
  String toShortTimeString(byte _form, const char *separator1, const char *separator2, const char *separator3, bool UTC = false);
  String toLongDateString(byte _form, const char *separator1, const char *separator2, bool UTC = false);
  String toShortDateString(byte _form, const char *separator1, const char *separator2, bool UTC = false);
  
  String toLongTimeString(byte _form = H_M_S, const char *separator1 = ":", bool UTC = false){
     return toLongTimeString(_form, separator1, separator1, separator1, UTC);
  }
  String toShortTimeString(byte _form = H_M_S, const char *separator1 = ":", bool UTC = false){
    return toShortTimeString(_form, separator1, separator1, separator1, UTC);
  }
  String toLongDateString(byte _form = YY_M_D, const char *separator1 = ".", bool UTC = false){
    return toLongDateString(_form, separator1, separator1, UTC);
  }
  String toShortDateString(byte _form = YY_M_D, const char *separator1 = ".", bool UTC = false){
    return toShortDateString(_form, separator1, separator1, UTC);
  }
  
  String toLongTimeString(byte _form, String separator1, String separator2, String separator3, bool UTC = false){
     return toLongTimeString(_form, separator1.c_str(), separator2.c_str(), separator3.c_str(), UTC);
  }
  String toShortTimeString(byte _form, String separator1, String separator2, String separator3, bool UTC = false){
    return toShortTimeString(_form, separator1.c_str(), separator2.c_str(), separator3.c_str(), UTC);
  }
  String toLongDateString(byte _form, String separator1, String separator2, bool UTC = false){
    return toLongDateString(_form, separator1.c_str(), separator2.c_str(), UTC);
  }
  String toShortDateString(byte _form, String separator1, String separator2, bool UTC = false){
    return toShortDateString(_form, separator1.c_str(), separator2.c_str(), UTC);
  }
  
  String toLongTimeString(byte _form, String separator1, bool UTC = false){
     return toLongTimeString(_form, separator1.c_str(), separator1.c_str(), separator1.c_str(), UTC);
  }
  String toShortTimeString(byte _form, String separator1, bool UTC = false){
    return toShortTimeString(_form, separator1.c_str(), separator1.c_str(), separator1.c_str(), UTC);
  }
  String toLongDateString(byte _form, String separator1, bool UTC = false){
    return toLongDateString(_form, separator1.c_str(), separator1.c_str(), UTC);
  }
  String toShortDateString(byte _form, String separator1, bool UTC = false){
    return toShortDateString(_form, separator1.c_str(), separator1.c_str(), UTC);
  }
  #endif

  void onSynch(void(*callback)(time_s*), bool UTC = false, bool write_raw = false);
  void synchNow(bool now = true);
  void synchInterval(unsigned int interval); //interval in seconds
  bool synchEnable(bool en);
  bool synchEnabled();
  unsigned long remainingSynchTime();

  #ifdef ESP8266 //code only for ESP8266

  void setNTPserver(char* addr);
  char* NTPclockID();
  byte NTPlastError();
  void NTPinterval(unsigned int interval);
  void NTPenable(bool en = true);
  bool NTPenabled();
  byte NTPsynchNow();
  long remainingNTPsynchTime();
  
  byte NTPhandler();

  void onNTPsynch(void(*callback)(byte));
  
  int updateTZ_DST_lastError();
  int updateTZ_DST(unsigned long interval = 0);
  #endif
  
  private:

  void (*_onSynch)(time_s*);
  unsigned long synch_millis = 0;
  bool onSynchUTC = false;
  bool swr = false; //synch write raw
  bool synchEN = false;
  unsigned int synch_interval = 300; //default is 5 minutes
  int64_t raw_time = 0; //from year 292277024B.C. to year 292277024
  float timezone = 0;
  int16_t shift = 0;
  bool time_format = FORMAT_24HOUR;
  bool _PM = false;
  bool opUTC = false; //Use UTC with operators

  #ifdef ESP8266 //code only for ESP8266
  const unsigned int localPort = 2390; // local port to listen for UDP packets
  byte ntp_err = 0; //Last error from NTP synchronization
  int TZDST_err = 0; //Last error from Time zone and DST update
  char ntp_server_url[NTP_SERVER_URL_SIZE] = {0}; //NTP server address
  bool ntp_en = false; //True if ntp synchronization is enabled
  unsigned int ntp_synch_int = 120; //ntp synchronization interval in seconds
  WiFiUDP ntp; //NTP server
  byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
  char clck_id[5] = {0}; //clock ID
  bool prepared;
  unsigned long TZDST_mil = 0; //Time zone and DST next update time
  unsigned long ntp_mil;
  void (*_onNTPsynch)(byte);

  void sendNTPpacket(IPAddress& address);
  int getTzDST(long* TZ_offset, long* DST_offset);
  HTTPClient http;
  #endif

  #ifndef DateTime_SAVE_FLASH
  const __FlashStringHelper *f_min;
  const __FlashStringHelper *f_s;
  const __FlashStringHelper *f_ms;
  #endif

  void readSynchTime(short *hour, short *minute, short *second, short *mil, short *year, short *month, short *day, bool UTC = false);
  void dateTimeToRaw(int64_t *mil_out, short hour, short minute, short second, short mil, short year, short month, short day);
  void rawToDateTime(int64_t *mil_in, short *hour, short *minute, short *second, short *mil, short *year, short *month, short *day);
  short getWriteOne(short value, byte variable, bool UTC = false);
  #ifndef DateTime_SAVE_FLASH
  String String_(short value, byte dec_places);
  #endif
};


#ifdef DateTime_USE_ALARM
class Alarm
{
  public:
  //constructors:
  Alarm();
  Alarm(DateTime &dt);
  Alarm(time_s &tim);
  Alarm(short hour, short minute, short second = 0, short mil = 0, short year = null_time, short month = null_time, short day = null_time);

  void setAlarm(DateTime &dt);
  void setAlarm(time_s &tim);
  void setAlarm(short hour, short minute, short second = 0, short mil = 0, short year = null_time, short month = null_time, short day = null_time);
  void getAlarm(DateTime &dt);
  time_s getAlarm();
  void getAlarm(short &hour, short &minute, short &second, short &mil, short &year, short &month, short &day);
  void getAlarm(short &hour, short &minute, short &second, short &mil);
  void getAlarm(short &hour, short &minute, short &second);
  void getAlarm(short &hour, short &minute);

  void onDays(/*const*/ byte *_days, byte _count);
  void onDays();
  bool enabled();
  void enable(bool _en = true);
  bool repeat();
  void repeat(bool _rep);
  void setSynch(DateTime *dt);
  void setSynch(time_s *tim);
  void resetSynch();
  void resetProtection();

  void onRinging(void(*callback)(time_s));

  bool handleAlarm(short hour = null_time, short minute = null_time, short second = null_time, short mil = null_time, short year = null_time, short month = null_time, short day = null_time);

  private:

  void (*_onRinging)(time_s);
  bool null_alarm = false;
  bool en = false;
  bool rep = false;
  byte *days;
  byte days_cnt = 0;
  bool days_en = false;
  byte synch_stat = 0; //0: manual synch, 1: DateTime synch, 2: time structure synch
  unsigned long next_ring_mi = 0;
  DateTime *synch_dt;
  time_s *synch_tim;
  time_s al_time;
};
#endif


/*! CPP guard */
#ifdef __cplusplus
}
#endif

#endif
