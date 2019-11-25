### DateTime library
Powerful library which helps you with time processing on all arduino boards including special functions designed for ESP8266 that keeps you time synchronized. There are a lot of ways to use this library. You can use date and time "variable", get difference between dates and times, set alarm or set millistimer. Main advantage of this library is the wide date range (from 1.1.32767 BC to 31.12.32767) including leap years, also millisecond time accuracy.  

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

`weekdayUTC(day_of_week);` Sets day of week of DateTime in UTC or returns set day of week (without argument) in UTC. Starts at sunday (1), the next is monday (2) or just use enums (WD_SUNDAY, WD_MONDAY,...)


`milliseconds(millliseconds);` Sets milliseconds of DateTime or returns set milliseconds (without argument).

`second(seconds);` Sets seconds of DateTime or returnz set seconds (without argument).

`minute(minutes);` Sets minutes of DateTime or returns set minutes (without argument).

`hour(hours);` Sets hours of DateTime or returns set hours (without argument).

`year(year);` Sets year of DateTime or returns set year (without argument).

`month(month);` Sets month of DateTime or returns set month (without argument).

`day(day);` Sets day of DateTime or returns set day (without argument).

`weekday(day_of_week);` Sets day of week of DateTime or returns set day of week (without argument). Starts at sunday (1), the next is monday (2) or just use enums (`WD_SUNDAY`, `WD_MONDAY`,...)


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



This is not all we are working on this README file
