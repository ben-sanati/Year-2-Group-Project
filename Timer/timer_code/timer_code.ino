#include "localTimer.h"

 // simulating the global steps variable.
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 init_timer();
}

void loop() {
   // put your main code here, to run repeatedly:
   
  bool steps_detected = false;
  check_steps(steps_detected);
    
  inactivity();
      
 //  delay(100);
}
