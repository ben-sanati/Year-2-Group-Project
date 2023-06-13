hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void setup() {

	Serial.begin(115200);
	
	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
	// Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second
	timer = timerBegin(0, 80, true);                
	timerAttachInterrupt(timer, &onTime, true);    
	// Fire Interrupt every 1m ticks, so 30min
	timerAlarmWrite(timer, 1800000000, true);			
	timerAlarmEnable(timer);
}

void IRAM_ATTR onTime() {
	portENTER_CRITICAL_ISR(&timerMux);
	digitalWrite(16, HIGH);
	portEXIT_CRITICAL_ISR(&timerMux);
	
}

void loop() {
	vTaskDelay(portMax_Delay);
}
