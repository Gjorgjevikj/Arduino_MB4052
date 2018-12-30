#include <Arduino_MB4052.h>

#define ADC_CS 5	// connected to MB4052 SC pin
#define ADC_CLK 3	// connected to MB4052 ADC CLK pin
#define ADC_DATA 4	// connected to MB4052 Data out pin

// MB4052s CO, C1 and RS should be prewired for selecting the channel and the range

Arduino_MB4052_1ch adc(ADC_CS, ADC_CLK, ADC_DATA);

// if supplying power on Vcc1, all readings are relative to this value (max. Vcc/2 or Vcc/8 depending on RS)
const float Vcc = 4.697;

void setup()
{
	adc.begin();
	Serial.begin(9600);
	delay(100);
}

void loop() 
{
	int av;
	
	av = adc.readADC();

	float v = av*(Vcc / (2.0 * 255));

	Serial.print("ADC value: ");
	Serial.print(av);
	Serial.print("\t = ");
	Serial.print(v,3);
	Serial.print("V");
	Serial.println();

	delay(100);
}

