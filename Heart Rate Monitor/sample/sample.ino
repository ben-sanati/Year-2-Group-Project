#include "heartrate.h"

MAX30105 sensor;
int buffer[BUFFER_SIZE];  
int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial); //We must wait for Teensy to come online
  delay(100);
  // Initialize sensor
  if (sensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  sensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE, SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

}

void loop() {

  sensor.check();
  while (sensor.available())
    {
      if(sensor.getFIFORed() < DETECTION_THRESHOLD)
        {
          sensor.nextSample();  
          continue;
        } 

       buffer[bufferIndex] = sensor.getFIFORed();
       bufferIndex++;
       sensor.nextSample();

       if(bufferIndex==BUFFER_SIZE)
       {       
         mean_center(buffer);
         for(int i =0; i<(BUFFER_SIZE-1); i++)
         {
            Serial.println(buffer[bufferIndex]);
         }
         
         Serial.println(calculate_bpm(find_period(buffer)));
         bufferIndex = 0; 
       }
    }

}
