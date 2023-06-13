#include "heartrate.h"

int find_period (int buffer[BUFFER_SIZE])
{
	int period = 0;
	while((autocorrelation(period+1, BUFFER_SIZE, buffer)<autocorrelation(period, BUFFER_SIZE, buffer))||
		 (autocorrelation(period+2, BUFFER_SIZE, buffer)<autocorrelation(period, BUFFER_SIZE, buffer)))
	{
		//Serial.println(period);
		//Serial.print(": ");
		//Serial.print(autocorrelation(period, BUFFER_SIZE, buffer));
		//Serial.println("down");
		period++;   
	}
	while((autocorrelation(period+1, BUFFER_SIZE, buffer)>autocorrelation(period, BUFFER_SIZE, buffer))||
		 (autocorrelation(period+2, BUFFER_SIZE, buffer)>autocorrelation(period, BUFFER_SIZE, buffer)))
	{
		//Serial.println(period);
		//Serial.print(": ");
		//Serial.print(autocorrelation(period, BUFFER_SIZE, buffer));
		//Serial.println("up");
		period++;   
	}
	
	return period;
}

void mean_center (int buffer[BUFFER_SIZE])
{
	long int sum = 0;
	int mean;
	for(int i=0; i<=(BUFFER_SIZE-1); i++)
	{
		sum += buffer[i];
	}
	mean = sum/BUFFER_SIZE;
	for(int i=0; i<=(BUFFER_SIZE-1); i++)
	{
		buffer[i]-= mean;
	}
}


long int autocorrelation(int lag ,int size, int *function )
{ 
	long int sum = 0;
	int iter = size - lag;
	for(int i=0; i<iter; i++)
	{
		sum += function[i]*function[i+lag];
	}
	return sum/iter;
}

int calculate_bpm(int period)
{
	return(60*SAMPLE_RATE/period);
}

void init_sensor(MAX30105 *sensor)
{
	sensor->begin(Wire, I2C_SPEED_FAST);
	sensor->setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE, SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);
}


int read_bpm(MAX30105 *sensor, int *bpm)
{
	static int buffer[BUFFER_SIZE] = {0};  
	static int bufferIndex = 0;
	static int counter = 0;
	int temp;
	static int avg = 0;
	int rvalue = 0;
	sensor->check();
	if(sensor->available())

	{
		if(sensor->getFIFORed() < DETECTION_THRESHOLD)
			{
				sensor->nextSample();  
				return 0;
			}
		buffer[bufferIndex] = sensor->getFIFORed();
		bufferIndex++;
		sensor->nextSample();
		if(bufferIndex == BUFFER_SIZE)
			{   
				mean_center(buffer);
				temp = calculate_bpm(find_period(buffer));
				if(temp > 40 && temp <200)
				{
					Serial.println(temp);
					rvalue = 2;
					avg += temp;
					counter++;
				}
				bufferIndex = 0; 
			}
	}
	if(counter == AVERAGING)
		{
			*bpm = avg/AVERAGING;
			Serial.println("average: ");
			Serial.println(*bpm);
			counter = 0;
			avg = 0;
			rvalue = 1;
		}
	return rvalue;

}

float calculate_factor(int restBPM, int finalBPM)
{
	return finalBPM/restBPM;
}