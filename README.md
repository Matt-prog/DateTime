### DateTime library
Powerful library which helps you with time processing on all arduino boards including special functions designed for ESP8266 that keeps you time synchronized. There are a lot of ways to use this library. You can use date and time "variable", get difference between dates and times, set alarm or set millistimer. Main advantage of this library is the wide date range (from 1.1.32767 BC to 31.12.32767) including leap years, also millisecond time accuracy.  

## DateTime
Object DateTime is like date and time variable. It can save time, date, time zone, DST and more, but this is not all. You can make it more dynamic using synchronization by milliseconds timer or NTP server (works only on ESP8266). There are lots of others functions or operators that are documented bellow.

**Constructors:**

`DateTime();` Creates new DateTime object with default value 1.1.0001 00:00:00:000.

`DateTime(int64_t mil);` Creates new DateTime object using UTC raw value in milliseconds.
> Raw DateTime value is signed 8-bytes long number (int64_t) represented as the number of milliseconds elapsed since 1.1.0001 00:00:00:000.

`DateTime(hour, minute, second, milliseconds, year, month, day);`Creates new DateTime object from time and date. This constructor have to contain at least first three arguments.
> To insert year before Christ just write negative year value. For example: 2000BC write as -2000

> Warning: Year 0 does not exist! After year 1BC is 1AD.

**Functions:**

`void raw(raw_value, UTC);` Sets new raw DateTime value. If argument UTC is false, inserted raw DateTime value will be to UTC using set time zone and DST offset.

`int64_t raw(UTC);` Returns raw DateTime value in UTC or not.

`millisecondsUTC(millliseconds);` Sets milliseconds of DateTime in UTC or return set milliseconds (without argument) in UTC.

`secondUTC(seconds);` Sets seconds of DateTime in UTC or return set seconds (without argument) in UTC.



This is not all we are working on this README file
