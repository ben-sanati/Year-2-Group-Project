#include "Esp.h"
#include "esp32-hal-adc.h"
#include "hall.h"
#include "accel.h"
#include "timer.h"

int hall_count = 0;
int hall_points = 0;
char output[512];

bool hes = false;
bool accel = false;
Accelerometer acc;

int points = 0;

void setup() {  
  Serial.begin(115200);
  //ADC pins for accelerometer
  pinMode(ADC1_0, INPUT);   //x
  pinMode(ADC1_1, INPUT);   //y
  pinMode(ADC1_2, INPUT);   //z
  
  //Pins 4 and 5 for push buttons
  pinMode(4, INPUT_PULLUP); //Hall Effect Sensor
  pinMode(5, INPUT_PULLUP); //Accelerometer
}

void loop() {
  if(digitalRead(4) == LOW){
    hes = true;
    accel = false;
    Serial.println("Hall Effect Sensor activated");
  } else if(digitalRead(5) == LOW){
    accel = true;
    hes = false;
    Serial.println("Accelerometer activated");
  }

  // for testing
  hes = true;
  accel = true;
  if(hes){
    // Read hall effect sensor
    int initialCount = hall_count;
    hall_count = getCount(hall_count);
  
    //required to ensure points dont keep increasing while count % 10 = 0 for more than 1 cycle of loop
    if(initialCount % 10 != 0) {
      hall_points = pointsReturn(hall_count, hall_points); 
    }

    sprintf(output,"Points: %d Count: %d\n", hall_points, hall_count);
    Serial.print(output);
    
  } if(accel){
    //Accelerometer code goes here
    points = acc.readAcc(points);
  }
  
  
}
