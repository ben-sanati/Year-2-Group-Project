#include <Arduino.h>

extern volatile bool Timer_interrupt_flag;
void IRAM_ATTR onTimer0();
void toggleLed(bool ledToggleStatus);
void init_timer();
void reset_timer0_method1();
void check_steps(bool steps_detected);
bool inactivity();
