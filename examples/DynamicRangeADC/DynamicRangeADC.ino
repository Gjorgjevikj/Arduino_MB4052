#include <Arduino_MB4052.h>

#define ADC_CS 5	// connected to MB4052 SC pin
#define ADC_CLK 3	// connected to MB4052 ADC CLK pin
#define ADC_DATA 4	// connected to MB4052 Data out pin
#define ADC_C0 6	// connected to MB4052 C0 pin
#define ADC_C1 7	// connected to MB4052 C1 pin
#define ADC_RS 8	// connected to MB4052 RS pin

typedef enum
{
	RANGE_Vcc_1_2 = 1, // Range Vcc/2
	RANGE_Vcc_1_8 = 0  // Range Vcc/8
} ADCRange_t;

/// <summary> Arduino_MB4052_dr Dynamic range class
/// for performing adc on selected chanel automatically adjusting the range
/// </summary>
class Arduino_MB4052_dr : private Arduino_MB4052
{
protected:
	uint8_t m_rs;
	ADCRange_t m_range;
	float m_Vcc;
public:
	/// <summary> Arduino_MB4052_dr constructor
	/// Instantiates Arduino_MB4052_dr object for performing adc on selected chanel with dynamic range adjusment
	/// </summary>
	/// <param name="m_cs_pin"> to MB4052 CS pin (active on LOW) </param>
	/// <param name="clk_pin"> to MB4052 ADC CLK pin (max. 100kHz) </param>
	/// <param name="data_pin"> to MB4052 Data Out pin </param>
	/// <param name="c0_pin"> to MB4052 C0 pin </param>
	/// <param name="c1_pin"> to MB4052 C1 pin </param>
	/// <param name="r_pin"> to MB4052 RS pin </param>
	Arduino_MB4052_dr(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t c0_pin, uint8_t c1_pin, uint8_t r_pin)
		: Arduino_MB4052(m_cs_pin, clk_pin, data_pin, c0_pin, c1_pin), m_rs(r_pin), m_Vcc(5.0), m_range(RANGE_Vcc_1_2) { }

	/// <summary> Arduino_MB4052_dr begin
	/// Sets up the HW
	/// </summary>
	void begin(ADCRange_t range)
	{
		Arduino_MB4052::begin();
		pinMode(m_rs, OUTPUT);
		setRange(range);
	}

	/// <summary> Sets up the range </summary>
	/// <param name="range"> RANGE_Vcc_1_2 or RANGE_Vcc_1_8 </param>
	void setRange(ADCRange_t range)
	{
		m_range = range;
		digitalWrite(m_rs, (range == RANGE_Vcc_1_2) ? HIGH : LOW);
	}

	/// <summary> Get the current range </summary>
	/// <returns> returns the range </returns>
	/// <param name="range"> RANGE_Vcc_1_2 or RANGE_Vcc_1_8 </param>
	ADCRange_t getRange(void)
	{
		return m_range;
	}

	/// <summary> Sets the Vcc value </summary>
	/// <param name="vcc"> the voltage on Vcc1 </param>
	void setVcc(float vcc)
	{
		m_Vcc = vcc;
	}
	
	/// <summary> Gets the setted Vcc value </summary>
	/// <returns> returns the setted Vcc value </returns>
	float getVcc(void) const
	{
		return m_Vcc;
	}

	/// <summary> Performs ADC on selected chanel and converts to voltage /// </summary>
	/// <remark> If signal value allows, automaticaly reduces the range 
	/// and performs another read for obtaining better precision </remark>
	/// <param name="ch"> chanel to read from 0-3 </param>
	/// <returns> returns the value read in volts </returns>
	float readV(uint8_t ch) const
	{
		uint8_t dval = readADC(ch);
		float aval = dval * m_Vcc / (2 * 255);
		
		if (m_range == RANGE_Vcc_1_8)
			aval /= 4;
		else if (aval < 0.9 * m_Vcc / 8)
		{
			//Serial.print('*');
			setRange(RANGE_Vcc_1_8);
			dval = readADC(ch);
			aval = dval * m_Vcc / (8 * 256);
			setRange(RANGE_Vcc_1_2);
		}

		return aval;
	}
};

Arduino_MB4052_dr adc(ADC_CS, ADC_CLK, ADC_DATA, ADC_C0, ADC_C1, ADC_RS);

const float Vcc = 4.685;

void setup()
{
	adc.begin(RANGE_Vcc_1_2);
	adc.setVcc(Vcc);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	pinMode(9, OUTPUT);
	analogWrite(9, 100);

	Serial.begin(9600);
	delay(100);
	Serial.println("Reading voltages ... in mV");
	Serial.flush();
}

void loop() 
{
	float av[4];
	
	digitalWrite(LED_BUILTIN, HIGH);

	for(int i=0; i<4; i++)
		av[i] = adc.readV(i);

	for (int i = 0; i < 4; i++)
	{
		Serial.print(av[i]*1000);
		Serial.print(", ");
	}
	Serial.println();

	digitalWrite(LED_BUILTIN, LOW);
	delay(50);
}

