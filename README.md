![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# ADC_7 Click

- **CIC Prefix**  : ADC7
- **Author**      : Nemanja Medakovic
- **Verison**     : 1.0.0
- **Date**        : Jul 2018.

---


### Software Support

We provide a library for the ADC_7 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2533/adc-7-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

Library can perform 32bit AD conversion and can read this 32bit converted voltage value with or without configuration byte/bytes.
Library also offers a choice to set configuration for the next conversion cycles. It is not necessary to set configuration for the each conversion.
User always can perform conversion cycle/cycles with the last entered configurations.
User can read the converted voltage value as 32bit digital value or as value calculated in mV unit.
For more details check documentation.

Key functions :

- ``` uint8_t adc7_setConfig( uint8_t gainConfig, uint8_t downSampFactor, uint8_t filterType ) ``` - Function performs the device configuration by sending 
  configuration data to configure the next conversion cycle.
- ``` void adc7_startConvCycle( void ) ``` - Function generates clock signal on MCK pin and on that way starts and performs the desired number of conversion cycles, 
  determined by Down Sampling Factor number.
- ``` uint8_t adc7_readResults( int16_t *voltage ) ``` - Function reads 32bit converted voltage value from AD converter and calculates this value to mV.

**Examples Description**

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes SPI interface and performs configuration for conversion cycles.
- Application Task - (code snippet) - Performs the determined number of conversion cycles which are necessary for averaging.
  When all conversion cycles are done, then reads the converted voltage value.
  Results will be logged on uart terminal. Repeats operation every 200ms.


```.c
void applicationTask()
{
    adc7_startConvCycle();
    
    while (adc7_checkDataReady() == _ADC7_DATA_NOT_READY);
    
    adc7_readResults( &voltageData );
    
    LongToStr( voltageData, text );
    mikrobus_logWrite( "Voltage:  ", _LOG_TEXT );
    mikrobus_logWrite( text, _LOG_TEXT );
    mikrobus_logWrite( "mV", _LOG_LINE );
    
    Delay_ms( 200 );
}
```

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2533/adc-7-click) page.

Other mikroE Libraries used in the example:

- Conversions
- SPI
- UART

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
