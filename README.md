# DateTime library

Powerful library which helps you with time processing on all arduino boards including special functions designed for ESP8266 that keeps you time synchronized. There are a lot of ways to use this library. You can use date and time "variable", get difference between dates and times, set alarm or set millistimer instead of delay function. Main advantage of this library is the wide date range (from 1.1.32767 BC to 31.12.32767) including leap years with millisecond time accuracy and fast calculations, because this library uses special math algorithms to count with leap years without cycles.  

## DateTime
Object DateTime is like date and time variable. It can save time, date, time zone, DST and more, but this is not all. You can make it more dynamic using synchronization by milliseconds timer or NTP server (works only on ESP8266). There are lots of others functions or operators that are documented bellow.

**Constructors:**

`DateTime();` Creates new DateTime object with default value 1.1.0001 00:00:00:000.

`DateTime(raw_value);` Creates new DateTime object using UTC raw value in milliseconds.
> Raw DateTime value is signed 8-bytes long number (int64_t) represented as the number of milliseconds elapsed since 1.1.0001 00:00:00:000.

`DateTime(hour, minute, second, milliseconds, year, month, day);`Creates new DateTime object from time and date. This constructor have to contain at least first three arguments.
> To insert year before Christ just write negative year value. For example: 2000BC write as -2000

> Warning: Year 0 does not exist! After year 1BC is 1AD.

**Functions:**

`void raw(raw_value, UTC);` Sets new raw DateTime value. If argument UTC is false, inserted raw DateTime value will be to UTC using set time zone and DST offset.

`int64_t raw(UTC);` Returns raw DateTime value in UTC or not.

`millisecondsUTC(millliseconds);` Sets milliseconds of DateTime in UTC or returns set milliseconds (without argument) in UTC.

`secondUTC(seconds);` Sets seconds of DateTime in UTC or returnz set seconds (without argument) in UTC.

`minuteUTC(minutes);` Sets minutes of DateTime in UTC or returns set minutes (without argument) in UTC.

`hourUTC(hours);` Sets hours of DateTime in UTC or returns set hours (without argument) in UTC.

`yearUTC(year);` Sets year of DateTime in UTC or returns set year (without argument) in UTC.

`monthUTC(month);` Sets month of DateTime in UTC or returns set month (without argument) in UTC.

`dayUTC(day);` Sets day of DateTime in UTC or returns set day (without argument) in UTC.

`weekdayUTC();` Returns day of week in UTC. Starts at sunday (1), the next is monday (2) or just use enums (WD_SUNDAY, WD_MONDAY,...)


`milliseconds(millliseconds);` Sets milliseconds of DateTime or returns set milliseconds (without argument).

`second(seconds);` Sets seconds of DateTime or returnz set seconds (without argument).

`minute(minutes);` Sets minutes of DateTime or returns set minutes (without argument).

`hour(hours);` Sets hours of DateTime or returns set hours (without argument).

`year(year);` Sets year of DateTime or returns set year (without argument).

`month(month);` Sets month of DateTime or returns set month (without argument).

`day(day);` Sets day of DateTime or returns set day (without argument).

`weekday(day_of_week);` Returns day of week. Starts at sunday (1), the next is monday (2) or just use enums (`WD_SUNDAY`, `WD_MONDAY`,...)


`setTimezone(timezone);` Sets time zone offset in hours. After changing time zone, result value of DateTime will change too.

`getTimezone();` Returns set time zone offset in hours.

`DST(enable, offset);` Sets DST offset in minutes(some countries have offset 10 or 30 minutes) or returns set DST offset in minutes (without arguments) (returns 0 if DST is disabled). This function cannot determine when DST is changing, because it does not have to be same in different countries, so you have to do it manually. On ESP8266 you can use function `updateTZ_DST()` to automatically set time zone and DST due to your location.

`isDST();` Returns true if DST is enabled.

`format(hour_format);` Sets 12-hour or 24-hour format or returns hour format (without argument). We recommended to use enums: `FORMAT_12HOUR` or `FORMAT_24HOUR`.

