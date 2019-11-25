### DateTime library
Powerful library which helps you with time processing on all arduino boards including special functions designed for ESP8266 that keeps you time synchronized. There are a lot of ways to use this library. You can use date and time "variable", get difference between dates and times, set alarm or set millistimer. Main advantage of this library is the wide date range (from 1.1.32767 BC to 31.12.32767) including leap years, also millisecond time accuracy.  

## DateTime
Object DateTime is like date and time variable. It can save time, date, time zone, DST and more, but this is not all. You can make it more dynamic using synchronization by milliseconds timer or NTP server (works only on ESP8266). There are lots of others functions or operators that are documented bellow.

**Constructors:**

`DateTime();` Creates new DateTime object with default value 1.1.0001 00:00:00:000.

`DateTime(int64_t mil);` Creates new DateTime object using UTC raw value in milliseconds.
> Raw DateTime value is signed 8-bytes long number (int64_t) in milliseconds that is counted from 1.1.0001 00:00:00:000.

**Functions:**


This is not all we are working on this README file
