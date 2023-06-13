#define LED_ALARM   16
#define steps 0

hw_timer_t *timer1 = NULL;



volatile bool Timer_interrupt_flag = false;
int Oldsteps = 0;

void reset_timer0();
void IRAM_ATTR onTimer0(){
  // Increment the counter and set the time of ISR
  
 

  Timer_interrupt_flag = true;
  

  // Give a semaphore that we can check in the loop
 
}



void setup() {
  Serial.begin(115200);    //need
  pinMode(LED_ALARM,OUTPUT);
  digitalWrite(LED_ALARM,LOW);

  Serial.println("start timer");

  // Create semaphore to inform us when the timer has fired
  
  
}

void loop() {
  // If Timer has fired
    
    if( steps > Oldsteps)
    {
        reset_timer0_method1();
        Oldsteps = steps;

    }

    if(Timer_interrupt_flag)
    {
        Serial.println("User has been inactive for 30 min");
        Timer_interrupt_flag = false;
        digitalWrite(LED_ALARM, HIGH);
        toggleLed();
       
    }
    else
    {
        digitalWrite(LED_ALARM, LOW);
    }

    
    

  }

}

 void toggleLed(bool togglestatus)
 {
   int ledPin = 2;
    static unsigned long int localTimeMicros = 0;
    static bool ledState = false;
    int ledDelay = 10000;
    
    if(timerReadMicros(timer1) - localTimeMicros >= ledDelay)
    {
      ledState = !ledState;
      digitalWrite(ledPin,ledState);
      localTimeMicros = timerReadMicros(timer1);
    }
 }

void init_timer()
{
  timer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer1, &onTimer0, true);
  timerAlarmWrite(timer1, 1000000, true); // set this values to 30 min.
  timerAlarmEnable(timer1);

}

void reset_timer0_method1()
{
  Serial.println("restarting timer")
  timerWrite(timer1, 0);//restarting timer
  init_timer();
}

void reset_timer0_method2() // this method needs to reinitalise the timer again after restarting
{
  Serial.println("restarting timer")
  timerAlarmDisable(timer1);		// stop alarm
  timerDetachInterrupt(timer1);	// detach interrupt
  timerEnd(timer1);			// end timer
  init_timer();
}
