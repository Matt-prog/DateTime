
#include "DateTime.h"

/*This function convert int64_t (8-bit number) to string, because original function
 *String() can convert maximaly 4-bit number. This function is also usefull in debug process,
 *where you can print raw value of DateTime or TimeSpan.
*/
String toStr64(int64_t val){ //convert 64 bit number to string
  bool neg = val < 0;
  if(neg) val = -val;
  String ret = String((byte)(val%10));
  while((val = val/10) > 0){
    ret = String((byte)(val%10)) + ret;
  }
  if(neg) ret = "-" + ret;
  return ret;
}

/*String toStr64(uint64_t val){ //convert 64 bit number to string
  String ret = String((byte)(val%10));
  while((val = val/10) > 0){
    ret = String((byte)(val%10)) + ret;
  }
  return ret;
}*/

/* This function returns how many day elapsed from date 0001.01.01.
 * If using years B.C., then it returns negative value.
 * It also counts with leap years as every function in this library.
 */
int32_t daysFromYearZero(int32_t year){
  if(year >= 0) year--; //skip year 0 because it does not exist
  bool neg = (year < 0); //check if year is B.C.
  int32_t days = 0;
  
  //getting count of leap days
  if(neg) days = ((year+1)/400)*97 + (((year+1)%400)/100)*24 + ((year+1)%100)/4;
  else days = (year/400)*97 + ((year%400)/100)*24 + (year%100)/4;
  
  days += year*365; //add normal days
  
  if(neg) days--; //this option is here because year 1 B.C. is also leap day
  return days;
}

/* This function returns year calculated from milliseconds from date 0000.01.01 and time 00:00:00:001
 * It can also returns years B.C. if input parameter mil_in is negative.
 * It also returns how many days elapsed in that year(starts from 1)
 * mil_in is input parameter; year and days_in_year are output parameters
 */
void yearFromMillis(int64_t &mil_in, short &year, long &days_in_year){
  char data[4];
  long excluded_days = 0;
  days_in_year = 0;
  bool neg = mil_in < 0;
  
  //calculate days from milliseconds
  if(neg) excluded_days = (mil_in+1)/MILLIS_IN_DAY;
  else excluded_days = mil_in/MILLIS_IN_DAY;
  if(neg) excluded_days-=1;

  /* Leap days are every fourth year, but with one exception.
   * If year is divisible by 100 and not divisible by 400 it is not leap year.
   * For example year 1900 was not leap year, but year 2000 was leap year.
   */
  data[0] = excluded_days/DAYS_IN_400_YRS; //count of fourth centuries
  if(neg) data[0]--;
  excluded_days = abs(excluded_days - data[0]*DAYS_IN_400_YRS); //rest days (this is modulo)
  data[1] = excluded_days/DAYS_IN_100_YRS; //count of centuries
  if(data[1] > 3){ //this is last day of 400 years block and it is leap
    data[1] = 3;
    if(neg) days_in_year = 1;
    else days_in_year = 366;
  }
  excluded_days = excluded_days - data[1]*DAYS_IN_100_YRS; //rest days after last century (now last leap day is 356th day not 366th) (also act as modulo)
  data[2] = excluded_days/DAYS_IN_4_YRS; //count of 4 year blocks
  excluded_days = excluded_days - data[2]*DAYS_IN_4_YRS; //rest days after last 4 years block (also act as modulo)
  data[3] = excluded_days/DAYS_IN_YEAR; //count of years
  if(data[3] > 3){ //this is normal leap day(which is every fourth yeary with some exceptions)
    data[3] = 3;
    days_in_year = 366;
  }
  if(days_in_year == 0)days_in_year = abs(excluded_days - data[3]*DAYS_IN_YEAR)+1; //days in year

  year = data[3] + data[2]*4 + data[1]*100 + data[0]*400; //put it all together
  if(!neg) year+=1; //skip year 0
}

//Returns true if the year is LEAP
bool isLeapYear(short year){
  if(year < 0) year++; //skip year 0 because it does not exist
  if(year%400 == 0) return true;
  return ((year%4) == 0 && (year%100) != 0);
}

/*Returns how many days are in specific month
 *Value year is optional and it is used to estable leap year
 */
byte daysInMonth(byte month, short year){
  const byte dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if(month == 2 && year != null_time && isLeapYear(year)) return 29; //this is February in leap year
  if(month < 1) month = 1;
  else if(month > 12) month = 12;
  return dim[month - 1];
}

/*This function returns how many days elapsed from 1.January
 *Did not starts from zero (the first of January is first day, so this return 1)
 *Value year is optional and it is used to estable leap year
 */
uint16_t dayInYear(byte day, byte month, short year){
  if(day < 1) day = 1;
  byte dim = daysInMonth(month,year);
  if(day > dim) day = dim;
  uint16_t days = 0;
  for(byte i = 1; i < month; i++){
    days += daysInMonth(i,year);
  }
  return days+day;
}



MillisTimer::MillisTimer(){
  interval = 0;
}

//Sets interval to timer in milliseconds
MillisTimer::MillisTimer(unsigned long millis_interval){
  interval = millis_interval;
}

//Sets interval to timer in milliseconds
void MillisTimer::setInterval(unsigned long millis_interval){
  next_mil -= interval;
  interval = millis_interval;
  next_mil += interval;
}

//Return timer interval
unsigned long MillisTimer::getInterval(){return interval;}

//Return remaining time
unsigned long MillisTimer::remainingTime(){
  unsigned long ret = next_mil - millis();
  if(ret < 0 || !en) ret = 0;
  else if(ret > interval) ret = interval;
  return ret;
}

//Sets function which will be done after time elapsed
void MillisTimer::onHandle(void(*callback)()){
  _onHandle = callback;
}

//This function have to be included in void loop()
void MillisTimer::handleTime(){
  if(millis() >= next_mil && en){
    if(_onHandle){
      _onHandle();
    }
    if(rep)next_mil += interval;
    else en = false;
  }
}

//Resets timer interval
void MillisTimer::reset(){
  next_mil = millis()+interval;
}

//Return true if timer is enabled
bool MillisTimer::enabled(){ return en;}

//Enable or disable timer
void MillisTimer::enable(bool _en){
  if(!en && _en) reset();
  en = _en;
}

//Return if timer will be re-enabled after elapsing timer
bool MillisTimer::repeat(){return rep;}

//Sets repeating flag
void MillisTimer::repeat(bool _rep){
  rep = _rep;
}



Alarm::Alarm(){
  null_alarm = true;
}

//Sets alarm at specific Date and Time
Alarm::Alarm(DateTime &dt){
  null_alarm = false;
  al_time = dt.get();
}

//Sets alarm at specific Date and Time
Alarm::Alarm(short hour, short minute, short second, short mil, short year, short month, short day){
  null_alarm = false;
  al_time.hour = hour;
  al_time.minute = minute;
  al_time.second = second;
  al_time.milliseconds = mil;
  al_time.year = year;
  al_time.month = month;
  al_time.day = day;
}

//Sets alarm at specific Date and Time
//You do not need to set weekday it will be done automatically
Alarm::Alarm(time_s &tim){
  null_alarm = false;
  al_time = tim;
}

//Sets alarm at specific Date and Time
void Alarm::setAlarm(DateTime &dt){
  null_alarm = false;
  next_ring_mi = 0;
  al_time = dt.get();
}

//Sets alarm at specific Date and Time
//You do not need to set weekday it will be done automatically
void Alarm::setAlarm(time_s &tim){
  null_alarm = false;
  next_ring_mi = 0;
  al_time = tim;
}

//Sets alarm at specific Date and Time
void Alarm::setAlarm(short hour, short minute, short second, short mil, short year, short month, short day){
  null_alarm = false;
  next_ring_mi = 0;
  al_time.hour = hour;
  al_time.minute = minute;
  al_time.second = second;
  al_time.milliseconds = mil;
  al_time.year = year;
  al_time.month = month;
  al_time.day = day;
}

//Returns alarm Date and Time
void Alarm::getAlarm(DateTime &dt){
  dt.set(al_time);
}

//Returns alarm Date and Time
time_s Alarm::getAlarm(){
  return al_time;
}

//Returns alarm Date and Time
void Alarm::getAlarm(short &hour, short &minute, short &second, short &mil, short &year, short &month, short &day){
  hour = al_time.hour;
  minute = al_time.minute;
  second = al_time.second;
  mil = al_time.milliseconds;
  year = al_time.year;
  month = al_time.month;
  day = al_time.day;
}

