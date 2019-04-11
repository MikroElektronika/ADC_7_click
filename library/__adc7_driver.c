/*
    __adc7_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__adc7_driver.h"
#include "__adc7_hal.c"

/* ------------------------------------------------------------------- MACROS */



/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __ADC7_DRV_I2C__
static uint8_t _slaveAddress;
#endif

#define VREF   4076

static uint16_t numSampl;
static float voltRef;
static uint32_t valueLSB;

const uint8_t _ADC7_SINC1_FILT                        = 0x01;
const uint8_t _ADC7_SINC2_FILT                        = 0x02;
const uint8_t _ADC7_SINC3_FILT                        = 0x03;
const uint8_t _ADC7_SINC4_FILT                        = 0x04;
const uint8_t _ADC7_SSINC_FILT                        = 0x05;
const uint8_t _ADC7_FLAT_PASSBAND_FILT                = 0x06;
const uint8_t _ADC7_AVERAGING_FILT                    = 0x07;

const uint8_t _ADC7_DOWNSAMPL_FACT_4                  = 0x02;
const uint8_t _ADC7_DOWNSAMPL_FACT_8                  = 0x03;
const uint8_t _ADC7_DOWNSAMPL_FACT_16                 = 0x04;
const uint8_t _ADC7_DOWNSAMPL_FACT_32                 = 0x05;
const uint8_t _ADC7_DOWNSAMPL_FACT_64                 = 0x06;
const uint8_t _ADC7_DOWNSAMPL_FACT_128                = 0x07;
const uint8_t _ADC7_DOWNSAMPL_FACT_256                = 0x08;
const uint8_t _ADC7_DOWNSAMPL_FACT_512                = 0x09;
const uint8_t _ADC7_DOWNSAMPL_FACT_1024               = 0x0A;
const uint8_t _ADC7_DOWNSAMPL_FACT_2048               = 0x0B;
const uint8_t _ADC7_DOWNSAMPL_FACT_4096               = 0x0C;
const uint8_t _ADC7_DOWNSAMPL_FACT_8192               = 0x0D;
const uint8_t _ADC7_DOWNSAMPL_FACT_16384              = 0x0E;

const uint8_t _ADC7_GAIN_EXPAN_EN                     = 0x01;
const uint8_t _ADC7_GAIN_COMPR_EN                     = 0x02;
const uint8_t _ADC7_GAIN_DISABLE                      = 0x00;

const uint8_t _ADC7_DATA_NOT_READY                    = 0x01;
const uint8_t _ADC7_DATA_IS_READY                     = 0x00;
const uint8_t _ADC7_DEVICE_IS_BUSY                    = 0x01;
const uint8_t _ADC7_DEVICE_NOT_BUSY                   = 0x00;
const uint8_t _ADC7_WRONG_GAIN_CONFIG                 = 0x02;
const uint8_t _ADC7_WRONG_DOWNSAMPL_FACT              = 0x03;
const uint8_t _ADC7_WRONG_FILT_TYPE                   = 0x04;

const uint8_t _ADC7_HIGH_STATE                        = 0x01;
const uint8_t _ADC7_LOW_STATE                         = 0x00;

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __ADC7_DRV_SPI__

void adc7_spiDriverInit(T_ADC7_P gpioObj, T_ADC7_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    hal_gpio_csSet( 1 );
    hal_gpio_rstSet( 0 );
    hal_gpio_pwmSet( 0 );
    numSampl = 4;
    voltRef = VREF;
    valueLSB = 2147483647;
}

#endif
#ifdef   __ADC7_DRV_I2C__

