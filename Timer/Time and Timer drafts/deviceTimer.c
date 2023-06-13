#define LED_ALARM   16

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux); 
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);

  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void setup() {
  Serial.begin(115200);    //need
  pinMode(LED_ALARM,OUTPUT);
  digitalWrite(LED_ALARM,LOW);

  Serial.println("start timer");

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1800000000, true); //need  1000000*1us =1s (1800000000*1us =30mins), autoreload true
  // Start an alarm
  timerAlarmEnable(timer);
}

void loop() {
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    uint32_t isrCount = 0, isrTime = 0;
    
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
    
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