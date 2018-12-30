/**************************************************************************/
/*!
    @file     MB4052.h
    @author   Dejan Gjorgjevikj
    @license  MIT (see license.txt)
    
	Arduino template class library for the Fujitsu MB4052 4-Channel 8-Bit A/D Converter 
	Produces slightly shorter and faster code than Arduino_MB4052 class
	Not recommended if you are connecting 2 or more MB4052s (will produce longer code)

    @section  HISTORY
    v1.0 - First release 30.12.2018
*/
/**************************************************************************/

#ifndef MB4052_H
#define MB4052_H

#define MB4052_VER 1.0.0

#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/// <summary> Interface to MB4052 A/D Converter
/// <para> CS_PIN to MB4052 CS pin (active on LOW) </para>
/// <para> CLK_PIN to MB4052 ADC CLK pin (max. 100kHz) </para>
/// <para> DATA_OUT_PIN to MB4052 Data Out pin </para>
/// <para> C0_PIN to MB4052 C0 pin </para>
/// <para> C1_PIN to MB4052 C1 pin </para>
/// </summary>
template <uint8_t CS_PIN, uint8_t CLK_PIN, uint8_t DATA_OUT_PIN, uint8_t C0_PIN, uint8_t C1_PIN>
class MB4052 
{
	/*=========================================================================
	CONVERSION DELAYS (in uS) according the datasheet
	https://pdf.datasheet.live/c4d38c53/fujitsu.com/MB4052PF.pdf
	-----------------------------------------------------------------------*/
	enum ADC_T_uS
	{
		ADC_TWACH = 5, // ADC CLK H Level Pulse Width
		ADC_TWACL = 6, // ADC CLK L Level Pusle Width
		ADC_CSWCS = 2, // CS H Level Pulse Width
		ADC_CSSCS = 2, // CS Set-up Time
		ADC_CSHCS = 2, // CS Hold Time
		ADC_CHHCH = 2, // Channel Hold Time
		ADC_PROPD = 2  // Propagation Delay Time
	};

public:
	/// <summary> Object for interfacing to the MB4052 A/D Converter </summary>
	Arduino_MB4052() {}

	/// <summary> Sets up the HW </summary>
	static void begin(void)
	{
		pinMode(DATA_OUT_PIN, INPUT_PULLUP);
		pinMode(CS_PIN, OUTPUT);
		pinMode(CLK_PIN, OUTPUT);
		digitalWrite(CS_PIN, HIGH);
		digitalWrite(CLK_PIN, LOW);
		pinMode(C0_PIN, OUTPUT);
		pinMode(C1_PIN, OUTPUT);
	}

	/// <summary> Performs ADC on a specified channel /// </summary>
	/// <param name="ch"> chanel to read from 0-3 </param>
	/// <returns> returns the value read (0-255) </returns>
	static uint8_t readADC(uint8_t ch)
	{
		static unsigned long last_on = 0UL; // time of last CS deactivated
		uint8_t val = 0;

		// select channel
		digitalWrite(C0_PIN, ch & 1);
		digitalWrite(C1_PIN, (ch >> 1) & 1);

		// wait for the minimum since CS went up time if necesarry 
		while (micros() - last_on < ADC_CSWCS)
			;
		
		digitalWrite(CS_PIN, LOW);
		delayMicroseconds(ADC_CSHCS);

		digitalWrite(CLK_PIN, HIGH);
		delayMicroseconds(ADC_TWACH);
		digitalWrite(CLK_PIN, LOW);
		delayMicroseconds(ADC_TWACL);
		//digitalRead(DATA_OUT_PIN); // start bit - ignore it

		for (int i = 0; i < 8; i++)
		{
			digitalWrite(CLK_PIN, HIGH);
			delayMicroseconds(ADC_TWACH);
			digitalWrite(CLK_PIN, LOW);
			val <<= 1;
			val |= digitalRead(DATA_OUT_PIN) ? 1 : 0;
			delayMicroseconds(ADC_TWACL);
		}

		digitalWrite(CLK_PIN, HIGH);
		delayMicroseconds(ADC_TWACH);
		digitalWrite(CLK_PIN, LOW);
		//digitalRead(DATA_OUT_PIN); // stop bit - ignore it
		//delayMicroseconds(ADC_TWACL);

		// done ADC - unselect
		digitalWrite(CS_PIN, HIGH);
		last_on = micros();

		return val;
	}
};

#endif