//Returns alarm Date and Time
void Alarm::getAlarm(short &hour, short &minute, short &second, short &mil){
  hour = al_time.hour;
  minute = al_time.minute;
  second = al_time.second;
  mil = al_time.milliseconds;
}

//Returns alarm Date and Time
void Alarm::getAlarm(short &hour, short &minute, short &second){
  hour = al_time.hour;
  minute = al_time.minute;
  second = al_time.second;
}

//Returns alarm Date and Time
void Alarm::getAlarm(short &hour, short &minute){
  hour = al_time.hour;
  minute = al_time.minute;
}

//Resets protection which disable "doubled" alarm ring
//This protection is automatically reset aproximmatelly 23 hours after the last ring
//We recommend to use it when synch time is changed by more than one hour
void Alarm::resetProtection(){
  next_ring_mi = 0;
}

//Sets function which will be done when timer rings
void Alarm::onRinging(void(*callback)(time_s)){
  _onRinging = callback;
}

//Sets array of days when the alarm will ring
//If repeating is dissabled, those days will be ignored
//Input value _count is size of the array
//EXAMPLE: byte days[] = {WD_MONDAY, WD_FRIDAY};
//         onDays(days,2);
//You can use enums WD_MONDAY,WD_TUESDAY, ...
void Alarm::onDays(byte *_days, byte _count){
  days = _days;
  days_cnt = _count;
  days_en = true;
}

//This function sets all days in week when the alarm will ring
void Alarm::onDays(){
  days_en = false;
}

//Return true if Alarm is enabled
bool Alarm::enabled(){return en;}

//Enable or disable Alarm
void Alarm::enable(bool _en){
  en = _en;
  next_ring_mi = 0;
}

//Return true if repeating Alarm is enabled
bool Alarm::repeat(){return rep;}

//Enable or disable repeating
//WARNIG: If repeating is disabled, Alarm will be disabled after first ring, that mean you have to re-enable it before another use
void Alarm::repeat(bool _rep){
  rep = _rep;
}

//Set time synchronization to Alarm clocks
//Use this function only when the DateTime instance have enabled synchronization ("DateTime::synchEnable(true)")
//EXAMPLE: setSynch(&myDateTime);
void Alarm::setSynch(DateTime *dt){
  synch_stat = 1;
  synch_dt = dt;
}

//Set time synchronization to Alarm clocks
//Use this function only when the time_s structure is synchronized with clock
//You do not need to use this function when synchronizing time, only you need to do is only write new value into the time_s structure
//Example: setSynch(&myTimeStructure);
void Alarm::setSynch(time_s *tim){
  synch_stat = 2;
  synch_tim = tim;
}

//Sets that synchronization will be done manually in function handleAlarm
void Alarm::resetSynch(){
  synch_stat = 0;
}

//This function have to be included in void loop()!!!
//Use input values only when clock synchronization is set to manually (using resetSynch())
bool Alarm::handleAlarm(short hour, short minute, short second, short mil, short year, short month, short day){
  if(en && !null_alarm){
    time_s now;
    //getting the present time
    if(hour == null_time && (synch_stat == 1 || synch_stat == 2)){
      if(synch_stat == 1){ //Synchronized DateTime class
        now = synch_dt->get(); //copy time structure from DateTime
      }
      else{
        now = *synch_tim; // copy time strucutre
      }
    }
    else{
      //fill time structure by values from the arguments of this function
      now.hour = hour;
      now.minute = minute;
      now.second = second;
      now.milliseconds = mil;
      now.year = year;
      now.month = month;
      now.day = day;
    }

    bool ring = false;
    bool date_available = (now.year != null_time && now.month != null_time && now.day != null_time && now.weekday != null_time && al_time.year != null_time && al_time.month != null_time && al_time.day != null_time);
    if(date_available){//we can work with date and days
      //checking if present time is greather than set and if we dont repeat same alarm
      //protection is disabled here
      if(!(/*next_ring_mi <= millis() &&*/ now.hour >= al_time.hour && now.minute >= al_time.minute && now.second >= al_time.second && now.milliseconds >= al_time.milliseconds && 
           now.year >= al_time.year && now.month >= al_time.month && now.day >= al_time.day)) return true;
      
      byte next_av_day = 0;
      if(days_en && days_cnt > 0 && rep){ //there we can work with days in week
        next_av_day = 7;
        int8_t diff = 0;

        //calculating weekday from data
        if(now.weekday == null_time) now.weekday = (((daysFromYearZero(year) + dayInYear(day,month,year)) + WD_CALIB) % 7)+1;
        
        for(byte i = 0; i < days_cnt; i++){
          if(days[i] >= 1 && days[i] <= 7){ //validate days
            ring |= (days[i] == now.weekday); //check if today is allowed day
            //trying to find next available day
            diff = days[i] - now.weekday;
            if(diff <= 0) diff = 7+diff;
            if(next_av_day > diff) next_av_day = diff;
          }
        }
        if(!ring) return true;
      }
      else{
        ring = true;
      }
      
      if(ring){
        next_ring_mi = millis()+HOUR_IN_MILLIS*23 - MINUTE; //the alarm can only ring after one day (with some tolerance)
        if(rep){
          //add days
          if(days_en) al_time.day += next_av_day;
          else al_time.day += 1;
          if(al_time.day > daysInMonth(al_time.month, al_time.year)){
            al_time.day -= daysInMonth(al_time.month, al_time.year);
            al_time.month++;
          }
          if(al_time.month > 12){//Happy new year
             al_time.month = 1;
             al_time.year++;
          }
        }
      }
    }
    else if(now.milliseconds != null_time && al_time.milliseconds != null_time){ //we can work with: hour,minute,second,milliseconds
      ring = (next_ring_mi <= millis() && now.hour >= al_time.hour && now.minute >= al_time.minute && now.second >= al_time.second && now.milliseconds >= al_time.milliseconds);
      if(ring) next_ring_mi = millis()+HOUR_IN_MILLIS*23 - MINUTE; //the alarm can only ring after one day (with some tolerance)
    }
    else if(now.second != null_time && al_time.second != null_time){ //we can work with: hour,minute,second
      ring = (next_ring_mi <= millis() && now.hour >= al_time.hour && now.minute >= al_time.minute && now.second >= al_time.second);
      if(ring) next_ring_mi = millis()+HOUR_IN_MILLIS*23 - MINUTE; //the alarm can only ring after one day (with some tolerance)
    }
    else if(now.minute != null_time && al_time.minute != null_time){ //we can work with: hour and minute
      ring = (next_ring_mi <= millis() && now.hour >= al_time.hour && now.minute >= al_time.minute);
      if(ring) next_ring_mi = millis()+HOUR_IN_MILLIS*23 - MINUTE; //the alarm can only ring after one day (with some tolerance)
    }
    else{
      return false; //bad set timer or bad handler argumenths
    }
    
    if(ring){ //alarm is ringing finally
      if(!rep) en = false;
      if(_onRinging){
        _onRinging(now);
      }
      //else return false;
    }
  }
  return true;
}



#ifndef DateTime_SAVE_FLASH
TimeSpan::TimeSpan(){
  raw_time = 0;
}

//Set time span value
TimeSpan::TimeSpan(int64_t days, long hours, long minutes, long seconds, long milliseconds){
  raw_time = days*DAY + hours*HOUR + minutes*MINUTE + seconds*SECOND + milliseconds;
}

//Use this function, if you want to find out the difference between two DateTimes
//first millisecond in negative is 0,0,0,0,-1 and first millisecond in positive is 0,0,0,0,0
void TimeSpan::set(int64_t days, long hours, long minutes, long seconds, long milliseconds){
  raw_time = days*DAY + hours*HOUR + minutes*MINUTE + seconds*SECOND + milliseconds;
}

//Use this function, if you want to find out the difference between two DateTimes
//first millisecond in negative is 0,0,0,0,-1 and first millisecond in positive is 0,0,0,0,0
void TimeSpan::get(long &days, long &hours, long &minutes, long &seconds, long &milliseconds){
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
  seconds = (raw_time%MINUTE)/SECOND;
  milliseconds = raw_time%SECOND;
}

//Use this function, if you want to find out the difference between two DateTimes
//first second in negative is 0,0,0,-1 and first second in positive is 0,0,0,0
void TimeSpan::get(long &days, long &hours, long &minutes, long &seconds){
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
  seconds = (raw_time%MINUTE)/SECOND;
}

//Use this function, if you want to find out the difference between two DateTimes
//first minute in negative is 0,0,-1 and first minute in positive is 0,0,0
void TimeSpan::get(long &days, long &hours, long &minutes){
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
}

