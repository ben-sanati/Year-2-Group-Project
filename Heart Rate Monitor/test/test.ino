
#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor; // initialize MAX30102 with I2C

#define DETECTION_THRESHOLD 50000
#define BUFFER_SIZE 400
#define LED_BRIGHTNESS 70
#define SAMPLE_AVERAGE 1
#define LED_MODE 2
#define SAMPLE_RATE 100
#define PULSE_WIDTH 69
#define ADC_RANGE 16384

int bufferIndex = 0;
int buffer[BUFFER_SIZE];
int temp;
int avg;
unsigned long int sum = 0;
int highest;
int period; 
int atcAtN; 
int tempSum;


long int autocorrelation(int lag ,int size, int *function )
{ 
  long long int sum = 0;
  int iter = size - lag;
  for(int i=0; i<iter; i++)
  {
    sum += function[i]*function[i+lag];
  }
  return sum/iter;
  
}

void setup()
{
  Serial.begin(115200);
  while(!Serial); //We must wait for Teensy to come online
  delay(100);
  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  byte ledBrightness = 70; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384



  particleSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE, SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE); //Configure sensor with these settings
}

void loop() 
{
  particleSensor.check(); //Check the sensor
  while (particleSensor.available()) {

      if(particleSensor.getFIFORed() < DETECTION_THRESHOLD)
      {
        particleSensor.nextSample();  
        continue;
      }
      temp = particleSensor.getFIFORed();
      sum +=temp;
      buffer[bufferIndex] = temp;
      
      // read next set of samples
      particleSensor.nextSample();     
      bufferIndex++;
      if(bufferIndex==BUFFER_SIZE)
      {
        tempSum = 0;
        period = 0;
        avg = sum/BUFFER_SIZE;
        for(int i=0; i<=(BUFFER_SIZE-1); i++)
        {
          buffer[i]=buffer[i]-avg;
          //Serial.println(buffer[i]);
        }
        sum = 0;
        bufferIndex = 0; 

        

        while((autocorrelation(period+1, BUFFER_SIZE, buffer)<autocorrelation(period, BUFFER_SIZE, buffer))||(autocorrelation(period+2, BUFFER_SIZE, buffer)
            <autocorrelation(period, BUFFER_SIZE, buffer)))
        {
           period++;   
        }

        while((autocorrelation(period+1, BUFFER_SIZE, buffer)>autocorrelation(period, BUFFER_SIZE, buffer))||(autocorrelation(period+2, BUFFER_SIZE, buffer)
            >autocorrelation(period, BUFFER_SIZE, buffer)))
        {
           period++;   
        }
        


         
        for(int i=0; i<150; i++)
        {
          Serial.print(i);
          Serial.print(": ");
          Serial.println(autocorrelation(i, BUFFER_SIZE, buffer));
        }
        
        
        //Serial.println(period);
        //Serial.println("BPM:");
        Serial.print(60*SAMPLE_RATE/period);
        Serial.println(" ");
      }
  }
}
