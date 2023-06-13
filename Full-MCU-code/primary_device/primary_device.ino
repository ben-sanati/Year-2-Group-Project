#include "heartrate.h"
#include "Pedometer.h"
#include "localTimer.h"
#include "Globaltime.h"
#include "credentials.h"

MAX30105 sensor;
MAX30105 *ptr;
int BPM;
float resting_bpm;
float active_bpm;
float factor;
int *ptrBPM;
bool active = false;
bool old_active = false;

pedometer_struct pedometer_data;

int led = 2;

String time_str;

void setup() {
  Serial.begin(115200);
  // Setup for Heartrate monitor
  ptr = &sensor;
  ptrBPM = &BPM;
  while(!Serial);
  init_sensor(ptr);

  // ---------------------------
  //Setup timer and Pedometer
  pinMode(led, OUTPUT);
  Serial.println("start timer");
  pedometer_data = init_pedometer_timer();
  init_timer();
}

void loop() {
  // Read heartrate monitor
  if(read_bpm(ptr,ptrBPM)){
    while(read_bpm(ptr,ptrBPM) != 1);
    Serial.println(active);
    Serial.println(BPM);
    if(!active){
      resting_bpm = static_cast<float>(BPM);
    } else{
      active_bpm = static_cast<float>(BPM);
    }
    Serial.print("Heart rate ratio = ");
    Serial.println(active_bpm/resting_bpm);
  }
  // -------------------------------
  //Pedometer:
  pedometer_data = detect_step_and_count(pedometer_data, 34, 35, 32);
  if(pedometer_data.step_detected){
     Serial.printf("\nSteps incremented , steps = %d\n",pedometer_data.steps);
  }

   old_active = active;
   check_steps(pedometer_data.step_detected);
   if(pedometer_data.step_detected){
    active = true;
   }
   if(!inactivity()){
    active = false;
   }
   if(!active && old_active){
    Serial.print("activity finished. Points: ");
    Serial.println(pedometer_data.total_points);
    Serial.print("adjusted for BPM: ");
    Serial.println(active_bpm/resting_bpm*pedometer_data.total_points);
    Serial.println("User ID: 000001");
    // global time
    time_str = get_time(SSID, PASSWORD);
     Serial.print("Time and date is: ");
     Serial.println(time_str);
   }
  
  
  // delay(100);
}