`isAM();` Returns true if time is an a.m.

`isPM();` Returns true if time is a p.m.

`toLongTimeString(format, separator, UTC);` Converts DateTime to LONG time String in special format (use enums: `H_M_S`, `HH_MM_SS_mmm`,... to define format). Separator is char, that is inserted between two values. Set UTC to true if you want to print time in UTC.

`toShortTimeString(format, separator, UTC);` Converts DateTime to SHORT time String. Arguments are same as in function above.

`toLongDateString(format, separator, UTC);` Converts DateTime to LONG date String in special format (use enums: `DD_MM_YYYY`, `YY_D_M`,... to define format). Separator is char, that is inserted between two values. Set UTC to true if you want to print time in UTC.

`toShortDateString(format, separator, UTC);` Converts DateTime to SHORT date String. Arguments are same as in function above.

`synchEnable(enable);` Enables or disables synchronization.

`synchEnabled();` Returns true if synchronization is enabled.

`synchInterval(interval);` Synch_function is called after this interval in seconds, but synchronization using milliseconds timer (millis()) still works. That mean that you do not have to set this interval if you want to only use milliseconds timer and no external synch_function.

`remainingSynchTime();` Returns remaining time to next synchronization in milliseconds.

`onSynch(synch_function, UTC, write_raw)` Using this function you can set synch_function, that will be called during synchronization after synchInterval. If argument UTC is true, then new DateTime value in synch_function will be in UTC, else not. If argument write_raw is true, then you need to use `raw()` function to update time, else you need to fill time_s(time structure) inside of synch_function.

`synchNow(now);` This function synchronize time using milliseconds timer immediately, but synch_function is called only if synchInterval expired or if argument now is true (default is false).


`operatorsUseUTC(enable);` Sets or returns true if operators will use UTC value of DateTime.


`setUTC(time_s time);` Sets UTC time from time_s (time structure).

`setUTC(hour, minute, second, milliseconds, year, month, day);` Sets time or date and time in UTC. This function have to contain at least first three arguments.

`setDateUTC(hour, minute, second, milliseconds, year, month, day);` Sets date in UTC. This function have to contain year, other arguments are optional.

`set(time_s time);` Sets time from time_s (time structure).

`set(hour, minute, second, milliseconds, year, month, day);` Sets time or date and time. This function have to contain at least first three arguments.

`setDate(hour, minute, second, milliseconds, year, month, day);` Sets date only. This function have to contain year, other arguments are optional.


`getUTC();` Returns time_s (time structure) in UTC.

```
getUTC(short *hour, short *minute, short *second, short *milliseconds, short *year, short *month, short *day);
getUTC(short *hour, short *minute, short *second, short *milliseconds);
getUTC(short *hour, short *minute, short *second);
```
These three functions will fill your variables (have to be short) with time in UTC. To pass argument use & pointer before variable.

`getDateUTC(short *year, short *month, short *day);` This function will fill your variables (short) with date in UTC. To pass argument use & pointer before variable.

`get();` Returns time_s (time structure).

```
get(short *hour, short *minute, short *second, short *milliseconds, short *year, short *month, short *day);
get(short *hour, short *minute, short *second, short *milliseconds);
get(short *hour, short *minute, short *second);
```
These three functions will fill your variables (have to be short) with time. To pass argument use & pointer before variable.

`getDate(short *year, short *month, short *day);` This function will fill your variables (short) with date. To pass argument use & pointer before variable.

`daysUTC();` Returns count of days since 1.1.0001 00:00:00:000 in UTC.

`days();` Returns count of days since 1.1.0001 00:00:00:000.

**ESP8266 only functions:**

`setNTPserver(URL);` Sets NTP server address.

`NTPclockID();` Returns received NTP clock ID reference.

`NTPlastError();` Returns last error that appear during NTP packet receivig. Use NTP error enums: `NTP_NONE`, `NTP_OK`, `NTP_NOT_FOUND`, `NTP_TIMEOUT`, `NTP_BAD_RESPONSE`, `NTP_NO_INTERNET`.

