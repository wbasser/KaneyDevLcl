/******************************************************************************
 * @file Clock_cfg.c
 *
 * @brief clock configuration implementation
 *
 * This file provides the implementation for the clock configuration
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// global constant parameter initializations -----------------------------------------
/// system clock definition
const CODE CLKSYSDEF  g_tClkSysDef =
{
  // use one of the below listed helper macros to define the system clock
  // CLKSYSMSIDEFM( msiclkrange, ahbpre, apb1pre, apb2pre )
  // CLKSYSMSIPLLDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, pllsaimult, pllsaipdiv, pllsaiqdiv, pllsaiqdiv )
  // CLKSYSHSIPLLDEFM( ahbpre, apb1pre, apb2pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, pllsaidiv, pllsaimult, pllsaiqdiv, pllsaiqdiv )
  // CLKSYSHSEPLLDEFM( ahbpre, apb1pre, apb2pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, pllsaidiv, pllsaimult, pllsaiqdiv, pllsaiqdiv, divx2 )
  // CLKSYSHSIPLLDEFM( APBX_PRESCALE_1, APBX_PRESCALE_1, APBX_PRESCALE_2, PLLX_DIVIDER_2, 24, PLLX_DIVIDER_4, PLLX_DIVIDER_2, PLLX_DIVIDER_2, PLLX_DIVIDER_24, PLLX_DIVIDER_2, PLLX_DIVIDER_2, PLLX_DIVIDER_2 )
  CLKSYSHSIPLLDEFM( APBX_PRESCALE_1, APBX_PRESCALE_1, APBX_PRESCALE_2, PLLX_DIVIDER_2, 24, PLLP_DIVIDER_32, PLLX_DIVIDER_8, PLLX_DIVIDER_6, 24, PLLX_DIVIDER_2, PLLP_DIVIDER_32, PLLX_DIVIDER_2 )
};

/// AHB clock enable definition
const CODE CLKAHB1ENBDEF  g_tClkAhb1EnbDef =
{
  // use the below listed helper macro to define the AHB
  // CLKAHB1ENBDEFM( dma1, dma2, dmamux, crc, touch )
  CLKAHB1ENBDEFM( ON, ON, ON, ON, OFF )
};

const CODE CLKAHB2ENBDEF  g_tClkAhb2EnbDef =
{
  // use the below listed helper macro to define the AHB
  // CLKAHB2ENBDEFM( gpioa, gpiob, gpioc, gpiod, gpioe, gpioh, adc, aes1 )
  CLKAHB2ENBDEFM( ON, ON, ON, ON, ON, ON, ON, OFF )
};

const CODE CLKAHB3ENBDEF  g_tClkAhb3EnbDef =
{
  // use the below listed helper macro to define the AHB 
  // CLKAHB3ENBDEFM( qpsi, pka, aes2, rng, hsem, ipcc, flash )
  CLKAHB3ENBDEFM( OFF, OFF, OFF, OFF, OFF, OFF, ON )
};

/// APB1 clock enable definition
const CODE CLKAPB1ENBDEF g_tClkApb1EnbDef =
{
  // use the below listed helper macro to define the APB1
  // CLKAPB1ENBDEFM( tim2, lcd, spi2, i2c1, i2c3, crs, usb, lptim1, lpuart1, lptim2 )
  CLKAPB1ENBDEFM( ON, OFF, ON, ON, ON, OFF, ON, OFF, ON, ON )
};

/// APB2 clock enable definition
const CODE CLKAPB2ENBDEF g_tClkApb2EnbDef =
{
  // use the below listed helper macro to define the APB2
  // CLKAPB2ENFDEFM( tim1, spi1, usart1, tim15, tim16, sai1 )
  CLKAPB2ENFDEFM( ON, ON, ON, OFF, OFF, OFF )
};

/// peripheral clock selection definition
const CODE CLKPERSRCDEF g_tClkPerSrcDef =
{
  // use the below lister helpmer macro to define the peripheral clock selection
  // CLKPERSRCDEFM( usart1, lpuart1, i2c1, i2c3, lptim1, lptm2, sai, usb, adc, rng )
  CLKPERSRCDEFM( SER_CLKSRC_HSI16, SER_CLKSRC_HSI16, SER_CLKSRC_HSI16, SER_CLKSRC_HSI16, LPTIM_CLKSRC_HSI16, LPTIM_CLKSRC_HSI16, SAI_CLKSRC_PLLP, USB_CLKSRC_PLLQ, ADC_CLKSRC_NONE, RNG_CLKSRC_LSICLK )
};

/**@} EOF Clock_cfg.c */
