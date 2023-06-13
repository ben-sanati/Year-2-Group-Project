#include "Globaltime.h"

const char* ntpServer = "uk.pool.ntp.org";

String getLocalTime()
{
  String time_str;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
     //Serial.println("Failed to obtain time");
     return("");
  }
  char output[80];
  strftime(output,80,"%D/%H:%M:%S",&timeinfo);
  time_str = String(output);
  return(time_str);
}

String get_time(const char* ssid, const char* password)
{
  String time_str = "";
  volatile bool time_tries = false;
  //Serial.printf("Connecting to %s ", ssid);
  //init and get the time
  while(time_str==""){
    configTime(0, 0, ntpServer);
    time_str = getLocalTime();
  }

  return(time_str);
}