//Use this function, if you want to find out the difference between two DateTimes
//first hour in negative is 0,-1 and first hour in positive is 0,0
void TimeSpan::get(long &days, long &hours){
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
}

//Use this function, if you want to find out the difference between two DateTimes
//first day in negative is -1 and first day in positive is 0
void TimeSpan::get(long &days){
  days = raw_time/DAY;
}

//Returns days and/or write new value
long TimeSpan::days(long _days){
  if(_days != null_time){
    set(_days,(raw_time%DAY)/HOUR,(raw_time%HOUR)/MINUTE,(raw_time%MINUTE)/SECOND,raw_time%SECOND);
  }
  return raw_time/DAY;
}

//Returns hours and/or write new value
long TimeSpan::hours(long _hours){
  if(_hours != null_time){
    set(raw_time/DAY,_hours,(raw_time%HOUR)/MINUTE,(raw_time%MINUTE)/SECOND,raw_time%SECOND);
  }
  return (raw_time%DAY)/HOUR;
}

//Returns minutes and/or write new value
long TimeSpan::minutes(long _minutes){
  if(_minutes != null_time){
    set(raw_time/DAY,(raw_time%DAY)/HOUR,_minutes,(raw_time%MINUTE)/SECOND,raw_time%SECOND);
  }
  return (raw_time%HOUR)/MINUTE;
}

//Returns seconds and/or write new value
long TimeSpan::seconds(long _seconds){
  if(_seconds != null_time){
    set(raw_time/DAY,(raw_time%DAY)/HOUR,(raw_time%HOUR)/MINUTE,_seconds,raw_time%SECOND);
  }
  return (raw_time%MINUTE)/SECOND;
}

//Returns milliseconds and/or write new value
long TimeSpan::milliseconds(long _mil){
  if(_mil != null_time){
    set(raw_time/DAY,(raw_time%DAY)/HOUR,(raw_time%HOUR)/MINUTE,(raw_time%MINUTE)/SECOND,_mil);
  }
  return raw_time%SECOND;
}

#endif

//Sets raw value
void TimeSpan::raw(int64_t _raw){
  raw_time = _raw;
}

//Returns raw value
int64_t TimeSpan::raw(){
  return raw_time;
}

//Create null DateTime
DateTime::DateTime(){
  raw_time = 0; //0001.01.01 00:00:00:000
}

//Create DateTime from another DateTime
DateTime::DateTime(DateTime &dt){
  copy(dt);
}

DateTime::DateTime(int64_t mil){
  raw_time = mil;
}

DateTime::DateTime(byte hour, byte minute, byte second){
  dateTimeToRaw(&raw_time, hour, minute, second, 0, 1,1,1);
}

DateTime::DateTime(byte hour, byte minute, byte second, uint16_t mil){
  dateTimeToRaw(&raw_time, hour, minute, second, mil, 1,1,1);
}

DateTime::DateTime(byte hour, byte minute, byte second, uint16_t mil, int16_t year, byte month, byte day){
  dateTimeToRaw(&raw_time, hour, minute, second, mil, year, month, day);
}

//Sets raw value in milliseconds
//Input value UTC is optional and it will convert raw value into UTC by set timezone and DST
void DateTime::raw(int64_t _raw, bool UTC){
  raw_time = _raw;
  if(!UTC) raw_time = raw_time - (long)(HOUR_IN_MILLIS*timezone) - shift*MINUTE;
  if(synchEN)synch_millis = millis(); //set new interval timer value, because this is same as synchronization
}

