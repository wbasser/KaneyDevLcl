/******************************************************************************
 * @file  Clock_def.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_DEF_H
#define _CLOCK_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro to define the system clock using the MSI
#define CLKSYSMSIDEFM( msiclkrange, ahbpre, apb1pre, apb2pre ) \
  .eSysClkSrc     = SYSLCK_SRC_MSI, \
  .eMsiClkRange   = msiclkrange, \
  .eAhbPrescale   = ahbpre, \
  .eApb1Prescale  = apb1pre, \
  .eApb2Prescale  = apb2pre

/// define the helper macro to define the system clock using the MSI/PLL
#define CLKSYSMSIPLLDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, pllmult, plldiv, pllpdiv, pllqdiv, pllrdiv, pllsaimult, pllsaipdiv, pllsaiqdiv, pllsaiRdiv ) \
  .eSysClkSrc     = SYSCLK_SRC_PLL, \
  .ePllClkSrc     = PLLCLK_SRC_MSI, \
  .eMsiClkRange   = msiclkrange, \
  .eAhbPrescale   = ahbpre, \
  .eApb1Prescale  = apb1pre, \
  .eApb2Prescale  = apb2pre, \
  .ePllDiv        = plldiv, \
  .nPllMult       = pllmult, \
  .ePllPDIv       = pllpdiv, \
  .ePllQDiv       = pllqdiv, \
  .ePllRDiv       = pllrdiv, \
  .nPllSaiMult    = pllsaiMult, \
  .ePllSaiPDiv    = pllsaipdiv, \
  .ePllSaiQDiv    = pllsaiqdiv, \
  .ePllSaiRDiv    = pllsairdiv

#define CLKSYSHSIPLLDEFM( ahbpre, apb1pre, apb2pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, pllsaimult, pllsaipdiv, pllsaiqdiv, pllsairdiv ) \
  .eSysClkSrc     = SYSCLK_SRC_PLL, \
  .ePllClkSrc     = PLLCLK_SRC_HSI16, \
  .eAhbPrescale   = ahbpre, \
  .eApb1Prescale  = apb1pre, \
  .eApb2Prescale  = apb2pre, \
  .ePllDiv        = plldiv, \
  .nPllMult       = pllmult, \
  .ePllPDiv       = pllpdiv, \
  .ePllQDiv       = pllqdiv, \
  .ePllRDiv       = pllrdiv, \
  .nPllSaiMult    = pllsaimult, \
  .ePllSaiPDiv    = pllsaipdiv, \
  .ePllSaiQDiv    = pllsaiqdiv, \
  .ePllSaiRDiv    = pllsairdiv

#define CLKSYSHSEPLLDEFM( ahbpre, apb1pre, apb2pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, pllsaipmult, pllsaiqdiv, pllsairdiv, divx2 ) \
  .eSysClkSrc     = SYSCLK_SRC_PLL, \
  .ePllClkSrc     = PLLCLK_SRC_HSE, \
  .eAhbPrescale   = ahbpre, \
  .eApb1Prescale  = apb1pre, \
  .eApb2Prescale  = apb2pre, \
  .ePllDiv        = plldiv, \
  .nPllMult       = pllmult, \
  .ePllPDIv       = pllpdiv, \
  .ePllQDiv       = pllqdiv, \
  .ePllRDiv       = pllrdiv, \
  .nPllSaiMult    = pllsaimult, \
  .ePllSaiPDiv    = pllsaipdiv, \
  .ePllSaiQDiv    = pllsaiqdiv, \
  .ePllSaiRDiv    = pllsairdiv, \
  .bHsePreDiv2Enb = divx2

/// define the helper macro to define the AHB #1 clock enable
#define CLKAHB1ENBDEFM( dma1, dma2, dmamux, crc, touch ) \
  .bDMA1          = dma1, \
  .bDMA2          = dma2, \
  .bDMAMUX1       = dmamux, \
  .bCrc           = crc, \
  .bTouch         = touch

/// define the helper macro to define the AHB #2 clock enable
#define CLKAHB2ENBDEFM( gpioa, gpiob, gpioc, gpiod, gpioe, gpioh, adc, aes1 ) \
  .bGPIOA         = gpioa, \
  .bGPIOB         = gpiob, \
  .bGPIOC         = gpioc, \
  .bGPIOD         = gpiod, \
  .bGPIOE         = gpioe, \
  .bGPIOH         = gpioh, \
  .bADC           = adc, \
  .bAES1          = aes1

/// define the helper macro to define the AHB #1 clock enable
#define CLKAHB3ENBDEFM( qpsi, pka, aes2, rng, hsem, ipcc, flash ) \
  .bQSPI          = qpsi, \
  .bPKA           = pka, \
  .bAES2          = aes2, \
  .bRNG           = rng, \
  .bHSEM          = hsem, \
  .bIPCC          = ipcc, \
  .bFlash         = flash, \

/// define the helper macro to define the APB1 clock enables
#define CLKAPB1ENBDEFM( tim2, lcd, spi2, i2c1, i2c3, crs, usb, lptim1, lpuart1, lptim2 ) \
  .bTim2          = tim2, \
  .bLcd           = lcd, \
  .bSpi2          = spi2, \
  .bI2c1          = i2c1, \
  .bI2c3          = i2c3, \
  .bCRS           = crs, \
  .bUSB           = usb, \
  .bLpTim1        = lptim1, \
  .bLpUart1       = lpuart1, \
  .bLpTim2        = lptim2 \


/// define the helper macro to define the APB2 clock enables
#define CLKAPB2ENFDEFM( tim1, spi1, usart1, tim16, tim17, sai1 ) \
  .bTim1          = tim1, \
  .bSpi1          = spi1, \
  .bUsart1        = usart1, \
  .bTim16         = tim16, \
  .bTim17         = tim17, \
  .bSai1          = sai1, \

/// define the helper macro to define the peripheral clock source
#define CLKPERSRCDEFM( usart1, lpuart1, i2c1, i2c3, lptim1, lptm2, sai, usb, adc, rng ) \
  .nUsart1ClkSrc  = usart1, \
  .nLpUart1ClkSrc = lpuart1, \
  .nI2c1ClkSrc    = i2c1, \
  .nI2c3ClkSrc    = i2c3, \
  .nLpTim1ClkSrc  = lptim1, \
  .nLpTim2ClkSrc  = lptim1, \
  .nSaiClkSrc     = sai, \
  .nUsbClkSrc     = usb, \
  .nAdcClkSrc     = adc, \
  .nRngClkSrc     = rng, \

// enumerations ---------------------------------------------------------------
/// enumerate the sources for the system clock
typedef enum _SYSCLKSRC
{
  SYSCLK_SRC_MSI = 0,             ///< MSI 
  SYSCLK_SRC_HSI16,               ///< High Speed Clock Internal
  SYSCLK_SRC_HSE,                 ///< High Speed Clock External
  SYSCLK_SRC_PLL,                  ///< PLL clock
  SYSCLK_SRC_MAX
} SYSCLKSRC;

/// enumerate the MSI clock range
typedef enum _MSICLKRANGE
{
  MSICLK_RANGE_100KHZ = 0,        ///< 100KHZ
  MSICLK_RANGE_200KHZ,            ///< 200KHZ
  MSICLK_RANGE_400KHZ,            ///< 400KHZ
  MSICLK_RANGE_800KHZ,            ///< 800KHZ
  MSICLK_RANGE_1MHZ,              ///< 1MHZ
  MSICLK_RANGE_2MHZ,              ///< 2MHZ
  MSICLK_RANGE_4MHZ,              ///< 4MHZ
  MSICLK_RANGE_8MHZ,              ///< 8MHZ
  MSICLK_RANGE_16MHZ,             ///< 16MHZ
  MSICLK_RANGE_24MHZ,             ///< 24MHZ
  MSICLK_RANGE_32MHZ,             ///< 32MHZ
  MSICLK_RANGE_48MHZ,             ///< 48MHZ
  MSICLK_RANGE_MAX
} MSICLKRANGE;

/// enumarate the PLL clock source
typedef enum _PLLCLKSRC
{
  PLLCLK_SRC_NONE = 0,            ///< no 
  PLLCLK_SRC_MSI,                 ///< MSI 
  PLLCLK_SRC_HSI16,               ///< High Speed Clock Internal
  PLLCLK_SRC_HSE,                 ///< High Speed Clock External
  PLLCLK_SRC_MAX,
} PLLCLKSRC;

/// enumerate the APB1/2 prescale
typedef enum _ABPXPRESCALE
{
  APBX_PRESCALE_1 = 0,            ///< prescale of 1
  APBX_PRESCALE_2 = 4,            ///< prescale of 2
  APBX_PRESCALE_4,                ///< prescale of 4
  APBX_PRESCALE_8,                ///< prescale of 8
  APBX_PRESCALE_16,               ///< prescale of 16
  APBX_PRESCALE_MAX
} ABPXPRESCALE;

/// enumerate the AHB prescale
typedef enum _AHBPRESCALE
{
  AHB_PRESCALE_1 = 0,             ///< prescale of 1
  AHB_PRESCALE_3,                 ///< prescale of 3
  AHB_PRESCALE_5,                 ///< prescale of 5
  AHB_PRESCALE_6 = 5,             ///< prescale of 6
  AHB_PRESCALE_10,                ///< prescale of 10
  AHB_PRESCALE_32,                ///< prescale of 32
  AHB_PRESCALE_2 = 8,             ///< prescale of 2
  AHB_PRESCALE_4,                 ///< prescale of 4
  AHB_PRESCALE_8,                 ///< prescale of 8
  AHB_PRESCALE_64,                ///< prescale of 64
  AHB_PRESCALE_128,               ///< prescale of 128
  AHB_PRESCALE_256,               ///< prescale of 256
  AHB_PRESCALE_512,               ///< prescale of 512
  AHB_PRESCALE_MAX
} AHBPRESCALE;

/// enumerate the MCO clock source
typedef enum _MCOCLKSRC
{
  MCO_CLKSRC_NONE = 0,            ///< no MCO clock source
  MCO_CLKSRC_SYSCLK,              ///< system clock
  MCO_CLKSRC_MSI,                 ///< MSI clock
  MCO_CLKSRC_HSI16,               ///< HSI*16 clock
  MCO_CLKSRC_HSE,                 ///< HSE clock
  MCO_CLKSRC_PLL,                 ///< PLL
  MCO_CLKSRC_LSI1,                ///< LSI1
  MCO_CLKSRC_LSI2,                ///< LSI2
  MCO_CLKSRC_LSE,                 ///< LSE
  MCO_CLKSRC_RC48,                ///< RC48
  MCO_CLKSRC_MAX
} MCOCLKSRC;

/// enumerate the MCO clock prescale
typedef enum _MCOPRESCALE
{
  MCO_PRESCALE_1 = 0,             ///< prescale of 1
  MCO_PRESCALE_2,                 ///< prescale of 2
  MCO_PRESCALE_4,                 ///< prescale of 4
  MCO_PRESCALE_8,                 ///< prescale of 8
  MCO_PRESCALE_16,                ///< prescale fo 16
  MCO_PRESCALE_MAX
} MCOPRESCALE;

/// enumerate the SMPSCLKSRC
typedef enum _SMPSCLKSRC
{
  SMPS_CLKSRC_HSI16 = 0,          ///< HSI16 clock selection
  SMPS_CLKSRC_MSI,                ///< MSI clock select
  SMPS_CLKSRC_HSE,                ///< HSE clock select
  SMPS_CLKSRC_MAX
} SMPSCLKSRC;

/// enumerate the serial clock select
typedef enum _SERCLKSRC
{
  SER_CLKSRC_PCLK = 0,         ///< PCLK clock source
  SER_CLKSRC_SYSCLK,           ///< System clock source
  SER_CLKSRC_HSI16,            ///< HSI clock source
  SER_CLKSRC_LSE,              ///< LSE clock source
  SER_CLKSRC_MAX
} SERCLKSRC;

/// enumerate the LPTIM clock select
typedef enum _LPTIMCLKSRC
{
  LPTIM_CLKSRC_PCLK = 0,                        ///< PCLK clock source
  LPTIM_CLKSRC_SYSCLK,                          ///< System clock source
  LPTIM_CLKSRC_HSI16,                           ///< HSI clock source
  LPTIM_CLKSRC_LSI,                             ///< LSI clock sourc
  LPTIM_CLKSRC_LSE,                             ///< LSE clock source
  LPTIM_CLKSRC_MAX
} LPTIMCLKSRC;

/// enumerate the SAI clock source
typedef enum _SAICLKSRC
{
  SAI_CLKSRC_PLLSAI1P = 0,               ///< PLLSAI PCLK
  SAI_CLKSRC_PLLP,                      ///< PLLP clock
  SAI_CLKSRC_HSI16,                     ///< HSI16 clock
  SAI_CLKSRC_EXTERN,                    ///< external clock
  SAI_CLKSRC_MAX
} SAICLKSRC;

/// enumerate the USB clock source
typedef enum _USBCLKSRC
{
  USB_CLKSRC_HSI48 = 0,                 ///< HSI48 clock
  USB_CLKSRC_PLLSAI1Q,                  ///< PLLSAIQ clock
  USB_CLKSRC_PLLQ,                      ///< PLL Q clock
  USB_CLKSRC_MSI,                       ///< MSI
  USB_CLKSRC_MAX
} USBCLKSRC;

/// enumerate the ADC clock source
typedef enum _ADCCLKSRC
{
  ADC_CLKSRC_NONE = 0,                  ///< no clock
  ADC_CLKSRC_PLLSAI1R,                  ///< PLLSAIR clock
  ADC_CLKSRC_PLLP,                      ///< PLL P Clock
  ADC_CLKSRC_SYSCLK,                    ///< system clock
  ADC_CLKSRC_MAX
} ADCCLKSRC;

/// enumerate the RNG clock source
typedef enum _RNGCLKSRC
{
  RNG_CLKSRC_USBCLKSRC = 0,             ///< same clock as USB48
  RNG_CLKSRC_LSICLK,                    ///< LSI clock
  RNG_CLKSRC_LSECLK,                    ///< LSE clock
  RNG_CLKSRC_MAX
} RNGCLKSRC;

/// enumerate the PLL divider
typedef enum _PLLXDIVIDER
{
  PLLX_DIVIDER_1 = 0,
  PLLX_DIVIDER_2,
  PLLX_DIVIDER_3,
  PLLX_DIVIDER_4,
  PLLX_DIVIDER_5,
  PLLX_DIVIDER_6,
  PLLX_DIVIDER_7,
  PLLX_DIVIDER_8,
  PLLX_DIVIDER_MAX
} PLLXDIVIDER;

/// enumerate the PLL-P divider
typedef enum _PLLPDIVIDER
{
  PLLP_DIVIDER_2 = 1,
  PLLP_DIVIDER_3,
  PLLP_DIVIDER_4,
  PLLP_DIVIDER_5,
  PLLP_DIVIDER_6,
  PLLP_DIVIDER_7,
  PLLP_DIVIDER_8,
  PLLP_DIVIDER_9,
  PLLP_DIVIDER_10,
  PLLP_DIVIDER_11,
  PLLP_DIVIDER_12,
  PLLP_DIVIDER_13,
  PLLP_DIVIDER_14,
  PLLP_DIVIDER_15,
  PLLP_DIVIDER_16,
  PLLP_DIVIDER_17,
  PLLP_DIVIDER_18,
  PLLP_DIVIDER_19,
  PLLP_DIVIDER_20,
  PLLP_DIVIDER_21,
  PLLP_DIVIDER_22,
  PLLP_DIVIDER_23,
  PLLP_DIVIDER_24,
  PLLP_DIVIDER_25,
  PLLP_DIVIDER_26,
  PLLP_DIVIDER_27,
  PLLP_DIVIDER_28,
  PLLP_DIVIDER_29,
  PLLP_DIVIDER_30,
  PLLP_DIVIDER_31,
  PLLP_DIVIDER_32,
  PLLP_DIVIDER_MAX
} PLLPDIVIDER;

// structures -----------------------------------------------------------------
/// define the system clock definition structure
typedef struct _CLKSYSDEF
{
  SYSCLKSRC     eSysClkSrc;       ///< clock source
  MSICLKRANGE   eMsiClkRange;     ///< MSI clock range
  PLLCLKSRC     ePllClkSrc;       ///< PLL clock source
  BOOL          bHsePreDiv2Enb;   ///< enable the HSE divide by 2 prescale
  PLLXDIVIDER   ePllDiv;          ///< PLL divider
  U8            nPllMult;         ///< PLL multiplier
  PLLPDIVIDER   ePllPDiv;         ///< PLL PCLck output prescale
  PLLXDIVIDER   ePllQDiv;         ///< PLL QCLck output prescale
  PLLXDIVIDER   ePllRDiv;         ///< PLL RCLck output prescale
  U8            nPllSaiMult;      ///< PLLSAI multiplier
  PLLPDIVIDER   ePllSaiPDiv;      ///< PLLSAI PCLck output prescale
  PLLXDIVIDER   ePllSaiQDiv;      ///< PLLSAI QCLck output prescale
  PLLXDIVIDER   ePllSaiRDiv;      ///< PLLSAI RCLck output prescale
  AHBPRESCALE   eAhbPrescale;     ///< AHB prescale
  ABPXPRESCALE  eApb1Prescale;    ///< APB1 prescale
  ABPXPRESCALE  eApb2Prescale;    ///< APB2 prescale
} CLKSYSDEF, *PCLKSYSDEF;
#define CLKSYSDEF_SIZE                  sizeof( CLKSYSDEF )

/// define the AHB1 clock enable structure
typedef struct _CLKAHB1ENBDEF
{
  BOOL          bDMA1     : 1;      ///< DMA1
  BOOL          bDMA2     : 1;      ///< DMA2
  BOOL          bDMAMUX1  : 1;      ///< DMAMUX
  BOOL          bCrc      : 1;      ///< CRC
  BOOL          bTouch    : 1;      ///< touch
} CLKAHB1ENBDEF, *PCLKAHB1ENBDEF;
#define CLKAHB1ENBDEF_SIZE              sizeof( CLKAHB1ENBDEF )

/// define the AHB2 clock enable structure
typedef struct _CLKAHB2ENBDEF
{
  BOOL          bGPIOA    : 1;      ///< GPIOA
  BOOL          bGPIOB    : 1;      ///< GPIOB
  BOOL          bGPIOC    : 1;      ///< GPIOC
  BOOL          bGPIOD    : 1;      ///< GPIOD
  BOOL          bGPIOE    : 1;      ///< GPIOE
  BOOL          bGPIOH    : 1;      ///< GPIOH
  BOOL          bADC      : 1;      ///< ADC
  BOOL          bAES1     : 1;      ///< AES1
} CLKAHB2ENBDEF, *PCLKAHB2ENBDEF;
#define CLKAHB2ENBDEF_SIZE                sizeof( CLKAHB2ENBDEF )

/// define the AHB3 clock enable structure
typedef struct _CLKAHB3ENBDEF
{
  BOOL          bQSPI     : 1;      ///< QSPI
  BOOL          bPKA      : 1;      ///< PKA
  BOOL          bAES2     : 1;      ///< AES2
  BOOL          bRNG      : 1;      ///< RNG
  BOOL          bHSEM     : 1;      ///< HSEM
  BOOL          bIPCC     : 1;      ///< IPCC
  BOOL          bFlash    : 1;      ///< Flash
} CLKAHB3ENBDEF, *PCLKAHB3ENBDEF;
#define CLKAHB3ENBDEF_SIZE                sizeof( CLKAHB3ENBDEF )

/// define the APB1 clock enable structure
typedef struct _CLKAPB1ENBDEF
{
  BOOL          bTim2   : 1;      ///< timer #2
  BOOL          bLcd    : 1;      ///< LCD
  BOOL          bRTC    : 1;      ///< RTC
  BOOL          bWWDG   : 1;      ///< WDG
  BOOL          bSpi2   : 1;      ///< SPI2
  BOOL          bI2c1   : 1;      ///< IC2C1
  BOOL          bI2c3   : 1;      ///< IC2C3
  BOOL          bCRS    : 1;      ///< CRS
  BOOL          bUSB    : 1;      ///< USB
  BOOL          bLpTim1 : 1;      ///< low power timer #1
  BOOL          bLpUart1: 1;      ///< low power UART #1
  BOOL          bLpTim2 : 1;      ///< low power timer #2
} CLKAPB1ENBDEF, *PCLKAPB1ENBDEF;
#define CLKAPB1ENBDEF_SIZE              sizeof( CLKAPB1ENBDEF )

/// define the APB2 clock enable structure
typedef struct _CLKAPB2ENBDEF
{
  BOOL          bTim1   : 1;      ///< timer #1
  BOOL          bSpi1   : 1;      ///< SPI1
  BOOL          bUsart1 : 1;      ///< USART #1
  BOOL          bTim16  : 1;      ///< timer #16
  BOOL          bTim17  : 1;      ///< timer #17
  BOOL          bSai1   : 1;      ///< SAI1
} CLKAPB2ENBDEF, *PCLKAPB2ENBDEF;
#define CLKAPB2ENBDEF_SIZE              sizeof( CLKAPB2ENBDEF )

/// define clock peripheral clock source structure
typedef struct _CLKPERSRCDEF
{
  SERCLKSRC     nUsart1ClkSrc;        ///< USART1 clock source
  SERCLKSRC     nLpUart1ClkSrc;       ///< LPUART clock source
  SERCLKSRC     nI2c1ClkSrc;          ///< IC21 clock source
  SERCLKSRC     nI2c3ClkSrc;          ///< IC23 clock source
  LPTIMCLKSRC   nLpTim1ClkSrc;        ///< LP timer 1 clock source
  LPTIMCLKSRC   nLpTim2ClkSrc;        ///< LP timer 2 clock source
  SAICLKSRC     nSaiClkSrc;           ///< SAI clock source
  USBCLKSRC     nUsbClkSrc;           ///< USB clock source
  ADCCLKSRC     nAdcClkSrc;           ///< ADC clock source
  RNGCLKSRC     nRngClkSrc;           ///< RNG clock source
} CLKPERSRCDEF, *PCLKPERSRCDEF;
#define CLKPERSRCDEF_SIZE                sizeof( CLKPERSRCDEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCK_DEF_H

