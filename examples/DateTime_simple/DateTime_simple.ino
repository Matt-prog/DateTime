/* This simple example shows how to use basic DateTime functions and operators.
 * It also shows how to use TimeSpan which is "difference" between two DateTimes.
 */
#include <DateTime.h>

MillisTimer timer; //New timer instance

DateTime time1(13,0,35); //New date time variable set to 13:00:35:000 01.01.0001
DateTime time2(8,23,20,500); //New date time variable set to 8:23:20:500 01.01.0001
DateTime time3(5,0,0,100,2019,9,11); //New date time variable set to 5:00:00:100 11.09.2019

//Variable TimeSpan store the difference between two DateTime variables
TimeSpan span(2); //New time span with value of 2 days

void setup(){
  Serial.begin(115200); //Start Serial communication
  
  timer.setInterval(2500);  //Set interval to timer
  timer.enable(true);       //Enable timer
  timer.repeat(true);       //Set timer repeating
  timer.onHandle(onTimer);  //Function which is done when timer expired

  time1.synchEnable(true);  //Enable synchronization by function millis()
  time1.setTimezone(-3);    //Set timezone to GMT-3
  //Function DST() cannot determine when it have to be enabled or not, you should to do it manually
  time1.DST(true);          //Enable DST with default 1 hour shift
  
  time2.synchEnable(true);  //Enable synchronization
  time2.format(FORMAT_12HOUR);  //Set 12 hour format
  //To get a.m. or p.m. only use function "time2.isAM()" or "time1.isPM()"
  
  time3.DST(true,180);          //Enable DST with default 2 hour shift (in minutes)
}

void loop(){
  timer.handleTime(); //this has to be included in loop
}

//Function which is done when timer expired
void onTimer(){
  //Print synchronized time
  Serial.print(F("time1: "));
  Serial.print(time1.toShortTimeString(H_M_S_m));
  Serial.print(F(" "));
  Serial.println(time1.toShortDateString(DD_MM_YYYY));

  //Print synchronized time in 12 hour format
  Serial.print(F("time2: "));
  Serial.print(time2.toLongTimeString(HH_MM_SS_mmm));
  Serial.print(F(" "));
  Serial.println(time2.toLongDateString(D_M_YY));

  //Print static time
  Serial.print(F("time3: "));
  Serial.print(time3.toLongTimeString(HH_MM_SS_mmm));
  Serial.print(F(" "));
  Serial.println(time3.toLongDateString(DD_MM_YYYY));
  time3 += span; //Add 2 days
  //Another way how to add 2 days is:
  //time3 += DAY*2

  Serial.println("----------------");
}

