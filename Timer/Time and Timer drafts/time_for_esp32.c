#include <WiFi.h>
#include <time.h>
String time_str;
char time_output[80];

const char* ssid       = "WIFI_SSID";
const char* password   = "WIFI_PASS";

const char* ntpServer = "uk.pool.ntp.org";

void getLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char output[80];
  strftime(time_output,80,"%D/%H:%M:%S",&timeinfo);
  time_str = String(output);
  Serial.println(time_str);
  return;
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(0, 0, ntpServer);
  getLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
}
