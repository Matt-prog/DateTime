/* This simple example shows how to use advanced functions of DateTime.
 */

#include <DateTime.h>

DateTime time1; //New date time variable
DateTime time_synch(5,0,0,100,2019,9,11); //New date time variable set to 5:00:00:100 11.09.2019

void setup(){
  Serial.begin(115200); //Start Serial communication

  time1(5,0,0,100,2019,9,11); //Set DateTime variable to 5:00:00:100 11.09.2019 (this is an advanced way)
  //You can also use function "time1.set()"

  time_synch.synchEnable(true); //Enable synchronization
}

void loop(){
  Serial.println("Time1:");
  //We prefer to use this type of getting every DateTime value only if synchronization is disabled,
  //because time can change after each "get" function
  //You can also use those functions to set time, for example: time1.hour(2) or if 12-hour format is enabled: time1.hour(2,PM);
  //To print value in UTC use, for example: time1.hourUTC()
  Serial.print(time1.hour());
  Serial.print(":");
  Serial.print(time1.minute());
  Serial.print(":");
  Serial.print(time1.second());
  Serial.print(":");
  Serial.print(time1.milliseconds());
  Serial.print(" ");
  Serial.print(time1.day());
  Serial.print(".");
  Serial.print(time1.month());
  Serial.print(".");
  Serial.print(time1.year());
  Serial.print(" ");
  Serial.println(weekdayToString(time1.weekday()));
  Serial.print("Raw: ");
  Serial.print(toStr64(time1)); //Print raw value, which is represented by milliseconds that elapsed from time 00:00:00:000 01.01.0001
  //Raw value type is int64_t, but function "print()" can print maximally int32_t (long), that is why we have to use function "toStr64()" that convert this number to String 
  Serial.println("ms");
  Serial.print("Days from date 01.01.0001: ");
  Serial.println(time1.days()); //Print how many days elapsed from date 01.01.0001
  //You can also use time1.daysUTC() to print days in UTC

  time1 += DAY + HOUR + MINUTE + SECOND + MILLISECOND; //add some time
  //By advanced function "time1.operatorsUseUTC(bool useUTC)" you can set, if it will calculate with UTC time or not.

  Serial.println("Synchronized time:");
  //This method of getting time is the safest and faster, but use more memory in RAM:
  time_s ts = time_synch.get();
  Serial.print(ts.hour);
  Serial.print(":");
  Serial.print(ts.minute);
  Serial.print(":");
  Serial.print(ts.second);
  Serial.print(":");
  Serial.print(ts.milliseconds);
  Serial.print(" ");
  Serial.print(ts.day);
  Serial.print(".");
  Serial.print(ts.month);
  Serial.print(".");
  Serial.print(ts.year);
  Serial.print(" ");
  Serial.println(weekdayToString(ts.weekday));
  Serial.print("Raw: ");
  Serial.print(toStr64(time_synch.raw())); //This also prints raw value, but you can use function time_synch.raw(true) to print value in UTC.
  Serial.println("ms");
  
  Serial.println();
}

//This function converts day of week to String
String weekdayToString(byte wd){
  switch(wd){
    case WD_SUNDAY: return "Sunday";
    case WD_MONDAY: return "Monday";
    case WD_TUESDAY: return "Tuesday";
    case WD_WEDNESDAY: return "Wednesday";
    case WD_THURSDAY: return "Thursday";
    case WD_FRIDAY: return "Friday";
    case WD_SATURDAY: return "Saturday";
    default: return "Unknown";
  }
}

