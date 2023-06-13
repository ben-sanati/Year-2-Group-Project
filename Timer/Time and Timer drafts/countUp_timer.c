// Stop button is attached to PIN 0 (IO0)
#define BTN_STOP_COUNTING    0

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  // Give a semaphore that we can check in the loop

}

void setup() {
  // start serial of 115200

  // Set BTN_STOP_COUNTING to input mode

  // Create semaphore to inform us when the timer has fired

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler 
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

}


void loop() {
	// If Timer has fired
		// Read the interrupt count and time
	
	// If BTN_STOP_COUNTING is pressed
		// stop timer
		// read time 
		// print the real time of the activity in string
		
		// If timer is still running
			// Stop and free timer
}