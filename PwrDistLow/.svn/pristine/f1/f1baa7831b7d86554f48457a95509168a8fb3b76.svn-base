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
  // CLKSYSHSINOPLLDEFM( ahppre, apb1pre, apb2pre )
  // CLKSYSHSIPLLDEFM( ahbpre, apb1pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv )
  // CLKSYSHSEPLLDEFM( ahbpre, apb1pre, plldiv, pllmult, pllpdiv, pllqdiv, pllrdiv, extfreq )
  CLKSYSHSINOPLLDEFM( AHB_PRESCALE_1, APBX_PRESCALE_1, APBX_PRESCALE_1 )
};

/// AHB 1 clock enable definition
const CODE CLKAHB1ENBDEF  g_tClkAhb1EnbDef =
{
  // use the below listed helper macro to define the AHB
  // CLKAHB1ENBDEFM( dma1, dma2, dmamux, cordic, fmac, flash, crc )
  CLKAHB1ENBDEFM( OFF, OFF, OFF, OFF, OFF, ON, ON )
};

/// AHB 2 clock enable definition
const CODE CLKAHB2ENBDEF  g_tClkAhb2EnbDef =
{
  // use the below listed helper macro to define the AHB
  // CLKAHB2ENBDEFM( gpioa, gpiob, gpioc, gpiod, gpioe, gpiof, gpiog, adc12, adc345, dac1, dac3, rng )
  CLKAHB2ENBDEFM( ON, ON, ON, ON, ON, ON, ON, OFF, OFF, OFF, OFF, OFF )
};

/// AHB 3 clock enable definition
const CODE CLKAHB3ENBDEF  g_tClkAhb3EnbDef =
{
  // use the below listed helper macro to define the AHB
  // CLKAHB3ENBDEFM( qspi )
  CLKAHB3ENBDEFM( OFF )
};

/// APB1 clock enable definition
const CODE CLKAPB1ENBDEF g_tClkApb1EnbDef =
{
  // use the below listed helper macro to define the APB1
  // CLKAPB1ENBDEFM( tim2, tim3, tim4, tim6, tim7, crs, rtc, wdg, spi2, spi3, usart2, usart3, uart4, uart5, i2c1, i2c2, usb, fdcan, pwr, i2c3, lptim1, lpuart1 )
  CLKAPB1ENBDEFM( OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, ON, OFF, OFF, ON )
};

/// APB2 clock enable definition
const CODE CLKAPB2ENBDEF g_tClkApb2EnbDef =
{
  // use the below listed helper macro to define the APB2
  // CLKAPB2ENBDEFM( syscfg, tim1, spi1, tim8, usart1, tim14, tim15, tim16, tim17, tim20, sai1 )
  CLKAPB2ENBDEFM( OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF )
};

/// peripheral clock selection 1 definition
const CODE CLKPERSRCDEF g_tClkPerSrcDef =
{
  // use the below lister helpmer macro to define the peripheral clock selection
  // CLKPERSRC1DEFM( usart1, usart2, usart3, uart4, uart5, lpuart1, i2c1, i2c2, i2c3, lptim1, sai1, i2cs23, fdcan, clk48, adc12, adc345 )
  CLKPERSRC1DEFM( UART_CLKSRC_PCLK2, UART_CLKSRC_PCLK1, UART_CLKSRC_PCLK1, UART_CLKSRC_PCLK1, UART_CLKSRC_PCLK1, UART_CLKSRC_PCLK1, I2C_CLKSRC_PCLK1, I2C_CLKSRC_PCLK1, I2C_CLKSRC_PCLK1, LPTIM_CLKSRC_PCLK1, I2S_CLKSRC_SYSCLK, I2S_CLKSRC_SYSCLK, FDCAN_CLKSRC_PCLK1, CLK48_CLKSRC_HSI48, ADC_CLKSRC_SYSCLK, ADC_CLKSRC_SYSCLK )
};

/**@} EOF Clock_cfg.c */
