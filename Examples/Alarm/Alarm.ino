/* This simple example shows how to use Alarm with DateTime.
*/

#include <DateTime.h>

Alarm alarm1; //New alarm instance
//You can set alarm like this:
//Alarm alarm1(22,0,0,0,2019,9,14);

//Or using DateTime:
//DateTime alarm_set(22,0,30,0,2019,9,14);

//Or using time structure:
time_s al; //DateTime structure

DateTime time1(22,0,0,0,2019,9,14); //Present time value

MillisTimer timer = MillisTimer(); //Timer, which will print current time every second

void setup() {
  Serial.begin(115200);

  /*---Setting Time counter (DateTime)---*/
  time1.synchEnable(true); //Enable synchronization

  //Fill data in time structure
  al.hour = 22;
  al.minute = 0;
  al.second = 1;
  al.milliseconds = 0;
  al.year = 2019;
  al.month = 9;
  al.day = 14;
  //If you want to remove for example year value, simply use:
  //al.year = null_time;
  //You don't have to fill weekday, it will be automatically determined
  
  /*---Setting Alarm---*/

  //Set alarm at specific time:
  //alarm1.setAlarm(22,0,0,0,2019,9,14);  //Set alarm manually (hour,minute,second,millisecond,year,month,day)
  //alarm1.setAlarm(&alarm_set);          //Set alarm using DateTime variable
  alarm1.setAlarm(&al);                   //Set alarm using time structure
  //You don't have to fill weekday, it will be automatically determined
  
  alarm1.onRinging(ring);   //Set function which will be done after alarm rings
  alarm1.enable();          //Enable alarm
  alarm1.repeat(true);      //Enable alarm repeating (day will be automatically increased after ring)
  //When repeating is disabled Alarm will be disabled after ringing

  //Add clock to Alarm:
  //alarm1.setSynch(&your_time_structure);  //Set pointer to time structure, which will be manually synchronized
  //You can also update time in Alarm manually in function: alarm1.handleAlarm(hour,minute,second,millisecond,year,month,day);
  alarm1.setSynch(&time1);                  //Set pointer to synchronized DateTime variable

  byte alarm_days[] = {WD_SATURDAY,WD_MONDAY,WD_TUESDAY}; //Array of available days
  alarm1.onDays(alarm_days,3);//Set days when alarm can ring

  /*---Setting Timer (MillisTimer)---*/
  timer.setInterval(1000);      //Set interval to timer
  timer.enable(true);           //Enable timer
  timer.repeat(true);           //Set timer repeating
  timer.onHandle(everySecond);  //Function which is done when timer expired
}

void loop() {
  //Those functions have to be included in loop
  alarm1.handleAlarm();
  timer.handleTime();
}

//Function which will be done after time rings
void ring(time_s present_time){
  Serial.print("Alarm is ringing, because it is: ");
  Serial.print(present_time.hour);
  Serial.print(":");
  Serial.print(present_time.minute);
  Serial.print(":");
  Serial.print(present_time.second);
  Serial.print(":");
  Serial.print(present_time.milliseconds);
  Serial.print(" ");
  Serial.print(present_time.day);
  Serial.print(".");
  Serial.print(present_time.month);
  Serial.print(".");
  Serial.println(present_time.year);
}

//This function is handled by timer1 and will be executed every second
void everySecond(){
  //Prints current time
  Serial.print("Now: ");
  Serial.print(time1.toShortTimeString(H_M_S_m,':'));
  Serial.print(" ");
  Serial.println(time1.toLongDateString(D_MM_YYYY,'.'));
}

/* Warning: Alarm may enable protection which blocks ringing in the same day more than one time.
 *          You should to reset this function when you change synchronization time by more than an hour
 *          or enable/disable DST or change time zone using function:
 *          alarm1.resetProtection();
 */