`NTPinterval(interval);` Sets interval of NTP synchronization in seconds. (Minimal interval is 120 seconds).

`NTPenable(enable);` Enables NTP synchronization. You do not have to call `synchEnable()`, because it will be called inside automatically, but when you are disabling NTP synchronization, you have to call it to disable milliseconds timer synchronization.

`NTPenabled();` Returns true if NTP synchronization is enabled.

`NTPsynchNow();` Immediately sends NTP packet request.

`remainingNTPsynchTime();` Returns remaining time to next NTP synchronization. Returns negative value when request was sent, but response was not received yet (remaining time to timeout, timeout is 5 seconds). Returns positive value when last synchronization has finished and the device is waiting to next synchronization.

`NTPhandler();` This function have to be included in loop when you want to use NTP synchronization.
>Do not use delay in your code if you want to use NTP synchronization, because you can get unaccurate time results.

`onNTPsynch(after_NTP_function);` Sets function which is done after NTP synchronization or receiving error. After_NTP_function has one parameter, that returns current NTP error.

`updateTZ_DST();` Automatically updates time zone and DST offset due to your public IP address using server worldtimeapi.org.

`updateTZ_DST_lastError();` Returns last error (http code) that was received from server.


**time_s or time structure:**

Variables:
```
hour
minute
second
milliseconds
year
month
day
weekday
```
In most applications is weekday "read only", because all functions in this library will compute day of week feom other values.

**Operators:**

```
DateTime dt;
TimeSpan ts;

dt(); //returns raw DateTime value

dt(hour, minute, second, milliseconds, year, month, day); //sets new date and time

dt = raw_value; //assigns raw DateTime value

dt = dt + raw_value;
dt = dt - raw_value;
dt = dt + ts; //TimeSpan
dt = dt - ts; //TimeSpan
dt = dt - dt;
dt = dt + dt; //mostly unused function
//You can also use += or -= operator

//comparison operators:
dt == dt;
dt == raw_value;
dt > dt;
dt > raw_value;
dt < dt;
dt < raw_value;
dt >= dt;
dt >= raw_value;
dt <= dt;
dt <= raw_value;
```

You can also use these enums with operators:
```
MILLISECOND
SECOND
MINUTE
HOUR
DAY
WEEK
MONTH_31
MONTH_30 or MONTH
MONTH_29
MONTH_28
YEAR
LEAP_YEAR
```

## TimeSpan

TimeSpan is special variable, that can hold time difference in milliseconds.

**Constructors:**

`TimeSpan();` Creates null TimeSpan.

`TimeSpan(raw_value);` Creates TimeSpan from raw_value.

`TimeSpan(days, hours, minutes, seconds, milliseconds);` Creates new TimeSpan variable. This constructor have to contain only first argument, others are optional.

**Functions:**

`set(days, hours, minutes, seconds, milliseconds);` Sets new values of TimeSpan. This function have to contain only first argument, others are optional.

```
get(long *days, long *hours, long *minutes, long *seconds, long *milliseconds);
get(long *days, long *hours, long *minutes, long *seconds);
get(long *days, long *hours, long *minutes);
get(long *days, long *hours);
get(long *days);
```
These five functions will fill your variables (have to be long) with time. To pass argument use & pointer before variable.

`days(days_);` Sets days of TimeSpan or returns set days (without argument).

`hours(hours_);` Sets hours of TimeSpan or returns set hours (without argument).

`minutes(minutes_);` Sets minutes of TimeSpan or returns set minutes (without argument).

`seconds(seconds_);` Sets seconds of TimeSpan or returns set seconds (without argument).

`milliseconds(milliseconds_);` Sets milliseconds of TimeSpan or returns set milliseconds (without argument).

`raw(raw_value);` Sets or returns raw TimeSpan value. Argument UTC is missing because we do not need it here.

**Operators:**

