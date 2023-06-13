#include "localTimer.h"



#define LED_ALARM   2


hw_timer_t *timer1 = NULL;



volatile bool Timer_interrupt_flag = false;

bool inactivity(){
  bool active = true;
  static bool ledToggleMode = false;
   toggleLed(ledToggleMode);


  static double lastTime = 0.0;
  if(Timer_interrupt_flag)
    {
        Serial.println("User has been inactive for 30 seconds");
        active = false;
        Timer_interrupt_flag = false;
        // Serial.println("Timer_interrupt_flag is set to false");
        //digitalWrite(LED_ALARM, HIGH);
        ledToggleMode = true;
        lastTime = timerReadSeconds(timer1);
       
    }
    else
    {
      if(timerReadSeconds(timer1) - lastTime >= 1)// toggles led for one second if user has been inactive for 5 seconds
      {
        ledToggleMode = false;
        digitalWrite(LED_ALARM,LOW);
        lastTime = timerReadSeconds(timer1);
        
      }

       if(timerReadSeconds(timer1) <= 0.2)
       {
         lastTime = timerReadSeconds(timer1);
       }
        //digitalWrite(LED_ALARM, LOW);
    }
  return active;
}

void check_steps(bool steps_detected){
  if(steps_detected)
    {
        reset_timer0_method1();

    }
}

void IRAM_ATTR onTimer0(){
  // Increment the counter and set the time of ISR
  
 

  Timer_interrupt_flag = true;
  //Serial.println("Timer_interrupt_flag is true");
  

}


void toggleLed(bool ledToggleStatus)
 {
  //Serial.printf("\ntime now = %lf",timerReadSeconds(timer1));
    static double localTimeSeconds = 0.0;
    static bool ledState = false;
    double ledDelay = 0.2;
   // Serial.println(ledToggleStatus);
   
     if(timerReadSeconds(timer1) - localTimeSeconds >= 0.1)
    {

      if(ledToggleStatus)
      {
    //  Serial.println("led state toggled");
     
      ledState = !ledState;
      digitalWrite(LED_ALARM,ledState);
     
      }

       localTimeSeconds = (double)timerReadSeconds(timer1);
    }

    if(timerReadSeconds(timer1) <= 0.2)
    {
      localTimeSeconds = (double)timerReadSeconds(timer1);
    }

  
 
 }

void init_timer()
{
  pinMode(LED_ALARM,OUTPUT);
    timer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer1, &onTimer0, true); // 1 tick = 1 usec
  timerAlarmWrite(timer1, 10000000, true); // set this values to 30 min - currently 10 sec
  timerAlarmEnable(timer1);
  // Serial.println("local timer initialized");

}

void reset_timer0_method1()
{
  // Serial.println("restarting timer using method1");
  timerWrite(timer1, 0);//restarting timer
  init_timer();
}

void reset_timer0_method2() // this method needs to reinitalise the timer again after restarting
{
  // Serial.println("restarting timer");
  timerAlarmDisable(timer1);    // stop alarm
  timerDetachInterrupt(timer1); // detach interrupt
  timerEnd(timer1);     // end timer
  init_timer();
}