//Returns raw value in milliseconds
//Input value UTC is optional, if it true, it will return raw UTC value
//Raw value is count of milliseconds from date 0001.01.01 and time 00:00:00:001
int64_t DateTime::raw(bool UTC){
  synchNow(false);
  if(!UTC) return raw_time + (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE;
  return raw_time;
}



//This function copy informations from DateTime dt to this class
//DateTime dt - class you want to copy from
//cpy_time - true if you want to copy date and time
//cpy_TZ - true if you want to copy time zone
//cpy_DST - true if you want to copy DST settings
//cpy_format - true if you want to copy hour format
//cpy_operatorsUseUTC - true if you want to copy operatorsUseUTC() setting
//cpy_synch - true if you want to copy all synch settings
//cpy_NTP - true if you want to copy all NTP synch settings
void DateTime::copy(DateTime &dt, bool cpy_time, bool cpy_TZ, bool cpy_DST, bool cpy_format, bool cpy_operatorsUseUTC, bool cpy_synch, bool cpy_NTP){
  if(cpy_time) raw(dt.raw(true),true);
  if(cpy_TZ) setTimezone(dt.getTimezone());
  if(cpy_DST) DST(true,dt.DST());
  if(cpy_format) format(dt.format());
  if(cpy_operatorsUseUTC) operatorsUseUTC(dt.operatorsUseUTC());
  if(cpy_synch){
    dt.synchCPY(_onSynch,&onSynchUTC,&swr,&synch_interval,&synch_millis);
    synchEnable(dt.synchEnabled());
  }
  #ifdef ESP8266 //code only for ESP8266
  if(cpy_NTP){
    dt.NTPsynchCPY(_onNTPsynch,&ntp_err,&TZDST_err,ntp_server_url,&ntp_en,&ntp_synch_int,&ntp,clck_id,&prepared,&TZDST_mil,&ntp_mil);
  }
  #endif
}


void DateTime::synchCPY(void(*callback)(time_s*),bool *onSynchUTC_, bool *swr_, unsigned int *synch_interval_, unsigned long *synch_millis_){
  callback = _onSynch;
  *onSynchUTC_ = onSynchUTC;
  *swr_ = swr;
  *synch_interval_ = synch_interval;
  *synch_millis_ = synch_millis;
}

#ifdef ESP8266 //code only for ESP8266
void DateTime::NTPsynchCPY(void (*callback)(byte), byte *ntp_err_, int *TZDST_err_, char ntp_server_url_[], bool *ntp_en_, unsigned int *ntp_synch_int_, WiFiUDP *ntp_, char clck_id_[5], bool *prepared_, unsigned long *TZDST_mil_, unsigned long *ntp_mil_){
  callback = _onNTPsynch;
  *ntp_err_ = ntp_err;
  *TZDST_err_ = TZDST_err;
  strcpy(ntp_server_url_,ntp_server_url);
  *ntp_en_ = ntp_en;
  *ntp_synch_int_ = ntp_synch_int;
  *ntp_ = ntp;
  for(byte i = 0; i < 5; i++) clck_id_[i] = clck_id[i];
  *prepared_ = prepared;
  *TZDST_mil_ = TZDST_mil;
  *ntp_mil_ = ntp_mil;
}
#endif


//Sets and/or return milliseconds in UTC
uint16_t DateTime::millisecondsUTC(uint16_t mil){
  return getWriteOne(mil,GWO_MILLIS,true);
}

//Sets and/or return milliseconds
uint16_t DateTime::milliseconds(uint16_t mil){
  return getWriteOne(mil,GWO_MILLIS,false);
}

//Sets and/or return hour in UTC in set format
byte DateTime::hourUTC(short hr, bool pm){
  if(time_format == FORMAT_12HOUR){
    if(hr != null_time){
      if(!pm && hr == 12) hr = 0;
      else if(pm && hr > 12) hr+=12;
    }
    byte ret = getWriteOne(hr,GWO_HOUR,true);
    if(ret < 12){
      _PM = false;
      if(ret == 0) ret = 12;
    }
    else{
      _PM = true;
      if(ret > 12) ret-=12;
    }
    return ret;
  }
  else{
    return getWriteOne(hr,GWO_HOUR,true);
  }
}

//Sets and/or return hour in set format
byte DateTime::hour(short hr, bool pm){
  if(time_format == FORMAT_12HOUR){
    if(hr != null_time){
      if(!pm && hr == 12) hr = 0;
      else if(pm && hr > 12) hr+=12;
    }
    byte ret = getWriteOne(hr,GWO_HOUR,false);
    if(ret < 12){
      _PM = false;
      if(ret == 0) ret = 12;
    }
    else{
      _PM = true;
      if(ret > 12) ret-=12;
    }
    return ret;
  }
  else{
    return getWriteOne(hr,GWO_HOUR,false);
  }
}

//Sets and/or return minute in UTC
byte DateTime::minuteUTC(short mi){
  return getWriteOne(mi,GWO_MINUTE, true);
}

//Sets and/or return minute
byte DateTime::minute(short mi){
  return getWriteOne(mi,GWO_MINUTE, false);
}

//Sets and/or return second in UTC
byte DateTime::secondUTC(short se){
  return getWriteOne(se,GWO_MINUTE, true);
}

//Sets and/or return second
byte DateTime::second(short se){
  return getWriteOne(se,GWO_MINUTE, false);
}

//Sets and/or return year in UTC
int16_t DateTime::yearUTC(short ye){
  return getWriteOne(ye,GWO_YEAR, true);
}

//Sets and/or return year
int16_t DateTime::year(short ye){
  return getWriteOne(ye,GWO_YEAR, false);
}

//Sets and/or return month in UTC
byte DateTime::monthUTC(short mo){
  return getWriteOne(mo,GWO_MINUTE, true);
}

//Sets and/or return month
byte DateTime::month(short mo){
  return getWriteOne(mo,GWO_MINUTE, false);
}

//WARNING: use this function (for writing) after monthUTC, never otherwise,
//         because for example february has only 28 days and maximum is the 28 not 31,
//         so it can cause bug
//Sets and/or return day in UTC
byte DateTime::dayUTC(short da){
  return getWriteOne(da,GWO_MINUTE, true);
}

//WARNING: use this function (for writing) after month, never otherwise,
//         because for example february has only 28 days and maximum is the 28 not 31,
//         so it can cause bug
//Sets and/or return day
byte DateTime::day(short da){
  return getWriteOne(da,GWO_MINUTE, false);
}

//Sets and/or return weekday in UTC
//Sunday is 1, Monday is 2, ...
//Or use WD_SUNDAY, WA_MONDAY, ...
byte DateTime::weekdayUTC(){
  //1 is sunday, 2 monday, ...
  return getWriteOne(0,GWO_WEEKDAY, true);
}

//Sets and/or return weekday
//Sunday is 1, Monday is 2, ...
//Or use WD_SUNDAY, WA_MONDAY, ...
byte DateTime::weekday(){
  //1 is sunday, 2 monday, ...
  return getWriteOne(0,GWO_WEEKDAY, false);
}

//Returns count of all days from date 0001.01.01 in UTC
//This function is reccomended to use when this class is used as timespan
//First day in negative is day -1 and first day in positive is 0 
long DateTime::daysUTC(){
  synchNow(false);
  if(raw_time < 0) return((raw_time+1)/MILLIS_IN_DAY)-1;
  else return raw_time/MILLIS_IN_DAY;
}

//Returns count of all days from date 0001.01.01 in UTC
//This function is reccomended to use when this class is used as timespan
//First day in negative is day -1 and first day in positive is 0 
long DateTime::days(){
  synchNow(false);
  if(raw_time < 0) return((raw_time + 1 + (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE)/MILLIS_IN_DAY)-1;
  else return (raw_time + (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE)/MILLIS_IN_DAY;
}


#ifdef DateTime_SAVE_FLASH || DateTime_USE_TIMESPAN
//UTC, DST and SYNCHRONIZATION will NOT work in those TimeSpan functions bellow
//Use this function, if you want to find out the difference between two DateTimes
//first millisecond in negative is 0,0,0,0,-1 and first millisecond in positive is 0,0,0,0,0
void DateTime::setTimeSpan(int64_t days, long hours, long minutes, long seconds, long milliseconds){
  raw_time = days*DAY + hours*HOUR + minutes*MINUTE + seconds*SECOND + milliseconds;
}

//Use this function, if you want to find out the difference between two DateTimes
//first millisecond in negative is 0,0,0,0,-1 and first millisecond in positive is 0,0,0,0,0
void DateTime::getTimeSpan(long &days, long &hours, long &minutes, long &seconds, long &milliseconds){
  synchNow(false);
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
  seconds = (raw_time%MINUTE)/SECOND;
  milliseconds = raw_time%SECOND;
}

//Use this function, if you want to find out the difference between two DateTimes
//first second in negative is 0,0,0,-1 and first second in positive is 0,0,0,0
void DateTime::getTimeSpan(long &days, long &hours, long &minutes, long &seconds){
  synchNow(false);
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
  seconds = (raw_time%MINUTE)/SECOND;
}

//Use this function, if you want to find out the difference between two DateTimes
//first minute in negative is 0,0,-1 and first minute in positive is 0,0,0
void DateTime::getTimeSpan(long &days, long &hours, long &minutes){
  synchNow(false);
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
  minutes = (raw_time%HOUR)/MINUTE;
}

//Use this function, if you want to find out the difference between two DateTimes
//first hour in negative is 0,-1 and first hour in positive is 0,0
void DateTime::getTimeSpan(long &days, long &hours){
  synchNow(false);
  days = raw_time/DAY;
  hours = (raw_time%DAY)/HOUR;
}

//Use this function, if you want to find out the difference between two DateTimes
//first day in negative is -1 and first day in positive is 0
void DateTime::getTimeSpan(long &days){
  synchNow(false);
  days = raw_time/DAY;
}
#endif


//Sets time using UNIX time format (time in seconds elapsed from 1.1.1970)
//Parameter ms sets milliseconds (optional)
void DateTime::setUNIX(uint32_t tim, short ms){
  raw_time = (int64_t)tim*SECOND + time_base1970 + ms;
}

//Returns time in UNIX time format (time in seconds elapsed from 1.1.1970)
//This value can be easly converted to time_t (e.g. time_t now = dt.getUNIX();)
uint32_t DateTime::getUNIX(){
  synchNow(false);
  return (raw_time - time_base1970)/SECOND;
}

//Sets new DateTime values in UTC
void DateTime::setUTC(short hour, short minute, short second, short mil, short year, short month, short day){
  if(minute == null_time || second == null_time || mil == null_time || year == null_time || month == null_time || day == null_time){
    short data[7] = {0};
    readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], true);
    if(hour == null_time) hour = data[0];
    if(minute == null_time) minute = data[1];
    if(second == null_time) second = data[2];
    if(mil == null_time) mil = data[3];
    if(year == null_time) year = data[4];
    if(month  == null_time) month = data[5];
    if(day == null_time) day = data[6];
  }
  dateTimeToRaw(&raw_time, hour, minute, second, mil, year, month, day);
}

//Sets new DateTime values
void DateTime::set(short hour, short minute, short second, short mil, short year, short month, short day){
  if(minute == null_time || second == null_time || mil == null_time || year == null_time || month == null_time || day == null_time){
    short data[7] = {0};
    readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], false);
    if(hour == null_time) hour = data[0];
    if(minute == null_time) minute = data[1];
    if(second == null_time) second = data[2];
    if(mil == null_time) mil = data[3];
    if(year == null_time) year = data[4];
    if(month  == null_time) month = data[5];
    if(day == null_time) day = data[6];
  }
  dateTimeToRaw(&raw_time, hour, minute, second, mil, year, month, day);
  raw_time = raw_time - (long)(HOUR_IN_MILLIS*timezone) - shift*MINUTE; //write UTC
}

//Sets new Date in UTC
void DateTime::setDateUTC(short year, short month, short day){
  short data[7] = {0};
  readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], true);
  if(year == null_time) year = data[4];
  if(month  == null_time) month = data[5];
  if(day == null_time) day = data[6];
  dateTimeToRaw(&raw_time, data[0], data[1], data[2], data[3], year, month, day);
}

//Sets new Date
void DateTime::setDate(short year, short month, short day){
  short data[7] = {0};
  readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], false);
  if(year == null_time) year = data[4];
  if(month  == null_time) month = data[5];
  if(day == null_time) day = data[6];
  dateTimeToRaw(&raw_time, data[0], data[1], data[2], data[3], year, month, day);
  raw_time = raw_time - (long)(HOUR_IN_MILLIS*timezone) - shift*MINUTE; //write UTC
}

//Returns time structure (time_s) in UTC
time_s DateTime::getUTC(){
  short data[7] = {0};
  readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], true);
  int32_t weekday = (((daysFromYearZero(data[4]) + dayInYear(data[6],data[5],data[4])) + WD_CALIB) % 7)+1;
  time_s ret;
  ret.hour = data[0];
  ret.minute = data[1];
  ret.second = data[2];
  ret.milliseconds = data[3];
  ret.year = data[4];
  ret.month = data[5];
  ret.day = data[6];
  ret.weekday = weekday;
  return ret;
}

//Returns time structure (time_s)
time_s DateTime::get(){
  short data[7] = {0};
  readSynchTime(&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6], false);
  int32_t weekday = (((daysFromYearZero(data[4]) + dayInYear(data[6],data[5],data[4])) + WD_CALIB) % 7)+1;
  time_s ret;
  ret.hour = data[0];
  ret.minute = data[1];
  ret.second = data[2];
  ret.milliseconds = data[3];
  ret.year = data[4];
  ret.month = data[5];
  ret.day = data[6];
  ret.weekday = weekday;
  return ret;
}

