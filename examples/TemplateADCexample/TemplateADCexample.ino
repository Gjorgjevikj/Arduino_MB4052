#include <MB4052.h>

static const uint8_t adc_CS = 5;	// connected to MB4052 SC pin
static const uint8_t adc_CLK = 3;	// connected to MB4052 ADC CLK pin
static const uint8_t adc_DATA = 4;	// connected to MB4052 Data out pin
static const uint8_t adc_C0 = 6;	// connected to MB4052 C0 pin
static const uint8_t adc_C1 = 7;	// connected to MB4052 C1 pin

typedef MB4052<adc_CS, adc_CLK, adc_DATA, adc_C0, adc_C1> adc;

void setup()
{
	adc::begin();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(9600);
	delay(100);
	Serial.println("Starting ADC on 4 channels...");
	Serial.flush();
}

void loop() 
{
	int av[4];
	
	digitalWrite(LED_BUILTIN, HIGH);

	for (int i = 0; i < 4; i++)
		av[i] = adc::readADC(i);

	Serial.print(0);
	Serial.print(", ");
	for (int i = 0; i < 4; i++)
	{
		Serial.print(av[i]);
		Serial.print(", ");
	}
	Serial.println(255);

	digitalWrite(LED_BUILTIN, LOW);
	delay(100);
}

