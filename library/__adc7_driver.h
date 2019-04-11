/*
    __adc7_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __adc7_driver.h
@brief    ADC_7 Driver
@mainpage ADC_7 Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   ADC7
@brief      ADC_7 Click Driver
@{

| Global Library Prefix | **ADC7** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **Jul 2018.**      |
| Developer             | **Nemanja Medakovic**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _ADC7_H_
#define _ADC7_H_

/** 
 * @macro T_ADC7_P
 * @brief Driver Abstract type 
 */
#define T_ADC7_P    const uint8_t*

/** @defgroup ADC7_COMPILE Compilation Config */              /** @{ */

   #define   __ADC7_DRV_SPI__                            /**<     @macro __ADC7_DRV_SPI__  @brief SPI driver selector */
//  #define   __ADC7_DRV_I2C__                            /**<     @macro __ADC7_DRV_I2C__  @brief I2C driver selector */                                          
// #define   __ADC7_DRV_UART__                           /**<     @macro __ADC7_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup ADC7_VAR Variables */                           /** @{ */

/** Filter Type */
extern const uint8_t _ADC7_SINC1_FILT            ;
extern const uint8_t _ADC7_SINC2_FILT            ;
extern const uint8_t _ADC7_SINC3_FILT            ;
extern const uint8_t _ADC7_SINC4_FILT            ;
extern const uint8_t _ADC7_SSINC_FILT            ;
extern const uint8_t _ADC7_FLAT_PASSBAND_FILT    ;
extern const uint8_t _ADC7_AVERAGING_FILT        ;

/** Down Sampling Factor */
extern const uint8_t _ADC7_DOWNSAMPL_FACT_4      ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_8      ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_16     ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_32     ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_64     ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_128    ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_256    ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_512    ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_1024   ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_2048   ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_4096   ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_8192   ;
extern const uint8_t _ADC7_DOWNSAMPL_FACT_16384  ;

/** Gain Configuration */
extern const uint8_t _ADC7_GAIN_EXPAN_EN         ;
extern const uint8_t _ADC7_GAIN_COMPR_EN         ;
extern const uint8_t _ADC7_GAIN_DISABLE          ;

/** Data and Busy Check Results */
extern const uint8_t _ADC7_DATA_NOT_READY        ;
extern const uint8_t _ADC7_DATA_IS_READY         ;
extern const uint8_t _ADC7_DEVICE_IS_BUSY        ;
extern const uint8_t _ADC7_DEVICE_NOT_BUSY       ;
extern const uint8_t _ADC7_WRONG_GAIN_CONFIG     ;
extern const uint8_t _ADC7_WRONG_DOWNSAMPL_FACT  ;
extern const uint8_t _ADC7_WRONG_FILT_TYPE       ;

extern const uint8_t _ADC7_HIGH_STATE            ;
extern const uint8_t _ADC7_LOW_STATE             ;

                                                                       /** @} */
/** @defgroup ADC7_TYPES Types */                             /** @{ */



                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup ADC7_INIT Driver Initialization */              /** @{ */

#ifdef   __ADC7_DRV_SPI__
void adc7_spiDriverInit(T_ADC7_P gpioObj, T_ADC7_P spiObj);
#endif
#ifdef   __ADC7_DRV_I2C__
void adc7_i2cDriverInit(T_ADC7_P gpioObj, T_ADC7_P i2cObj, uint8_t slave);
#endif
#ifdef   __ADC7_DRV_UART__
void adc7_uartDriverInit(T_ADC7_P gpioObj, T_ADC7_P uartObj);
#endif

                                                                       /** @} */
/** @defgroup ADC7_FUNC Driver Functions */                   /** @{ */

/**
 * @brief Configuration Set function
 *
 * @param[in] gainConfig  Gain configuration (0-3)
 * @param[in] downSampFactor  Down Sampling Factor (2-14)
 * @param[in] filterType  Filter Type (1-7)
 *
 * @returns Is device busy or not
 *
 * Function performs the device configuration by sending configuration data to configure the next conversion cycle.
 * @note
 * Configuration should be performed only if device is not busy.
 * If you want to continue the conversions with the same configuration, you should write 0x00 when you read data.
 * It's not necessary to repeat configuration set function.
 */
