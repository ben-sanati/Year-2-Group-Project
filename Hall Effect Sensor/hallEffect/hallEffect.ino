#include "hall.h"

int count = 0;
int points = 0;
char output[512];

void setup() {  
  Serial.begin(115200);
}

void loop() {
  int initialCount = count;
  count = getCount(count);

  //required to ensure points dont keep increasing while count % 10 = 0 for more than 1 cycle of loop
  if(initialCount % 10 != 0) {
    points = pointsReturn(count, points); 
  }

  sprintf(output,"Points: %d Count: %d\n", points, count);
  Serial.print(output);
}
