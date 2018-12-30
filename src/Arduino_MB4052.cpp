/**************************************************************************/
/*!
    @file     Arduino_MB4052.cpp
    @author   Dejan Gjorgjevikj
    @license  MIT (see license.txt)

	Arduino library for the MB4052 4-Channel 8-Bit A/D Converter 

    @section  HISTORY
    v1.0 - First release 30.12.2018
*/
/**************************************************************************/

#include "Arduino_MB4052.h"

/// <summary> 
/// Instantiates object for communicating to the MB4052 - ADC from preselected channel
/// </summary>
/// <param name="m_cs_pin"> to MB4052 CS pin (active on LOW) </param>
/// <param name="clk_pin"> to MB4052 ADC CLK pin (max. 100kHz) </param>
/// <param name="data_pin"> to MB4052 Data Out pin </param>
Arduino_MB4052_1ch::Arduino_MB4052_1ch(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin) 
	: m_cs(m_cs_pin), m_clk(clk_pin), m_data(data_pin) { };

/// <summary> Set up the HW </summary>
void Arduino_MB4052_1ch::begin(void) const
{
	pinMode(m_data, INPUT_PULLUP);
	pinMode(m_cs, OUTPUT);
	pinMode(m_clk, OUTPUT);
	digitalWrite(m_cs, HIGH); 
	digitalWrite(m_clk, LOW);
}

/// <summary> Performs ADC on the preselected channel /// </summary>
/// <returns> returns the value read (0-255) </returns>
uint8_t Arduino_MB4052_1ch::readADC() const
{
	static unsigned long last_on = 0UL; // time of last CS deactivated
	uint8_t val = 0;

	// wait for the minimum time if necessary  
	while (micros() - last_on < ADC_CSWCS)
		;
	digitalWrite(m_cs, LOW);
	delayMicroseconds(ADC_CSHCS);

	digitalWrite(m_clk, HIGH);
	delayMicroseconds(ADC_TWACH);
	digitalWrite(m_clk, LOW);
	delayMicroseconds(ADC_TWACL);
	//digitalRead(m_data); // start bit - ignore it

	for (int i = 0; i < 8; i++)
	{
		digitalWrite(m_clk, HIGH);
		delayMicroseconds(ADC_TWACH);
		digitalWrite(m_clk, LOW);
		val <<= 1;
		val |= digitalRead(m_data) ? 1 : 0;
		delayMicroseconds(ADC_TWACL);
	}

	digitalWrite(m_clk, HIGH);
	delayMicroseconds(ADC_TWACH);
	digitalWrite(m_clk, LOW);
	//digitalRead(m_data); // stop bit - ignore it
	//delayMicroseconds(ADC_TWACL);

	// done ADC - unselect
	digitalWrite(m_cs, HIGH);
	last_on = micros();

	return val;
}

/// <summary> 
/// Instantiates object for communicating to the MB4052 performing ADC on specified channel
/// </summary>
/// <param name="m_cs_pin"> to MB4052 CS pin (active on LOW) </param>
/// <param name="clk_pin"> to MB4052 ADC CLK pin (max. 100kHz) </param>
/// <param name="data_pin"> to MB4052 Data Out pin </param>
/// <param name="c0_pin"> to MB4052 C0 pin </param>
/// <param name="c1_pin"> to MB4052 C1 pin </param>
Arduino_MB4052::Arduino_MB4052(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t c0_pin, uint8_t c1_pin)
	: Arduino_MB4052_1ch(m_cs_pin, clk_pin, data_pin), m_c0(c0_pin), m_c1(c1_pin) { };

/// <summary> Set up the HW </summary>
void Arduino_MB4052::begin(void) const
{
	Arduino_MB4052_1ch::begin();
	pinMode(m_c0, OUTPUT);
	pinMode(m_c1, OUTPUT);
};

/// <summary> Performs ADC on a selected channel /// </summary>
/// <param name="ch"> channel to read from 0-3 </param>
/// <returns> returns the value read (0-255) </returns>
uint8_t Arduino_MB4052::readADC(uint8_t ch) const
{
	digitalWrite(m_c0, ch & 1);
	digitalWrite(m_c1, (ch>>1) & 1);
	return Arduino_MB4052_1ch::readADC();
};