uint8_t adc7_setConfig( uint8_t gainConfig, uint8_t downSampFactor, uint8_t filterType );

/**
 * @brief Data Ready Check function
 *
 * @returns 0 - Data is ready, 1 - Data is not ready
 *
 * Function checks is data ready for reading.
 */
uint8_t adc7_checkDataReady( void );

/**
 * @brief Busy Check function
 *
 * @returns 0 - Device is not busy, 1 - Device is busy
 *
 * Function checks is device busy (is conversion cycle finished).
 */
uint8_t adc7_checkBusy( void );

/**
 * @brief Clock Set function
 *
 * @param[in] state  0 - low state, 1 - high state
 *
 * Function sets clock (MCK) pin to desired state.
 * @note
 * By setting this pin to high state user can start one conversion cycle.
 * After that this pin should be set to low state.
 */
void adc7_setClock( uint8_t state );

/**
 * @brief Preset Mode function
 *
 * @param[in] state  0 - Normal Mode, 1 - Preset Mode
 *
 * Function puts the device to Preset or to Normal Mode.
 */
void adc7_presetMode( uint8_t state );

/**
 * @brief Bytes Read function
 *
 * @param[in] nBytes  Number of bytes to be read
 * @param[out] dataOut  Memory where data bytes be stored
 *
 * @returns Is data ready or not
 *
 * Function reads the desired number of data bytes (32-bit converted data from ADC + configuration byte/bytes).
 */
uint8_t adc7_readBytes( uint8_t nBytes, uint8_t *dataOut );

/**
 * @brief Start Conversion function
 *
 * Function generates clock signal on MCK pin and on that way starts and performs the desired number of conversion cycles,
 * determined by Down Sampling Factor number.
 * @note
 * When all conversion cycles be done, then data should be read.
 */
void adc7_startConvCycle( void );

/**
 * @brief Results Read function
 *
 * @param[out] voltage  Memory where converted voltage value be stored
 *
 * @returns Is data ready or not
 *
 * Function reads 32bit converted voltage value from AD converter and calculates this value to mV.
 * @note
 * The voltage data will be read only if data is ready for reading.
 */
uint8_t adc7_readResults( int16_t *voltage );

                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_ADC_7_STM.c
    @example Click_ADC_7_TIVA.c
    @example Click_ADC_7_CEC.c
    @example Click_ADC_7_KINETIS.c
    @example Click_ADC_7_MSP.c
    @example Click_ADC_7_PIC.c
    @example Click_ADC_7_PIC32.c
    @example Click_ADC_7_DSPIC.c
    @example Click_ADC_7_AVR.c
    @example Click_ADC_7_FT90x.c
    @example Click_ADC_7_STM.mbas
    @example Click_ADC_7_TIVA.mbas
    @example Click_ADC_7_CEC.mbas
    @example Click_ADC_7_KINETIS.mbas
    @example Click_ADC_7_MSP.mbas
    @example Click_ADC_7_PIC.mbas
    @example Click_ADC_7_PIC32.mbas
    @example Click_ADC_7_DSPIC.mbas
    @example Click_ADC_7_AVR.mbas
    @example Click_ADC_7_FT90x.mbas
    @example Click_ADC_7_STM.mpas
    @example Click_ADC_7_TIVA.mpas
    @example Click_ADC_7_CEC.mpas
    @example Click_ADC_7_KINETIS.mpas
    @example Click_ADC_7_MSP.mpas
    @example Click_ADC_7_PIC.mpas
    @example Click_ADC_7_PIC32.mpas
    @example Click_ADC_7_DSPIC.mpas
    @example Click_ADC_7_AVR.mpas
    @example Click_ADC_7_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __adc7_driver.h

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