//Returns values of DateTime in UTC
void DateTime::getUTC(short &hour, short &minute, short &second, short &mil,short &year, short &month, short &day){
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, true);
}

//Returns values of DateTime
void DateTime::get(short &hour, short &minute, short &second, short &mil,short &year, short &month, short &day){
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, false);
}

//Returns values of DateTime in UTC
void DateTime::getUTC(short &hour, short &minute, short &second, short &mil){
  short year,month,day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, true);
}

//Returns values of DateTime
void DateTime::get(short &hour, short &minute, short &second, short &mil){
  short year,month,day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, false);
}

//Returns values of DateTime in UTC
void DateTime::getUTC(short &hour, short &minute, short &second){
  short mil,year,month,day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, true);
}

//Returns values of DateTime
void DateTime::get(short &hour, short &minute, short &second){
  short mil,year,month,day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, false);
}

//Returns values of DateTime in UTC
void DateTime::getDateUTC(short &year, short &month, short &day){
  short hour, minute, second, mil;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, true);
}

//Returns values of DateTime
void DateTime::getDate(short &year, short &month, short &day){
  short hour, minute, second, mil;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, false);
}

//If true, operators will calculate with UTC time
void DateTime::operatorsUseUTC(bool _en){
  opUTC = _en;
}

//Sets function which will be done when synchronization is needed
//Second parameter sets if new data are in UTC or not
//When third parameter is true, synchronization inside of synchronization function
//have to be written in raw form using "raw" function
void DateTime::onSynch(void(*callback)(time_s*), bool UTC, bool write_raw){
  _onSynch = callback;
  onSynchUTC = UTC;
  swr = write_raw;
}

//Sets synchronization interval in milliseconds
//WARNING: If synchronization is enabled, and you call some function to get time, it will
//         automatically synchronize this time with millis() function, but if the set interval expires,
//         it will call function which is set in onSynch function
void DateTime::synchInterval(unsigned int interval){
  synch_interval = interval;
}

//Returns remaining time to the next synch in milliseconds
unsigned long DateTime::remainingSynchTime(){
  if(!synchEN) return 0;
  return (synch_millis+synch_interval*SECOND) - millis();
}

//Enables or disables synchronization
//If enabled: value will dynamically increase
//If disabled: value will be still static
bool DateTime::synchEnable(bool en){
  bool old = synchEN;
  synchEN = en;
  if(old && !synchEN){
    //Last time synch before disabling synchronization
    raw_time += (millis()-synch_millis);
    synch_millis = 0; //reset
  }
  //else if(!old && synchEN) synch_millis = millis(); //set new interval timer value
  return old;
}

//Returns true if synchronization is enabled
bool DateTime::synchEnabled(){
  return synchEN;
}

//Sets time zone in hours
//Value 0 is UTC
//Available time zones: -11,-10,-9,...,-1,0,+1,...,+10,+11,+12
//You can also use this -3.5 for GMT-3:30
void DateTime::setTimezone(float _timezone){
  if(_timezone > 12.00) timezone = 12.00;
  else if(_timezone < -11.00) timezone = -11.00;
  else timezone = _timezone;
}

//Returns set time zone
float DateTime::getTimezone(){
  return timezone;
}

//Enable or disable DST
//Second parameter sets shift in minutes (default is 60)
//WARNING: This function cannot determine the time change. You have to do it manually by calling this function.
void DateTime::DST(bool _en, int8_t _shift){
  if(!_en) shift = 0;
  else shift = _shift;
}

//Sets hour format (FORMAT_12HOUR, FORMAT_24HOUR (default))
//WARNING: In this version 12-hour format works only with functions hour and hourUTC and all to String functions
void DateTime::format(bool _form){
  time_format = _form;
}

//Return true if time is PM
//WARNING: Use only after calling any "get time" function, not before, because it will return bad value
bool DateTime::isPM(){
  return _PM;
}

//Return true if time is AM
//WARNING: Use only after calling any "get time" function, not before, because it will return bad value
bool DateTime::isAM(){
  return !_PM;
}

#ifndef DateTime_SAVE_FLASH //Functions bellow works only if save flash mode is not activated
//Returns time in long form text (for example: 12h:5min:5s:400ms)
//First parameter sets text form (for example H_M_S_m)
//Second sets first separator between values
//Third sets second separator between values
//Fourth sets third separator between values
//Fifth sets if value will be in UTC
String DateTime::toLongTimeString(byte _form, const char *separator1, const char *separator2, const char *separator3, bool UTC){
  if(_form < HH_MM_SS_mmm || _form > SS_mmm) _form = H_M_S;
  short hour, minute, second, mil, year, month, day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, UTC);
  String pm = "";
  f_ms = F("ms");
  f_s = F("s");
  f_min = F("min");
  if(time_format == FORMAT_12HOUR){
    if(hour < 12){
      pm=F("a.m.");
      if(hour == 0) hour = 12;
    }
    else{
      pm=F("p.m.");
      if(hour > 12) hour-=12;
    }
  }
  else pm=F("h");
  switch(_form){
    case HH_MM_SS_mmm: return String_(hour,2)+pm+separator1+String_(minute,2)+f_min+separator2+String_(second,2)+f_s+separator3+String_(mil,3)+f_ms;
    case H_M_S_m: return String(hour)+pm+separator1+String(minute)+f_min+separator2+String(second)+f_s+separator3+String(mil)+f_ms;
    case HH_MM_SS: return String_(hour,2)+pm+separator1+String_(minute,2)+f_min+separator2+String_(second,2)+f_s;
    case H_M_S: return String(hour)+pm+separator1+String(minute)+f_min+separator2+String(second)+f_s;
    case HH_MM: return String_(hour,2)+pm+separator1+String_(minute,2)+f_min;
    case H_M: return String(hour)+pm+separator1+String(minute)+f_min;
    case M_S: return String(minute)+f_min+separator1+String(second)+f_s;
    case MM_SS: return String_(minute,2)+f_min+separator1+String_(second,2)+f_s;
    case M_S_m: return String(minute)+f_min+separator1+String(second)+f_s+separator2+String(mil)+f_ms;
    case MM_SS_mmm: return String_(minute,2)+f_min+separator1+String_(second,2)+f_s+separator2+String_(mil,2)+f_ms;
    case S_m: return String(second)+f_s+separator1+String(mil)+f_ms;
    case SS_mmm: return String_(second,2)+f_s+separator1+String_(mil,2)+f_ms;
  }
}


//Returns time in short form text (for example: 12:5:5:400)
//First parameter sets text form (for example H_M_S_m)
//Second sets first separator between values
//Third sets second separator between values
//Fourth sets third separator between values
//Fifth sets if value will be in UTC
String DateTime::toShortTimeString(byte _form, const char *separator1, const char *separator2, const char *separator3, bool UTC){
  if(_form < HH_MM_SS_mmm || _form > SS_mmm) _form = H_M_S;
  short hour, minute, second, mil, year, month, day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, UTC);
  String pm = "";
  if(time_format == FORMAT_12HOUR){
    if(hour < 12){
      pm=F("a.m.");
      if(hour == 0) hour = 12;
    }
    else{
      pm=F("p.m.");
      if(hour > 12) hour-=12;
    }
  }
  switch(_form){
    case HH_MM_SS_mmm: return String_(hour,2)+pm+separator1+String_(minute,2)+separator2+String_(second,2)+separator3+String_(mil,3);
    case H_M_S_m: return String(hour)+pm+separator1+String(minute)+separator2+String(second)+separator3+String(mil);
    case HH_MM_SS: return String_(hour,2)+pm+separator1+String_(minute,2)+separator2+String_(second,2);
    case H_M_S: return String(hour)+pm+separator1+String(minute)+separator2+String(second);
    case HH_MM: return String_(hour,2)+pm+separator1+String_(minute,2);
    case H_M: return String(hour)+pm+separator1+String(minute);
    case M_S: return String(minute)+separator1+String(second);
    case MM_SS: return String_(minute,2)+separator1+String_(second,2);
    case M_S_m: return String(minute)+separator1+String(second)+separator2+String(mil);
    case MM_SS_mmm: return String_(minute,2)+separator1+String_(second,2)+separator2+String_(mil,3);
    case S_m: return String(second)+separator1+String(mil);
    case SS_mmm: return String_(second,2)+separator1+String_(mil,3);
  }
}

