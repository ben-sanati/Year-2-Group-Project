
#include "Globaltime.h"



 // simulating the global steps variable.
void setup() {
  String time_str;
  // put your setup code here, to run once:
  Serial.begin(115200);
  time_str = get_time("VM9149941", "Wq5xxnnpjrrh");
  Serial.println(time_str);
}

void loop() {
}
