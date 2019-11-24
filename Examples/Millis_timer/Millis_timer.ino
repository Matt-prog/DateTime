/* This simple example shows how to use MillisTimer to execute some function.
*/
#include <DateTime.h>

MillisTimer timer = MillisTimer(); //New timer instance

void setup(){
  Serial.begin(115200);
  timer.setInterval(1000);  //Set interval to timer
  timer.enable(true);       //Enable timer
  timer.repeat(true);       //Set timer repeating
  timer.onHandle(onTimer);  //Function which is done when timer expired
}

void loop(){
  timer.handleTime(); //This has to be included in loop
}

//Function which is done when timer expired
void onTimer(){
  Serial.print("Milliseconds: ");
  Serial.println(millis());
}