//Returns date in long form text (for example: 1.January.2019)
//First parameter sets text form (for example D_MM_YYYY)
//Second sets first separator between values
//Third sets second separator between values
//Fourth sets if value will be in UTC
String DateTime::toLongDateString(byte _form, const char *separator1, const char *separator2, bool UTC){
  if(_form < DD_MM_YYYY || _form > M_D) _form = D_MM_YYYY;
  short hour, minute, second, mil, year, month, day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, UTC);
  String BC = "";
  String month_ = "";
  switch(month){
    case 1: month_ = F("January"); break;
    case 2: month_ = F("February"); break;
    case 3: month_ = F("March"); break;
    case 4: month_ = F("April"); break;
    case 5: month_ = F("May"); break;
    case 6: month_ = F("June"); break;
    case 7: month_ = F("July"); break;
    case 8: month_ = F("August"); break;
    case 9: month_ = F("September"); break;
    case 10: month_ = F("October"); break;
    case 11: month_ = F("November"); break;
    case 12: month_ = F("December"); break;
  }
  if(_form % 2 == 0) month_ = month_.substring(0,3);
  if(year < 0) BC = F("B.C");
  switch(_form){
    case YYYY_MM_DD:
    case YYYY_M_DD: return String(year)+BC+separator1+month_+separator2+String_(day,2);
    case YY_M_D:
    case YY_MM_D: return String_(year,2)+BC+separator1+month_+separator2+String(day);
    case YY_M_DD:
    case YY_MM_DD: return String_(year,2)+BC+separator1+month+separator2+String(day,2);
    case DD_MM_YYYY:
    case DD_M_YYYY: return String_(day,2)+separator1+month_+separator2+String(year)+BC;
    case D_M_YY: 
    case D_MM_YY: return String(day)+separator1+month_+separator2+String_(year,2)+BC;
    case DD_M_YY:
    case DD_MM_YY: return String_(day,2)+separator1+month_+separator2+String_(year,2)+BC;
    case YYYY_M_D:
    case YYYY_MM_D: return String(year)+BC+separator1+month_+separator2+String(day);
    case D_M_YYYY:
    case D_MM_YYYY: return String(day)+separator1+month_+separator2+String(year)+BC;
    case D_M:
    case D_MM: return String(day)+separator1+month_;
    case DD_M:
    case DD_MM: return String_(day,2)+separator1+month_;
    case M_YY:
    case MM_YY: return month_+separator1+String_(year,2)+BC;
    case M_YYYY:
    case MM_YYYY: return month_+separator1+String(year)+BC;
    case M_D:
    case MM_D: return month_+separator1+String(day);
    case M_DD:
    case MM_DD: return month_+separator1+String_(day,2);
    case YY_M:
    case YY_MM: return String_(year,2)+BC+separator1+month_;
    case YYYY_M:
    case YYYY_MM: return String(year)+BC+separator1+month_;
  }
}

//Returns date in short form text (for example: 1.1.2019)
//First parameter sets text form (for example D_M_YYYY)
//Second sets first separator between values
//Third sets second separator between values
//Fourth sets if value will be in UTC
String DateTime::toShortDateString(byte _form, const char *separator1, const char *separator2, bool UTC){
  if(_form < DD_MM_YYYY || _form > M_D) _form = D_M_YYYY;
  short hour, minute, second, mil, year, month, day;
  readSynchTime(&hour, &minute, &second, &mil, &year, &month, &day, UTC);
  String BC = "";
  if(year < 0) BC = F("B.C");
  switch(_form){
    case YYYY_MM_DD: return String(year)+BC+separator1+String_(month,2)+separator2+String_(day,2);
    case YYYY_M_DD: return String(year)+BC+separator1+String(month)+separator2+String_(day,2);
    case YY_MM_D: return String_(year,2)+BC+separator1+String_(month,2)+separator2+String(day);
    case YY_M_D: return String_(year,2)+BC+separator1+String(month)+separator2+String(day);
    case YY_MM_DD: return String_(year,2)+BC+separator1+String_(month,2)+separator2+String_(day,2);
    case YY_M_DD: return String_(year,2)+BC+separator1+String(month)+separator2+String_(day,2);
    case DD_MM_YYYY: return String_(day,2)+separator1+String_(month,2)+separator2+String(year)+BC;
    case DD_M_YYYY: return String_(day,2)+separator1+String(month)+separator2+String(year)+BC;
    case D_MM_YY: return String(day)+separator1+String_(month,2)+separator2+String_(year,2)+BC;
    case D_M_YY: return String(day)+separator1+String(month)+separator2+String_(year,2)+BC;
    case DD_MM_YY: return String_(day,2)+separator1+String_(month,2)+separator2+String_(year,2)+BC;
    case DD_M_YY: return String_(day,2)+separator1+String(month)+separator2+String_(year,2)+BC;
    case YYYY_MM_D: return String(year)+BC+separator1+String_(month,2)+separator2+String(day);
    case YYYY_M_D: return String(year)+BC+separator1+String(month)+separator2+String(day);
    case D_MM_YYYY: return String(day)+separator1+String_(month,2)+separator2+String(year)+BC;
    case D_M_YYYY: return String(day)+separator1+String(month)+separator2+String(year)+BC;
    case D_MM: return String(day)+separator1+String_(month,2);
    case D_M: return String(day)+separator1+String(month);
    case DD_MM: return String_(day,2)+separator1+String_(month,2);
    case DD_M: return String_(day,2)+separator1+String(month);
    case MM_YY: return String_(month,2)+separator1+String_(year,2)+BC;
    case M_YY: return String(month)+separator1+String_(year,2)+BC;
    case M_YYYY: return String(month)+separator1+String(year)+BC;
    case MM_YYYY: return String_(month,2)+separator1+String(year)+BC;
    case MM_D: return String_(month,2)+separator1+String(day);
    case M_D: return String(month)+separator1+String(day);
    case MM_DD: return String_(month,2)+separator1+String_(day,2);
    case M_DD: return String(month)+separator1+String_(day,2);
    case YY_MM: return String_(year,2)+BC+separator1+String_(month,2);
    case YY_M: return String_(year,2)+BC+separator1+String(month);
    case YYYY_M: return String(year)+BC+separator1+String(month);
    case YYYY_MM: return String(year)+BC+separator1+String_(month,2);
  }
}

//Convert value to string but with set decimal places
String DateTime::String_(short value, byte dec_places){
  String ret = "";
  bool neg = value < 0;
  value = abs(value);
  for(byte i = 0; i < dec_places; i++){
    if(i > 0)value = value/10;
    ret=String(value%10) + ret;
  }
  if(neg) return '-' + ret;
  return ret;
}
#endif

#ifdef ESP8266 //code only for ESP8266

//Sets NTP server URL address
void DateTime::setNTPserver(char* addr){
  strncpy(ntp_server_url,addr,NTP_SERVER_URL_SIZE);
  ntp_server_url[NTP_SERVER_URL_SIZE-1] = 0;
}

//Returns NTP clock ID
char* DateTime::NTPclockID(){
  return clck_id;
}

//Returns last NTP error
byte DateTime::NTPlastError(){
  return ntp_err;
}

//Sets NTP interval in seconds (minimal is 120s)
void DateTime::NTPinterval(unsigned int interval){
  if(interval < 120) interval = 120;
  ntp_synch_int = interval;
}

//Returns time in ms which remains to next NTP synchronization process
//Returns negative value when request was sent, but not received yet. (Time to timeout) (timeout is defaultly set to 5s)
//Else returns positve value
long DateTime::remainingNTPsynchTime(){
  if(!ntp_en) return 0;
  if(prepared) return -(long)((ntp_mil+ntp_synch_int * SECOND) - millis());
  else return (ntp_mil+ntp_synch_int * SECOND) - millis();
}

//Enable or disable NTP synchronization
//If synchronization is disabled, this will automatically enable it
void DateTime::NTPenable(bool en){
  if(!synchEN && en)synchEnable(true);
  //if(!ntp_en && en) ntp.begin(localPort);
  ntp_en = en;
}

//Return true if NTP synchronization is enabled
bool DateTime::NTPenabled(){
  return ntp_en;
}


void DateTime::onNTPsynch(void(*callback)(byte)){
  _onNTPsynch = callback;
}

