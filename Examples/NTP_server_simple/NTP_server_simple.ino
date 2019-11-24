/* This simple example shows, how to synchronize time using NTP server.
 * We recommended to use NTPsynchNow() after deep sleep, because during deep sleep time is not synchronized!
 * WARNING: Please do not use delay() or delaymicroseconds(), because it can delay receiving response
 *          from NTP server and you can get inaccurate time.
 * WARNING: This example works only on ESP8266!
 */

#ifdef ESP8266

#include <ESP8266WiFi.h>
#include <DateTime.h>

#define SSID "your-ssid"
#define PASS  "your-password"

const char* ntpServerName = "your-NTP-pool-server"; //Insert URL address of NTP server

#define timeZone +1 //Time zone GMT+1
#define DST_offset 60 //DST offset in minutes

DateTime now = DateTime(); //Date time object

MillisTimer timer = MillisTimer(); //New timer instance

unsigned long print_mil = 0;

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

  now.setTimezone(timeZone);  //Set time zone
  now.DST(true,DST_offset);   //Enable DST and set DST offset

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
