#include <Wire.h>
#include "MAX30105.h"

const int DETECTION_THRESHOLD = 50000;
const int BUFFER_SIZE = 400;
const int LED_BRIGHTNESS = 70;
const int SAMPLE_AVERAGE = 1;
const int LED_MODE = 2;
const int SAMPLE_RATE = 100;
const int PULSE_WIDTH = 69;
const int ADC_RANGE = 16384;
const int AVERAGING = 3;

int find_period (int buffer[BUFFER_SIZE]);
void mean_center (int buffer[BUFFER_SIZE]);
long int autocorrelation(int lag ,int size, int *function );
int calculate_bpm(int period);
void init_sensor(MAX30105 *sensor);
int read_bpm(MAX30105 *sensor, int *bpm);
float calculate_factor(int restBPM, int finalBPM);