//Sends UDP synchronization request to NTP server
byte DateTime::NTPsynchNow(){
  if(!prepared){
    ntp.begin(localPort);
    //get a random server from the pool
    IPAddress timeServerIP;
    if(WiFi.hostByName(ntp_server_url, timeServerIP) != 1){
      ntp_err = NTP_NOT_FOUND;
      if(_onNTPsynch) _onNTPsynch(ntp_err);
    }
    else{
      sendNTPpacket(timeServerIP); // send an NTP packet to a time server
      prepared = true;
      ntp_mil = millis()+5000; //Setting timeout
    }
  }
  if(prepared){
    int cb = ntp.parsePacket();
    if(!cb){
      if(ntp_mil <= millis()){ //NTP Server timeout
        ntp_err = NTP_TIMEOUT;
        prepared = false;
        ntp.stop();
        ntp_mil = millis() + 5000; //delay 5 seconds before another try
        if(_onNTPsynch) _onNTPsynch(ntp_err);
      }
    }
    else if(cb < 48){
      ntp_err = NTP_BAD_RESPONSE;
      prepared = false;
      ntp.stop();
      ntp_mil = millis() + 5000; //delay 5 seconds before another try
      if(_onNTPsynch) _onNTPsynch(ntp_err);
    }
    else{
      // We've received a packet, read the data from it
      ntp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  
      //the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, extract the two words:
  
      // combine the four bytes (two words) into a long integer
      // this is NTP time (milliseconds since Jan 1 1900):
      int64_t t2 =  ((uint32_t)packetBuffer[40] << 24) | ((uint32_t)packetBuffer[41] << 16) | ((uint32_t)packetBuffer[42] << 8) | packetBuffer[43];
      int ms = ((unsigned long)(((unsigned int)packetBuffer[44]<<2)|((unsigned int)packetBuffer[45]>>6))*1000)/1024; //milliseconds calculated from fraction
      t2 = t2*SECOND + ms;

      //time when packet was received by server
      int64_t t1 = ((uint32_t)packetBuffer[32] << 24) | ((uint32_t)packetBuffer[33] << 16) | ((uint32_t)packetBuffer[34] << 8) | packetBuffer[35];
      ms = ((unsigned long)(((unsigned int)packetBuffer[36]<<2)|((unsigned int)packetBuffer[37]>>6))*1000)/1024; //milliseconds calculated from fraction
      t1 = t1*SECOND + ms;

      if(t1 < 60000 && t2 < 60000){ //Low value means, that something is bad, because we have received time at year 1900.
        ntp_err = NTP_BAD_RESPONSE;
        prepared = false;
        ntp_mil = millis() + 5000; //delay 5 seconds before another try
        if(_onNTPsynch) _onNTPsynch(ntp_err);
        return ntp_err;
      }

      t1 = ((millis() - (ntp_mil - 5000)) - (t2 - t1))/2; //t1 is now used as offset

      //writing clock reference ID to array
      clck_id[0] = (char)packetBuffer[12];
      clck_id[1] = (char)packetBuffer[13];
      clck_id[2] = (char)packetBuffer[14];
      clck_id[3] = (char)packetBuffer[15];
      clck_id[4] = '\0';

      raw(time_base + t2 + t1,true); //assigning a new value to time in UTC with applied offset
      prepared = false;
      ntp_mil = millis() + ntp_synch_int * SECOND;
      ntp_err = NTP_OK;
      ntp.stop();
      if(_onNTPsynch) _onNTPsynch(ntp_err);
    }
  }
  return ntp_err;
}

//NTP handler have to be included in loop
//Returns NTP error
//Warning: FOR BETTER RESULTS DO NOT USE delay() OR delayMicroseconds(), BECAUSE IT CAN CAUSE LONGER RESPONSE
byte DateTime::NTPhandler(){
  bool WiFi_conn = (WiFi.status() == WL_CONNECTED);
  if(synchEN && ntp_en && WiFi_conn){
    if(!prepared && ntp_mil <= millis()){
      ntp.begin(localPort);
      //get a random server from the pool
      IPAddress timeServerIP;
      if(WiFi.hostByName(ntp_server_url, timeServerIP) != 1){
        ntp_err = NTP_NOT_FOUND;
        if(_onNTPsynch) _onNTPsynch(ntp_err);
      }
      else{
        sendNTPpacket(timeServerIP); // send an NTP packet to a time server
        prepared = true;
        ntp_mil = millis()+2500; //Setting timeout
      }
    }
    if (prepared){
      int cb = ntp.parsePacket();
      if(!cb){
        if(ntp_mil <= millis()){ //NTP Server timeout
          ntp_err = NTP_TIMEOUT;
          prepared = false;
          ntp.stop();
          ntp_mil = millis() + 5000; //delay 5 seconds before another try
          if(_onNTPsynch) _onNTPsynch(ntp_err);
        }
      }
      else if(cb < 48){
        ntp_err = NTP_BAD_RESPONSE;
        prepared = false;
        ntp.stop();
        ntp_mil = millis() + 5000; //delay 5 seconds before another try
        if(_onNTPsynch) _onNTPsynch(ntp_err);
      }
      else{
        // We've received a packet, read the data from it
        ntp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    
        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, extract the two words:
    
        // combine the four bytes (two words) into a long integer
        // this is NTP time (milliseconds since Jan 1 1900):
        int64_t t2 =  ((uint32_t)packetBuffer[40] << 24) | ((uint32_t)packetBuffer[41] << 16) | ((uint32_t)packetBuffer[42] << 8) | packetBuffer[43];
        int ms = ((unsigned long)(((unsigned int)packetBuffer[44]<<2)|((unsigned int)packetBuffer[45]>>6))*1000)/1024; //milliseconds calculated from fraction
        t2 = t2*SECOND + ms;

        //time when packet was received by server
        int64_t t1 = ((uint32_t)packetBuffer[32] << 24) | ((uint32_t)packetBuffer[33] << 16) | ((uint32_t)packetBuffer[34] << 8) | packetBuffer[35];
        ms = ((unsigned long)(((unsigned int)packetBuffer[36]<<2)|((unsigned int)packetBuffer[37]>>6))*1000)/1024; //milliseconds calculated from fraction
        t1 = t1*SECOND + ms;

        if(t1 < 60000 || t2 < 60000){ //Low value means, that something is bad, because we have received time at year 1900.
          ntp_err = NTP_BAD_RESPONSE;
          prepared = false;
          ntp_mil = millis() + 5000; //delay 5 seconds before another try
          if(_onNTPsynch) _onNTPsynch(ntp_err);
          return ntp_err;
        }

        t1 = ((millis() - (ntp_mil - 5000)) - (t2 - t1))/2; //t1 is now used as offset

        //writing clock reference ID to array
        clck_id[0] = (char)packetBuffer[12];
        clck_id[1] = (char)packetBuffer[13];
        clck_id[2] = (char)packetBuffer[14];
        clck_id[3] = (char)packetBuffer[15];
        clck_id[4] = '\0';

        raw(time_base + t2 + t1,true); //assigning a new value to time in UTC with applied offset
        prepared = false;
        ntp_mil = millis() + ntp_synch_int * SECOND;
        ntp_err = NTP_OK;
        ntp.stop();
        if(_onNTPsynch) _onNTPsynch(ntp_err);
      }
    }
  }
  else if(!WiFi_conn){
    ntp_err = NTP_NO_INTERNET;
    ntp.stop();
  }
  else{
    ntp_err = NTP_NONE;
    ntp.stop();
  }
  return ntp_err;
}

// send an NTP request to the time server at the given address
void DateTime::sendNTPpacket(IPAddress& address){
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011; // LI(3), Version(4), Mode(3,client)
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval (polling interval = 2^6 = 64sec)
  packetBuffer[3] = 0xEC;  // Peer Clock Precision (precision = 2^(-20) = 0,95367us) (0xEC signed = -20)
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ntp.beginPacket(address, 123); //NTP requests are to port 123
  ntp.write(packetBuffer, NTP_PACKET_SIZE);
  ntp.endPacket();
}

//Returns last http Code from function updateTZ_DST
int DateTime::updateTZ_DST_lastError(){
  return TZDST_err; //returns http Code
}

//This function update Time zone and DST offset. We recommend to use it inside void loop()
//Parameter interval is in milliseconds.
//Returns http Code of response (200 == OK)
//This function takes 100ms - 200ms
int DateTime::updateTZ_DST(unsigned long inter_){
  if(TZDST_mil < millis() || inter_ == 0){
    if(inter_ != 0) TZDST_mil = millis()+inter_;
    long TZ = getTimezone()*3600.00;
    long DST_ = DST()*60;
    TZDST_err = getTzDST(&TZ, &DST_);
    if(TZDST_err == 200){ //OK
      setTimezone(TZ/3600.00);//Set time zone
      DST(true,DST_/60);   //Enable DST and set DST offset
    }
  }
  return TZDST_err; //returns http Code
}

