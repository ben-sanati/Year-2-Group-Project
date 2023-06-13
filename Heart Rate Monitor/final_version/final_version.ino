#include "heartrate.h"

MAX30105 sensor;
MAX30105 *ptr;
int BPM;
int *ptrBPM;


void setup() 
{
  ptr = &sensor;
  ptrBPM = &BPM;
  Serial.begin(115200);
  while(!Serial);
  init_sensor(ptr);

}

void loop() 
{

  read_bpm(ptr,ptrBPM);
  
}
