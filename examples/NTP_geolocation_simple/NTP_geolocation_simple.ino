/* This simple example shows, how to use DateTime with IPGeolocation
 * library, which help us to get time zone and DST and how to synchronize
 * time using NTP server.
 * You can download IPGeolocation library here: https://github.com/Matt-prog/IPgeolocation-master
 * WARNING: Please do not use delay() or delaymicroseconds(), because it can delay receiving response
 *          from NTP server and you can get inaccurate time.
 * WARNING: This example works only on ESP8266!
 */

#ifdef ESP8266

#include <ESP8266WiFi.h>
#include <DateTime.h>
#include "IPGeolocation.h"

#define SSID "your-ssid"
#define PASS  "your-password"

//To get this API key, you have to register at https://ipgeolocation.io/
#define GEO_API_KEY "your-geo-api-key"

char* ntpServerName = "your-NTP-pool-server"; //Insert URL address of NTP server

DateTime now = DateTime(); //Date time object

MillisTimer timer = MillisTimer(); //New timer instance

IPGeolocation geo(GEO_API_KEY); //IPgeolocation object
IPGeo IPG; //IPgeolocation structure

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
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

  //We are using IPgeolocation API to get time offset and DST offset
  Serial.println();
  Serial.println("Geolocation: ");
  if(geo.updateData(&IPG)) Serial.println("DONE");
  else Serial.println("FAILED");
  Serial.println();

  now.setNTPserver(ntpServerName); //set NTP server address
  now.NTPinterval(300); //set NTP synchronization interval to 5 minutes (5*60 = 300 seconds)
  now.synchEnable(true); //enable time synchronization
  now.NTPenable(true); //enable time synchronization using NTP server
  now.onNTPsynch(NTP_synch); //function which will be done after NTP synchronization

  now.setTimezone(IPG.time_zone); //Set time zone offset which we get from IPgeolocation server
  now.DST(IPG.is_DST,IPG.dst_offset*60); //Set DST offset which we get from IPgeolocation server

  //Timer is used to print real time every second
  timer.setInterval(1000);  //Set interval to timer
  timer.enable(true);       //Enable timer
  timer.repeat(true);       //Set timer repeating
  timer.onHandle(printTime);  //Function which is done when timer expired
}

void loop() {
  //Those functions have to be included in loop
  now.NTPhandler();
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
