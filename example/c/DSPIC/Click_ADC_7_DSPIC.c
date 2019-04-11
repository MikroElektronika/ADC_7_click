/*
Example for ADC_7 Click

    Date          : Jul 2018.
    Author        : Nemanja Medakovic

Test configuration dsPIC :
    
    MCU                : P33FJ256GP710A
    Dev. Board         : EasyPIC Fusion v7
    dsPIC Compiler ver : v7.1.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes SPI interface and performs configuration for conversion cycles.
- Application Task - (code snippet) - Performs the determined number of conversion cycles which are necessary for averaging.
  When all conversion cycles are done, then reads the converted voltage value.
  Results will be logged on uart terminal. Repeats operation every 200ms.

*/

#include "Click_ADC_7_types.h"
#include "Click_ADC_7_config.h"

int16_t voltageData;
char text[ 20 ];

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_AN_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_PWM_PIN, _GPIO_OUTPUT );

    mikrobus_spiInit( _MIKROBUS1, &_ADC7_SPI_CFG[0] );

    mikrobus_logInit( _LOG_USBUART_A, 9600 );
    mikrobus_logWrite( "Initializing...", _LOG_LINE );

    Delay_ms( 100 );
}

void applicationInit()
{
    adc7_spiDriverInit( (T_ADC7_P)&_MIKROBUS1_GPIO, (T_ADC7_P)&_MIKROBUS1_SPI );
    Delay_ms( 300 );
    
    adc7_presetMode( _ADC7_LOW_STATE );
    while (adc7_checkBusy() == _ADC7_DEVICE_IS_BUSY);
    adc7_setConfig( _ADC7_GAIN_DISABLE, _ADC7_DOWNSAMPL_FACT_64, _ADC7_AVERAGING_FILT );
    while (adc7_checkBusy() == _ADC7_DEVICE_IS_BUSY);
    
    mikrobus_logWrite( "ADC 7 is initialized", _LOG_LINE );
    mikrobus_logWrite( "", _LOG_LINE );
    Delay_ms( 100 );
}

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

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}