```
DateTime dt;
TimeSpan ts;

ts(); //returns raw TimeSpan value

ts(days, hours, minutes, seconds, milliseconds); //sets new TimeSpan values

ts = raw_value; //assigns raw TimeSpan value

ts = ts + raw_value;
ts = ts - raw_value;
ts = ts + ts;
ts = ts - ts;
ts = dt + ts;
ts = dt - ts;
//You can also use += or -= operator

//comparison operators:
ts == ts;
ts == raw_value;
ts > ts;
ts > raw_value;
ts < ts;
ts < raw_value;
ts >= ts;
ts >= raw_value;
ts <= ts;
ts <= raw_value;
```

## Alarm

**Constructors:**

`Alarm();` Creates null Alarm object.

`Alarm(DateTime *dt);` Creates Alarm object with alarm set at time and date from variable DateTime. To pass argument use & pointer before variable. If you change DateTime after this assignment, alarm setting will not change.

`Alarm(time_s tim);` Creates Alarm object with alarm set at time and date from time structure (time_s).

`Alarm(hour, minute, second, mil, year, month, day);` Creates Alarm object with alarm set at time and date. This constructor have to contain at least first two arguments.

**Functions:**

`setAlarm(DateTime *dt);` Sets alarm at time and date from variable DateTime. To pass argument use & pointer before variable. If you change DateTime after this assignment, alarm setting will not change.

`setAlarm(time_s *tim);` Sets alarm at time and date from time structure (time_s).

`setAlarm(hour, minute, second, mil, year, month, day);` Sets alarm at time and date. This function have to contain at least first two arguments.

`getAlarm(DateTime *dt);` Writes alarm set time and date to DateTime. To pass argument use & pointer before variable.

`getAlarm();` Returns alarm set time and date as time structure (time_s).

```
getAlarm(short *hour, short *minute, short *second, short *mil, short *year, short *month, short *day);
getAlarm(short *hour, short *minute, short *second, short *mil);
getAlarm(short *hour, short *minute, short *second);
getAlarm(short *hour, short *minute);
```
These four functions will fill your variables (have to be short) with time and date of alarm setting. To pass argument use & pointer before variable.

`onDays(byte *days, count);` Sets days of week, when alarm can ring. This setting is usable only when repeating is enabled. First paramenter is byte array of days, for example: `byte days[] = {WD_SUNDAY, WD_FRIDAY};`. Second parameter is count of days in this array. If you change this array after this assignment, set days will change too.

`onDays();` This function without any parameter clear this setting (sets that every day in week is available).

`enable(enable);` Enables or disables alarm.

`enabled();` Returns true when alarm is enabled;

`repeat(enable);` Sets if alarm will set new date (from available days of week which you set in function `onDays()`) after ringing or if it disables itself. This function without argument returns if repeating is enabled.

`resetProtection();` Resets alarm protection timer.
>Alarm protection timer interval is set to aproximmatelly 23 hours after the last ring. This is very necessary, because when you have your alarm synchronized only with time (not date), and after ringing time will move back (because of unaccurate time synchronization) or time will be checked more times in just one second, it can cause, that alarm will rings two times in a row. But when you change DST or time zone, we recommended to use function above to reset this protection.

>Alarm protection works only when you use third method of time synchronizing (or second method, but with null date values) and you did not fill date parameters or when you use function `setAlarm(hour, minute, second, mil, year, month, day)` without date parameters.

`onRinging(function);` Sets function that is called when alarm rings.

**Three methods how to synchronize alarm clock with real time:**

1. `setSynch(DateTime *dt);` Sets "clock" with real time. Here it is synchronized DateTime. We recommend this type of synchronization, because it is the most accurate method. To pass argument use & pointer before variable.

2. `setSynch(time_s *tim);` Sets "clock" with real time. Here it is time structure (time_s), that is extarnally synchronized with real time as often as possible. To pass argument use & pointer before variable.

3. Third way how to synchronize time is to write it directly to handler `handleAlarm()`, but if you have used first or second sync method, you have to use `resetSynch();` to reset synch setting. This method is documented bellow.

