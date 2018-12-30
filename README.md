# Arduino driver for MB4052 ADC
Arduino library for the MB4052 4-Channel 8-Bit A/D Converter 

## MB4052 

Arduino_MB4052 object enables performing ADC using the Fujitsy MB4052 
  
  ```C
  Arduino_MB4052(uint8_t m_cs_pin, uint8_t clk_pin, uint8_t data_pin, uint8_t c0_pin, uint8_t c1_pin) // constructor
    m_cs_pin -> to MB4052 CS pin
    clk_pin  -> to MB4052 ADC CLK pin (max. 100kHz) 
    data_pin -> to MB4052 Data Out pin 
    c0_pin   -> to MB4052 C0 pin 
    c1_pin   -> to MB4052 C1 pin 

  begin(); // initializes the hardware
  
  readADC(uint8_t ch); // Performs ADC on a selected chanel (0-3)
  ```
  
   
