/* This simple example that shows how to use DateTime on ESP8266 including time synchronization
 * using NTP server and acquistion of Time Zone and DST from http://worldtimeapi.org API.
 * It also shows how to use MillisTimer instead of delay.
 * WARNING: Please do not use delay() or delaymicroseconds(), because it can delay receiving response
 *          from NTP server and you can get inaccurate time.
 * WARNING: WARNING: This example works only on ESP8266!
 */

#ifdef ESP8266

#include <ESP8266WiFi.h>
#include <DateTime.h>

#define SSID "your-ssid"
#define PASS  "your-password"

char* ntpServerName = "your-NTP-pool-server"; //Insert URL address of NTP server

DateTime now = DateTime(); //Date time object

MillisTimer timer = MillisTimer(); //New timer instance


void setup() {
  Serial.begin(115200);

  //We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  now.setNTPserver(ntpServerName); //Set NTP server address
  now.NTPinterval(300); //Set NTP synchronization interval to 5 minutes (5*60 = 300 seconds)
  now.synchEnable(true); //Enable time synchronization
  now.NTPenable(true); //Enable time synchronization using NTP server
  now.onNTPsynch(NTP_synch); //Function which will be done after NTP synchronization

  //Timer is used to print real time every second
  timer.setInterval(1000);  //Set interval to timer
  timer.enable(true);       //Enable timer
  timer.repeat(true);       //Set timer repeating
  timer.onHandle(printTime);  //Function which is done when timer expired
}

void loop() {
  //Those functions have to be included in loop
  now.NTPhandler();
  now.updateTZ_DST(60000); //This function updates Time zone and DST every minute using http://worldtimeapi.org API
  timer.handleTime();
}

void printTime(){
  //Prints time and date every second
  Serial.println(now.toLongDateString(D_M_YYYY,'.'));
  Serial.println(now.toShortTimeString(H_M_S_m,':'));
  Serial.println();
}

void NTP_synch(byte error){
  //Prints NTP communication state
  Serial.print("NTP server synchronization: ");
  if(error == NTP_OK) Serial.println("DONE");
  else if(error == NTP_NOT_FOUND) Serial.println("SERVER NOT FOUND");
  else if(error == NTP_TIMEOUT) Serial.println("SERVER TIMEOUT");
  else if(error == NTP_BAD_RESPONSE) Serial.println("INVALID RESPONSE");
  else Serial.println("UNKNOWN STATE");
  Serial.print("Clock ID: ");
  Serial.println(now.NTPclockID());
}


#else
  #error "We are sorry, but this example works only on ESP8266!"
#endif