`handleAlarm(hour, minute, second, mil, year, month, day);` This function have to be included in loop when you want to use Alarm. You have to use this function with aguments (first two are required, others are optional) when you have chosen third method of time synchronization. Arguments have to contain real time values. This function returns true when everything is set correctly.

`handleAlarm();` This function have to be included in loop only when you are using first or second method of time sync.

##MillisTimer

**Constructors:**

`MillisTimer();` Creates null millis timer.

`MillisTimer(millis_interval);` Creates millis timer and sets interval in milliseconds.

**Functions:**

`setInterval(millis_interval);` Sets interval of millis timer in milliseconds

`getInterval();` Returns interval of millis timer in milliseconds.

`remainingTime();` Returns remaining time until millis timer expires in milliseconds.

`onHandle(function);` Sets function that is done after time expires.

`handleTime();` This function have to be included in loop.

`reset();` Resets millis timer with last set interval.

`enable(enable);` Enables millis timer. When timer was disabled and now you are enabling it, it will automatically resets.

`enabled();` Returns true when millis timer is enabled.

`repeat(enable);` Sets if millis timer will resets after expiration. When it is called without any parameter, it will return true if repeating is enabled.

## You should to know

Function that is called on DateTime synch have to look like this:
```
DateTime dt;
dt.onSynch(DTsynch, false, false); //or just use onSynch(DTsynch);

void DTsynch(time_s *old_time){
  old_time->hour = new_hour;
  old_time->minute = new_minute;
  //...
}
```

Function that is called on Alarm ringing have to look like this:
```
Alarm al;
al.onRinging(Alarm_ring);

void Alarm_ring(time_s now){
  //structure now is time right now, when Alarm is ringing
  Serial.print(now.second);
  //...
}
```

- Function that is called on MillisTimer expiration have to look like this:
```
MillisTimer mt;
mt.onHandle(onTimer);

void onTimer(){
  Serial.println("Timer expired!");
}
```

All available formats that are used when DateTime is converted to String (for example: `toLongString()`,...):
For date:
```
DD_MM_YYYY  //08.06.2019
DD_M_YYYY   //08.6.2019
D_MM_YYYY   //8.06.2019
D_M_YYYY    //8.6.2019
MM_YYYY     //06.2019
M_YYYY      //6.2019
YYYY_MM_DD  //2019.06.08
YYYY_M_DD   //2019.6.08
YYYY_MM_D   //2019.06.8
YYYY_M_D    //2019.6.8
YYYY_MM     //2019.06
YYYY_M      //2019.6
DD_MM_YY    //08.06.19
DD_M_YY     //08.6.19
D_MM_YY     //8.06.19
D_M_YY      //8.6.19
MM_YY       //06.19
M_YY        //6.19
YY_MM_DD    //19.06.08
YY_M_DD     //19.6.08
YY_MM_D     //19.06.8
YY_M_D      //19.6.8
YY_MM       //19.06
YY_M        //19.6
DD_MM       //08.06
DD_M        //08.6
D_MM        //8.06
D_M         //8.6
MM_DD       //06.08
M_DD        //6.08
MM_D        //06.8
M_D         //6.8
```

For time:
```
HH_MM_SS_mmm  //09:05:03:054
H_M_S_m       //9:5:3:54
HH_MM_SS      //09:05:03
H_M_S         //9:5:3
HH_MM         //09:05
H_M           //9:5
M_S           //5:3
MM_SS         //05:03
M_S_m         //5:3:54
MM_SS_mmm     //05:03:054
S_m           //3:54
SS_mmm        //03:054
```

`null_time` can be used on variables hour,minute,second,milliseconds,year,month,day with function `set()`. It says, that this value will be ignored and will not be replaced by new value.

Raw DateTime value cannot be printed or converted to String, because it is uint64_t. But you can use `String toStr64(uint64_t num);` that converts uint64_t to String.

Not all functions was documented here, if you want to know more info or functions, you should to read DateTime.cpp file.
