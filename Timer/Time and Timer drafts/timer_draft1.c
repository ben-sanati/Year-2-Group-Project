#define LED_ALARM   16
#define steps 0

hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

volatile bool end_timer = 0;
int steps_old = 0;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

void IRAM_ATTR onTimer0(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux0); 
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux0);

  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void IRAM_ATTR onTimer1(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux1); 
  if (steps > steps_old){
    end_timer = true;
  }
  else{
    end_timer = false;
  }
  steps_old = steps;
  portEXIT_CRITICAL_ISR(&timerMux1);
}

void setup() {
  Serial.begin(115200);    //need
  pinMode(LED_ALARM,OUTPUT);
  digitalWrite(LED_ALARM,LOW);

  Serial.println("start timer");

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  timer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 1000000, true);
  timerAlarmEnable(timer1);
  
  if (!end_timer){
    timer0 = timerBegin(0, 80, true);
    timerAttachInterrupt(timer0, &onTimer0, true);
    timerAlarmWrite(timer0, 1800000000, true);
    timerAlarmEnable(timer0);
  }
}

void loop() {
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    uint32_t isrCount = 0, isrTime = 0;
    
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux0);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux0);
    
    Serial.print(isrTime); // in miliseconds
    Serial.println(" ms");

    if (isrTime >= 1800000000000){
      digitalWrite(LED_ALARM, HIGH);
    }
    else{
      digitalWrite(LED_ALARM, LOW);
    }
  }

}
