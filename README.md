# Arduino driver for MB4052 ADC
Arduino library for the Fujitsu MB4052 4-Channel 8-Bit A/D Converter 

## Arduino_MB4052_1ch Class

Interface class to MB4052 for performing ADC on a single (hardwired / preselected) channel  

## Arduino_MB4052 Class

Interface class to MB4052 for performing ADC on specified channel 

  ```C
    // Instantiates object for communicating to the MB4052 performing ADC on specified channel
  Arduino_MB4052(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t c0_pin, uint8_t c1_pin) 
    m_cs_pin -> to MB4052 CS pin
    clk_pin  -> to MB4052 ADC CLK pin (max. 100kHz) 
    data_pin -> to MB4052 Data Out pin 
    c0_pin   -> to MB4052 C0 pin 
    c1_pin   -> to MB4052 C1 pin 
  
  void begin(void); // initializes the hardware
  
  uint8_t readADC(uint8_t ch); // Performs ADC on a selected chanel (0-3), returns the value read (0-255)
  ```
  
  ## MB4052 Template Class

Arduino template class library for the Fujitsu MB4052 4-Channel 8-Bit A/D Converter 
Pins are provided as template arguments and the class is fully static
Produces slightly shorter and faster code than Arduino_MB4052 class
Not recommended if you are connecting 2 or more MB4052s (will produce longer code) 

  ```C
    // Interface to MB4052 A/D Converter
    //   CS_PIN to MB4052 CS pin (active on LOW)
    //   CLK_PIN to MB4052 ADC CLK pin (max. 100kHz)
    //   DATA_OUT_PIN to MB4052 Data Out pin
    //   C0_PIN to MB4052 C0 pin
    //   C1_PIN to MB4052 C1 pin
  template <uint8_t CS_PIN, uint8_t CLK_PIN, uint8_t DATA_OUT_PIN, uint8_t C0_PIN, uint8_t C1_PIN> class MB4052 

  void MB4052::begin(void); // initializes the hardware
  
  uint8_t MB4052::readADC(uint8_t ch); // Performs ADC on a selected chanel (0-3), returns the value read (0-255)
  ```
  
  Example:
  
  ```C
  typedef MB4052<adc_CS, adc_CLK, adc_DATA, adc_C0, adc_C1> adc;

	. . .
	adc::begin();
	. . .
	val = adc::readADC(i);
	. . .

  ```
  
 