//Gets timezone and DST offsets in seconds from server worldtimeapi.org
//Returns httpCode of received message
int DateTime::getTzDST(long* TZ_offset, long* DST_offset){
  //http.setReuse(false);
  http.begin(F("http://worldtimeapi.org/api/ip"));
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    const String& json = http.getString();
    char raw_offset[] = "\"raw_offset\":";
    char dst_offset[] = "\"dst_offset\":";
    int json_length = json.length();
    int ind = json.indexOf(F("\"raw_offset\":"));
    if(ind >= 0){
      *TZ_offset = 0;
      ind += 13;  //length of searched string
      for(; ind < json_length; ind++){
        if(json.charAt(ind) >= '0' && json.charAt(ind) <= '9'){
          *TZ_offset *= 10;
          *TZ_offset += (json.charAt(ind) - '0');
        }
        else break;
      }
    }

    ind = json.indexOf(F("\"dst_offset\":"));
    if(ind >= 0){
      *DST_offset = 0;
      ind += 13; //length of searched string
      for(; ind < json_length; ind++){
        if(json.charAt(ind) >= '0' && json.charAt(ind) <= '9'){
          *DST_offset *= 10;
          *DST_offset += (json.charAt(ind) - '0');
        }
        else break;
      }
    }
  }
  http.end();
  return httpCode;
}



#endif

//This function immediately synchronize time using millis() method or calling function selected using onSynch() function
//If parameter now is true, function selected using onSynch() function will be always done,
//else it will be done only if synchronization timer expires
void DateTime::synchNow(bool now){
  short hour, minute, second, mil, year, month, day;
  int64_t synch_time = raw_time + (millis()-synch_millis); //synchronization with millis() function
  if(!onSynchUTC) synch_time += (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE; //compensation of non UTC time
  rawToDateTime(&synch_time,&hour,&minute,&second,&mil,&year,&month,&day); //converting raw value to date and time
  if(_onSynch && (millis() > synch_millis+synch_interval*SECOND || synch_millis == 0 || now)){ //if on synch function is set
    //synchronysation
    synch_millis = millis(); //set new interval timer value
    time_s time_; //create new time structure
    time_.hour = hour;
    time_.minute = minute;
    time_.second = second;
    time_.milliseconds = mil;
    time_.year = year;
    time_.month = month;
    time_.day = day;
    time_.weekday = (((daysFromYearZero(year) + dayInYear(day,month,year)) + WD_CALIB) % 7)+1;
    _onSynch(&time_); //calling on synch function
    if(!swr){ //if enabled, user have to use "raw()" function to write time in raw form (for NTP servers)
      //convert from time structure into raw value
      hour = time_.hour;
      minute = time_.minute;
      second = time_.second;
      mil = time_.milliseconds;
      year = time_.year;
      month = time_.month;
      day = time_.day;
      dateTimeToRaw(&raw_time,hour,minute,second,mil,year,month,day);
    }
    if(!onSynchUTC) raw_time = raw_time - (long)(HOUR_IN_MILLIS*timezone) - shift*MINUTE; //write this time in UTC
  }
}


//Synchronize time and then return it
void DateTime::readSynchTime(short *hour, short *minute, short *second, short *mil, short *year, short *month, short *day, bool UTC){
  //Synchronization
  if(synchEN){
    int64_t synch_time = raw_time + (millis()-synch_millis); //synchronization with millis() function
    if(!onSynchUTC) synch_time += (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE; //compensation of non UTC time
    rawToDateTime(&synch_time,hour,minute,second,mil,year,month,day); //converting raw value to date and time
    if(_onSynch && (millis() > synch_millis+synch_interval*SECOND || synch_millis == 0)){ //if on synch function is set
      //need synchronization
      synch_millis = millis(); //set new interval timer value
      time_s time_; //create new time structure
      time_.hour = *hour;
      time_.minute = *minute;
      time_.second = *second;
      time_.milliseconds = *mil;
      time_.year = *year;
      time_.month = *month;
      time_.day = *day;
      time_.weekday = (((daysFromYearZero(*year) + dayInYear(*day,*month,*year)) + WD_CALIB) % 7)+1;
      _onSynch(&time_); //calling on synch function
      if(!swr){ //if enabled, user have to use "raw()" function to write time in raw form (for NTP servers)
        //convert from time structure into raw value
        *hour = time_.hour;
        *minute = time_.minute;
        *second = time_.second;
        *mil = time_.milliseconds;
        *year = time_.year;
        *month = time_.month;
        *day = time_.day;
        dateTimeToRaw(&raw_time,*hour,*minute,*second,*mil,*year,*month,*day);
      }
      if(!onSynchUTC) raw_time = raw_time - (long)(HOUR_IN_MILLIS*timezone) - shift*MINUTE; //write this time in UTC
    }
  }
  
  //Read data from raw value only if needed and compensate non UTC or UTC time
  if(!synchEN || (UTC && !onSynchUTC) || (!UTC && onSynchUTC)){
    if(UTC) rawToDateTime(&raw_time,hour,minute,second,mil,year,month,day); //data in UTC
    else{
      int64_t synch_time = raw_time + (long)(HOUR_IN_MILLIS*timezone) + shift*MINUTE;
      rawToDateTime(&synch_time,hour,minute,second,mil,year,month,day);
    }
  }
}

//Returns or write selected value from DateTime
//First parameter is value to write (use null_time to read only)
//Second parameter is pointing to which value will be read
//Third parameter sets if value will be in UTC time
short DateTime::getWriteOne(short value, byte variable, bool UTC){
  short hour,minute,second,mil,year,month,day;
  readSynchTime(&hour,&minute,&second,&mil,&year,&month,&day, UTC);
  short ret = null_time;
  //Reading
  switch(variable){
    case GWO_HOUR: hour = constrain(value,0,23); ret = hour; break;
    case GWO_MINUTE: minute = constrain(value,0,59); ret = minute; break;
    case GWO_SECOND: second = constrain(value,0,59); ret = second; break;
    case GWO_MILLIS: mil = constrain(value,0,999); ret = mil; break;
    case GWO_YEAR: if(year == 0) year = 1; /*year = value;*/ ret = year; break;
    case GWO_MONTH: month = constrain(value,1,12); ret = month; break;
    case GWO_DAY: day = constrain(value,1,daysInMonth(month,year)); ret = day; break;
    case GWO_WEEKDAY: return (((daysFromYearZero(year) + dayInYear(day,month,year)) + WD_CALIB) % 7)+1;
  }
  //Writing
  if(value != null_time) dateTimeToRaw(&raw_time, hour, minute, second, mil, year, month, day);
  return ret;
}

//Converts DateTime values to raw value
void DateTime::dateTimeToRaw(int64_t *mil_out, short hour, short minute, short second, short mil, short year, short month, short day){
  *mil_out = 0;
  if(mil >= 0 && mil < SECOND) *mil_out = mil;
  if(second >= 0 && second < 60) *mil_out += (uint16_t)second*SECOND;
  if(minute >= 0 && minute < 60) *mil_out += (uint32_t)minute*MINUTE;
  if(hour >= 0 && hour < 24) *mil_out += (int64_t)hour*HOUR;
  if(day > 0 && month > 0)*mil_out += (int64_t)(dayInYear(day,month,year)-1)*MILLIS_IN_DAY;
  if(year != null_time) *mil_out += (int64_t)daysFromYearZero(year)*MILLIS_IN_DAY;
}

//Converts raw value to DateTime values
void DateTime::rawToDateTime(int64_t *mil_in, short *hour, short *minute, short *second, short *mil, short *year, short *month, short *day){
  if(*year != null_time){
    long days = 0;
    yearFromMillis(*mil_in,*year,days);
    byte i = 1;
    byte dim; //days in month
    while((dim = daysInMonth(i,*year)) < days && i <= 12){
      days -= dim;
      i++;
    }
    if(*month != null_time) *month = i;
    if(*day != null_time) *day = days;
  }

  unsigned long mil_in_day = *mil_in%DAY;
  if(*mil_in < 0) mil_in_day = DAY-abs(mil_in_day);
  if(*mil != null_time) *mil = mil_in_day%SECOND; //milliseconds
  if(*second != null_time) *second = (mil_in_day%MINUTE)/SECOND;
  if(*minute != null_time) *minute = (mil_in_day%HOUR)/MINUTE;
  if(*hour != null_time) *hour = mil_in_day/HOUR;
}