void adc7_i2cDriverInit(T_ADC7_P gpioObj, T_ADC7_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __ADC7_DRV_UART__

void adc7_uartDriverInit(T_ADC7_P gpioObj, T_ADC7_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif

/* ----------------------------------------------------------- IMPLEMENTATION */

uint8_t adc7_setConfig( uint8_t gainConfig, uint8_t downSampFactor, uint8_t filterType )
{
    uint8_t tempData[ 2 ];
    uint8_t count;
    
    if (gainConfig > 3)
    {
        return _ADC7_WRONG_GAIN_CONFIG;
    }
    if ((downSampFactor < 2) || (downSampFactor > 14))
    {
        return _ADC7_WRONG_DOWNSAMPL_FACT;
    }
    if ((filterType < 1) || (filterType > 7))
    {
        return _ADC7_WRONG_FILT_TYPE;
    }
    
    numSampl = 1;
    for (count = 0; count < downSampFactor; count++)
    {
        numSampl *= 2;
    }
    
    switch (gainConfig)
    {
        case 0 :
        {
            voltRef = VREF;
            valueLSB = 2147483647;
        break;
        }
        case 1 :
        {
            voltRef = VREF;
            valueLSB = 1073741823;
        break;
        }
        case 2 :
        {
            voltRef = (float)VREF * 0.8;
            valueLSB = 2147483647;
        break;
        }
        case 3 :
        {
            voltRef = (float)VREF * 0.8;
            valueLSB = 1073741823;
        break;
        }
        default :
        {
        break;
        }
    }
    
    tempData[ 0 ] = 0x80;
    tempData[ 0 ] |= gainConfig << 4;
    tempData[ 0 ] |= downSampFactor;
    tempData[ 1 ] = filterType << 4;
    
    if (adc7_checkBusy() == _ADC7_DEVICE_NOT_BUSY)
    {
        hal_gpio_csSet( 0 );
        hal_spiWrite( tempData, 2 );
        hal_gpio_csSet( 1 );
    
        return _ADC7_DEVICE_NOT_BUSY;
    }
    else
    {
        return _ADC7_DEVICE_IS_BUSY;
    }
}

uint8_t adc7_checkDataReady( void )
{
    if (hal_gpio_anGet())
    {
        return _ADC7_DATA_NOT_READY;
    }
    else
    {
        return _ADC7_DATA_IS_READY;
    }
}

uint8_t adc7_checkBusy( void )
{
    if (hal_gpio_intGet())
    {
        return _ADC7_DEVICE_IS_BUSY;
    }
    else
    {
        return _ADC7_DEVICE_NOT_BUSY;
    }
}

void adc7_setClock( uint8_t state )
{
    if (state)
    {
        hal_gpio_pwmSet( 1 );
    }
    else
    {
        hal_gpio_pwmSet( 0 );
    }
}

void adc7_presetMode( uint8_t state )
{
    if (state)
    {
        hal_gpio_rstSet( 1 );
    }
    else
    {
        hal_gpio_rstSet( 0 );
    }
}

uint8_t adc7_readBytes( uint8_t nBytes, uint8_t *dataOut )
{
    uint8_t tempData[ 256 ] = { 0 };
    
    if (adc7_checkDataReady() == _ADC7_DATA_IS_READY)
    {
        hal_gpio_csSet( 0 );
        hal_spiTransfer( tempData, dataOut, nBytes );
        hal_gpio_csSet( 1 );
        
        return _ADC7_DATA_IS_READY;
    }
    else
    {
        return _ADC7_DATA_NOT_READY;
    }
}

void adc7_startConvCycle( void )
{
    uint16_t count;
    
    for (count = 0; count < numSampl; count++)
    {
        adc7_setClock( 1 );
        Delay_1us();
        adc7_setClock( 0 );
        Delay_1us();
        
        while (adc7_checkBusy());
    }
}

uint8_t adc7_readResults( int16_t *voltage )
{
    int32_t voltData;
    double temp;
    uint8_t buffData[ 4 ];
    uint8_t checkReady;
    
    checkReady = adc7_readBytes( 4, buffData );
    
    if (checkReady)
    {
        return checkReady;
    }
    
    voltData = buffData[ 0 ];
    voltData <<= 8;
    voltData |= buffData[ 1 ];
    voltData <<= 8;
    voltData |= buffData[ 2 ];
    voltData <<= 8;
    voltData |= buffData[ 3 ];
    
    temp = (double)voltData / valueLSB;
    temp *= voltRef;
    *voltage = (int16_t)temp;
    
    return checkReady;
}

/* -------------------------------------------------------------------------- */
/*
  __adc7_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */