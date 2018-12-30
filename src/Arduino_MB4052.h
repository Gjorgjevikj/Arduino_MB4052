/**************************************************************************/
/*!
    @file     Arduino_MB4052.cpp
    @author   D.Gjorgjevikj
    @license  MIT (see license.txt)
    
	Arduino library for the MB4052 4-Channel 8-Bit A/D Converter 

    @section  HISTORY
    v1.0 - First release 30.12.2018
*/
/**************************************************************************/

#ifndef ARDUINO_MB4052_H
#define ARDUINO_MB4052_H

#define ARDUINO_MB4052_VER 1.0.0

#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/*=========================================================================
	CONVERSION DELAYS (in uS) according the datashet 
	https://pdf.datasheet.live/c4d38c53/fujitsu.com/MB4052PF.pdf
	-----------------------------------------------------------------------*/
#define ADC_TWACH (5) // ADC CLK H Level Pulse Width
#define ADC_TWACL (6) // ADC CLK L Level Pusle Width
#define ADC_CSWCS (2) // CS H Level Pulse Width
#define ADC_CSSCS (2) // CS Set-up Time
#define ADC_CSHCS (2) // CS Hold Time
#define ADC_CHHCH (2) // Channel Hold Time
#define ADC_PROPD (2) // Propagation Delay Time
/*=========================================================================*/

/// <summary> Arduino_MB4052_1ch Class 
/// for performing adc on a single chanel (harwired selection)
/// </summary>
class Arduino_MB4052_1ch
{
protected:
	uint8_t m_cs;
	uint8_t m_clk;
	uint8_t m_data;
public:
	Arduino_MB4052_1ch(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin);
	void begin(void) const;
	uint8_t readADC() const;
};

/// <summary> Arduino_MB4052 Class 
/// for performing adc on selected chanel 
/// </summary>
class Arduino_MB4052 : private Arduino_MB4052_1ch
{
protected:
	uint8_t m_c0;
	uint8_t m_c1;
public:
	Arduino_MB4052(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t c0_pin, uint8_t c1_pin);
	void begin(void) const;
	uint8_t readADC(uint8_t ch) const;
};

#endif