/******************************************************************************
 * @file Clock.c
 *
 * @brief clock module implementation
 *
 * This file clock module initialization and up/down scaling
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the internal oscillator frequencies
#define CLK_HSI16_FREQ                          ( 16000000ul )
#define CLK_HSI48_FREQ                          ( 48000000ul )
#define CLK_LSI_FREQ                            ( 32000ul )
#define CLK_LSE_FREQ                            ( 32768ul )

// enumerations ---------------------------------------------------------------
/// enumerate the selection of the peripheral bus interfaces
typedef enum _APBSELECT
{
  APBSELECT_APB1 = 0,
  APBSELECT_APB2,
  APBSELECT_MAX
} APBSELECT;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   uSysClkFreq;
static  U32   uHseClkFreq;
static  U32   uPllPClkFreq;
static  U32   uPllQClkFreq;
static  U32   uPllRClkFreq;
static  U32   uPclk1Freq;
static  U32   uPclk2Freq;

// local function prototypes --------------------------------------------------
static  void  ConfigurePll( PCLKSYSDEF ptClkDef );
static  void  ConfigureAhb( void );
static  void  ConfigureApb( void );
static  void  ConfigurePerClocks( void );
static  void  ComputeSystemClockFreq( void );
static  U32   GetI2cClockFreq( I2CCLKSRC eClkSel );
static  U32   GetUsartClockFreq( CLOCKMUXSEL eClockMuxSel, UARTCLKSRC eClkSel );
static  U32   GetAdcClockFreq( ADCCLKSRC eClkSel );

// constant parameter initializations -----------------------------------------
// define the AHB prescale table
static const U32 auAhbPrescale[ ] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 
  1, 2, 3, 4, 6, 7, 8, 9  
};

/// define the APBX prescale table
static const U32 auApbxPrescale[ ] =
{
  0, 0, 0, 1, 2, 3, 4, 5
};

/******************************************************************************
 * @function Clock_Initialization
 *
 * @brief clock initialization
 *
 * This function will initialize the clock control module
 *
 * @return True if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL Clock_Initialize( void )
{
  PCLKSYSDEF  ptClkDef;
  U32         uTemp;
  
  // clear the PLL clock frequencies
  uPllPClkFreq = uPllQClkFreq = uPllRClkFreq = 0;

  // set the pointer
  ptClkDef = ( PCLKSYSDEF )&g_tClkSysDef;
  
  // determine the clock source
  switch( ptClkDef->eSysClkSrc )
  {
    case SYSCLK_SRC_HSI16 :
      // turn on the clock/wait ready
      RCC->CR  |= RCC_CR_HSION;
      while(( RCC->CR & RCC_CR_HSIRDY ) != RCC_CR_HSIRDY);
      break;
      
    case SYSCLK_SRC_HSE :
      RCC->CR |= RCC_CR_HSEON;
      while(( RCC->CR & RCC_CR_HSERDY ) != RCC_CR_HSERDY );
      uHseClkFreq = ptClkDef->uExtFreq;
      break;
      
    case SYSCLK_SRC_PLLR :
      // determine the source of the 
      switch( ptClkDef->ePllClkSrc )
      {
        case PLLCLK_SRC_HSI16 :
          // turn on the clock/wait ready
          RCC->CR  |= RCC_CR_HSION;
          while(( RCC->CR & RCC_CR_HSIRDY ) != RCC_CR_HSIRDY );
          break;
          
        case PLLCLK_SRC_HSE :
          RCC->CR |= RCC_CR_HSEON;
          while(( RCC->CR & RCC_CR_HSERDY ) != RCC_CR_HSERDY );
          break;
          
        default :
          //#error "Illegal PLL clock source!"
          break;
      }
      
      // now set upt the PLL
      ConfigurePll( ptClkDef );
      break;
      
    default :
      // this is an error - report it
      //#error "Illegal System Clock Source!"
      break;
  }
  
  // setup the prescales/MCO clock source/
  uTemp = BITS( ptClkDef->eAhbPrescale, RCC_CFGR_HPRE_Pos );
  uTemp |= BITS( ptClkDef->eApb1Prescale, RCC_CFGR_PPRE1_Pos );
  uTemp |= BITS( ptClkDef->eApb2Prescale, RCC_CFGR_PPRE2_Pos );
  uTemp |= BITS( ptClkDef->eSysClkSrc, RCC_CFGR_SW_Pos );
  RCC->CFGR = uTemp;
  
  // now wait for clock source to be active
  uTemp = BITS( ptClkDef->eSysClkSrc, RCC_CFGR_SWS_Pos );
  while(( RCC->CFGR & RCC_CFGR_SWS ) != uTemp );
  
  // set up the AHB/APB1/APB2 clock enables
  ConfigureAhb( );
  ConfigureApb( );

  // set up the peripheral clock sources
  ConfigurePerClocks( );

  // compute clock frequency
  ComputeSystemClockFreq( );

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function Clock_Close
 *
 * @brief close the clock
 *
 * This function will close the clock
 *
 *****************************************************************************/
void Clock_Close( void )
{
}

/******************************************************************************
 * @function Clock_GetSysClkFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetSysClkFreq( void )
{
  // GetSystemClockSource
  return( uSysClkFreq );
}

/******************************************************************************
 * @function Clock_GetAPB1Frequency
 *
 * @brief get the current APB1 clock frequency
 *
 * This function will return the current APB1 clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetAPB1Frequency( void )
{
  U32 uApbFreq;

  // get the base frequency
  uApbFreq = uSysClkFreq;

  // compute the divisor
  uApbFreq >>= auApbxPrescale[ ( RCC->CFGR & RCC_CFGR_PPRE1 ) >> RCC_CFGR_PPRE1_Pos ];

  // return the freq
  return( uApbFreq );
}

/******************************************************************************
 * @function Clock_GetAPB2Frequency
 *
 * @brief get the current APB2 clock frequency
 *
 * This function will return the current APB2 clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetAPB2Frequency( void )
{
  U32 uApbFreq;

  // get the base frequency
  uApbFreq = uSysClkFreq;

  // compute the divisor
  uApbFreq >>= auApbxPrescale[ ( RCC->CFGR & RCC_CFGR_PPRE2 ) >> RCC_CFGR_PPRE2_Pos ];

  // return the freq
  return( uApbFreq );
}

/******************************************************************************
 * @function Clock_GetPerClkMuxFrequency
 *
 * @brief get the current peripheral clock frequency
 *
 * This function will return the current clock frequency for a given peripheral
 * mux
 *
 *  @param[in]  eClkMuxEnum   clock mux enumeration
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetPerClkMuxFrequency( CLOCKMUXSEL eClkMuxSel )
{
  U32 uClkMuxFreq = 0;

  switch( eClkMuxSel )
  {
    case CLOCK_MUXSEL_ADC12 :
    case CLOCK_MUXSEL_ADC345 :
      break;

    case CLOCK_MUXSEL_CRS :
      uClkMuxFreq = CLK_HSI48_FREQ;
      break;

    case CLOCK_MUXSEL_DAC :
      uClkMuxFreq = uSysClkFreq;
      break;

    case CLOCK_MUXSEL_FDCAN :
      switch(( RCC->CCIPR & RCC_CCIPR_FDCANSEL ) >> RCC_CCIPR_FDCANSEL_Pos )
      {
        case FDCAN_CLKSRC_HSE :
          uClkMuxFreq = uHseClkFreq;
          break;
          
        case FDCAN_CLKSRC_PLLQ :
          uClkMuxFreq = uPllQClkFreq;
          break;
          
        case FDCAN_CLKSRC_PCLK1 :
          uClkMuxFreq = uPclk1Freq;
          break;
          
        default :
          break;
      }
      break;

    case CLOCK_MUXSEL_I2C1 :
      uClkMuxFreq = GetI2cClockFreq(( RCC->CCIPR & RCC_CCIPR_I2C1SEL ) >> RCC_CCIPR_I2C1SEL_Pos );
      break;

    case CLOCK_MUXSEL_I2C2 :
      uClkMuxFreq = GetI2cClockFreq(( RCC->CCIPR & RCC_CCIPR_I2C2SEL ) >> RCC_CCIPR_I2C2SEL_Pos );
      break;

    case CLOCK_MUXSEL_I2C3 :
      uClkMuxFreq = GetI2cClockFreq(( RCC->CCIPR & RCC_CCIPR_I2C3SEL ) >> RCC_CCIPR_I2C3SEL_Pos );
      break;

    case CLOCK_MUXSEL_LPTIM1 :
      switch( ( RCC->CCIPR & RCC_CCIPR_LPTIM1SEL ) >> RCC_CCIPR_LPTIM1SEL_Pos )
      {
        case LPTIM_CLKSRC_PCLK1 :
          uClkMuxFreq = uPclk1Freq;
          break;
          
        case LPTIM_CLKSRC_LSI :
          uClkMuxFreq = CLK_LSI_FREQ;
          break;
          
        case LPTIM_CLKSRC_HSI16 :
          uClkMuxFreq = CLK_HSI16_FREQ;
          break;
          
        case LPTIM_CLKSRC_LSE :
          uClkMuxFreq = CLK_LSE_FREQ;
          break;
          
        default :
          break;
      }
      break;

    case CLOCK_MUXSEL_LPUART1 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_LPUART1, ( RCC->CCIPR & RCC_CCIPR_LPUART1SEL ) >> RCC_CCIPR_LPUART1SEL_Pos );
      break;

    case CLOCK_MUXSEL_PWR :
      uClkMuxFreq = uSysClkFreq;
      break;

    case CLOCK_MUXSEL_TIM1 :
    case CLOCK_MUXSEL_TIM8 :
    case CLOCK_MUXSEL_TIM20 :
      uClkMuxFreq = uPclk2Freq;
      break;

    case CLOCK_MUXSEL_TIM2 :
    case CLOCK_MUXSEL_TIM3 :
    case CLOCK_MUXSEL_TIM4 :
    case CLOCK_MUXSEL_TIM6 :
    case CLOCK_MUXSEL_TIM7 :
    case CLOCK_MUXSEL_TIM15 :
    case CLOCK_MUXSEL_TIM16 :
    case CLOCK_MUXSEL_TIM17 :
      uClkMuxFreq = uPclk1Freq;
      break;

    case CLOCK_MUXSEL_USART1 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_USART1, ( RCC->CCIPR & RCC_CCIPR_USART1SEL ) >> RCC_CCIPR_USART1SEL_Pos );
      break;

    case CLOCK_MUXSEL_USART2 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_USART2, ( RCC->CCIPR & RCC_CCIPR_USART2SEL ) >> RCC_CCIPR_USART2SEL_Pos );
      break;

    case CLOCK_MUXSEL_USART3 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_USART3, ( RCC->CCIPR & RCC_CCIPR_USART3SEL ) >> RCC_CCIPR_USART3SEL_Pos );
      break;

    case CLOCK_MUXSEL_UART4 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_UART4, ( RCC->CCIPR & RCC_CCIPR_UART4SEL ) >> RCC_CCIPR_UART4SEL_Pos );
      break;

    case CLOCK_MUXSEL_UART5 :
      uClkMuxFreq = GetUsartClockFreq( CLOCK_MUXSEL_UART5, ( RCC->CCIPR & RCC_CCIPR_UART5SEL ) >> RCC_CCIPR_UART5SEL_Pos );
      break;

    case CLOCK_MUXSEL_WDG :
      uClkMuxFreq = CLK_LSI_FREQ;
      break;

    case CLOCK_MUXSEL_MAX :
      break;

    default :
      break;
  }

  // return the frequency
  return( uClkMuxFreq );
}

/******************************************************************************
 * @function Clock_SetMasterClockOutput
 *
 * @brief set the master clock output
 *
 * This function will set the master clock output
 *
 * @param[in] eSrc      clock source
 * @param[in] ePrescale clock prescale
 *
 *****************************************************************************/
void Clock_SetMasterClockOutput( MCOCLKSRC eSrc, MCOPRESCALE ePrescale )
{
  U32 uTemp;

  // read the register
  uTemp = RCC->CFGR;

  // remove the current MCO settings
  uTemp &= 0x00FFFFFF;

  // now set the new settings
  uTemp |= BITS( eSrc, RCC_CFGR_MCOSEL_Pos );
  uTemp |= BITS( ePrescale, RCC_CFGR_MCOPRE_Pos );

  // now write it back out
  RCC->CFGR = uTemp;
}

/******************************************************************************
 * @function ConfigurePll
 *
 * @brief PLL configuration
 *
 * This function will configure the PLL
 *
 *  @param[in]      ptClkDef    pointer to the clock defitinion
 *
 *****************************************************************************/
static void ConfigurePll( PCLKSYSDEF ptClkDef )
{
  U32 uTemp, uRefClk;
  
  // set up the PLL
  uTemp = RCC_PLLCFGR_PLLREN;
  uTemp |= RCC_PLLCFGR_PLLPEN;
  uTemp |= RCC_PLLCFGR_PLLQEN;
  uTemp |= BITS( ptClkDef->ePllClkSrc, RCC_PLLCFGR_PLLSRC_Pos );
  uTemp |= BITS( ptClkDef->ePllDiv, RCC_PLLCFGR_PLLN_Pos );
  uTemp |= BITS( ptClkDef->nPllMult, RCC_PLLCFGR_PLLM_Pos );
  uTemp |= BITS( ptClkDef->ePllPDiv, RCC_PLLCFGR_PLLP_Pos );
  uTemp |= BITS( ptClkDef->ePllQDiv, RCC_PLLCFGR_PLLQ_Pos );
  uTemp |= BITS( ptClkDef->ePllRDiv, RCC_PLLCFGR_PLLR_Pos );

  // write the config
  RCC->PLLCFGR = uTemp;

  // turn it on/wait till it is ready
  RCC->CR |= RCC_CR_PLLON;
  while(( RCC->CR & RCC_CR_PLLRDY ) != RCC_CR_PLLRDY );

  // now set the clock frequency
  switch(( RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC ) >> RCC_PLLCFGR_PLLSRC_Pos )
  {
    case PLLCLK_SRC_HSI16 :
      uRefClk = CLK_HSI16_FREQ;
      break;

    case PLLCLK_SRC_HSE :
      uRefClk = CLK_HSE_FREQ;
      break;

    default :
      break;
  }

  // calcuate the clock frequencies
  uTemp = ( uRefClk / ptClkDef->ePllDiv ) * ptClkDef->nPllMult;
  uPllPClkFreq = uTemp / ptClkDef->ePllPDiv;
  uPllQClkFreq = uTemp / ptClkDef->ePllQDiv;
  uPllRClkFreq = uTemp / ptClkDef->ePllRDiv;
}

/******************************************************************************
 * @function ConfigureAhb
 *
 * @brief AHB configuration
 *
 * This function will configure the AHB
 *
 *****************************************************************************/
static void ConfigureAhb( void )
{
  PCLKAHB1ENBDEF  ptClkAhb1EnbDef;
  PCLKAHB2ENBDEF  ptClkAhb2EnbDef;
  PCLKAHB3ENBDEF  ptClkAhb3EnbDef;
  U32             uTemp;
  
  // set the pointers
  ptClkAhb1EnbDef = ( PCLKAHB1ENBDEF )&g_tClkAhb1EnbDef;
  ptClkAhb2EnbDef = ( PCLKAHB2ENBDEF )&g_tClkAhb2EnbDef;
  ptClkAhb3EnbDef = ( PCLKAHB3ENBDEF )&g_tClkAhb3EnbDef;
  
  // build the AHB1 value/write it
  uTemp =  ( ptClkAhb1EnbDef->bDMA1 )     ? RCC_AHB1ENR_DMA1EN    : 0;
  uTemp |= ( ptClkAhb1EnbDef->bDMA2 )     ? RCC_AHB1ENR_DMA2EN    : 0;
  uTemp |= ( ptClkAhb1EnbDef->bDMAMux )   ? RCC_AHB1ENR_DMAMUX1EN : 0;
  uTemp |= ( ptClkAhb1EnbDef->bCordic )   ? RCC_AHB1ENR_CORDICEN  : 0;
  uTemp |= ( ptClkAhb1EnbDef->bFMAC )     ? RCC_AHB1ENR_FMACEN    : 0;
  uTemp |= ( ptClkAhb1EnbDef->bFlash )    ? RCC_AHB1ENR_FLASHEN   : 0;
  uTemp |= ( ptClkAhb1EnbDef->bCRC )      ? RCC_AHB1ENR_CRCEN     : 0;
  RCC->AHB1ENR = uTemp;

  // build the AHB2 value/write it
  uTemp =  ( ptClkAhb2EnbDef->bGPIOA )    ? RCC_AHB2ENR_GPIOAEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOB )    ? RCC_AHB2ENR_GPIOBEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOC )    ? RCC_AHB2ENR_GPIOCEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOD )    ? RCC_AHB2ENR_GPIODEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOE )    ? RCC_AHB2ENR_GPIOEEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOF )    ? RCC_AHB2ENR_GPIOFEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOG )    ? RCC_AHB2ENR_GPIOGEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bADC12 )    ? RCC_AHB2ENR_ADC12EN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bADC345 )   ? RCC_AHB2ENR_ADC345EN  : 0;
  uTemp |= ( ptClkAhb2EnbDef->bDAC1 )     ? RCC_AHB2ENR_DAC1EN    : 0;
  uTemp |= ( ptClkAhb2EnbDef->bDAC3 )     ? RCC_AHB2ENR_DAC3EN    : 0;
  uTemp |= ( ptClkAhb2EnbDef->bRNG )      ? RCC_AHB2ENR_RNGEN     : 0;
  RCC->AHB2ENR = uTemp;

  // build the AHB3 value/write it
  uTemp =  ( ptClkAhb3EnbDef->bQSPI )     ? RCC_AHB3ENR_QSPIEN    : 0;
  RCC->AHB3ENR = uTemp;
}

/******************************************************************************
 * @function ConfigureApb1
 *
 * @brief Apb1 configuration
 *
 * This function will configure the APB1
 *
 *****************************************************************************/
static void ConfigureApb( void )
{
  PCLKAPB1ENBDEF  ptClkApb1EnbDef;
  PCLKAPB2ENBDEF  ptClkApb2EnbDef;
  U32             uTemp;

  // set the pointer
  ptClkApb1EnbDef = ( PCLKAPB1ENBDEF )&g_tClkApb1EnbDef;
  ptClkApb2EnbDef = ( PCLKAPB2ENBDEF )&g_tClkApb2EnbDef;
  
  // build the APB1 reg 1 value/write it
  uTemp =  ( ptClkApb1EnbDef->bTim2 )     ? RCC_APB1ENR1_TIM2EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bTim3 )     ? RCC_APB1ENR1_TIM3EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bTim4 )     ? RCC_APB1ENR1_TIM4EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bTim6 )     ? RCC_APB1ENR1_TIM6EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bTim7 )     ? RCC_APB1ENR1_TIM7EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bCRS )      ? RCC_APB1ENR1_CRSEN      : 0;
  uTemp |= ( ptClkApb1EnbDef->bRTC )      ? RCC_APB1ENR1_RTCAPBEN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bWWDG )     ? RCC_APB1ENR1_WWDGEN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bSpi2 )     ? RCC_APB1ENR1_SPI2EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bSpi3 )     ? RCC_APB1ENR1_SPI3EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bUsart2 )   ? RCC_APB1ENR1_USART2EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bUsart3 )   ? RCC_APB1ENR1_USART3EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bUart4 )    ? RCC_APB1ENR1_UART4EN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bUart5 )    ? RCC_APB1ENR1_UART5EN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bI2c1 )     ? RCC_APB1ENR1_I2C1EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bI2c2 )     ? RCC_APB1ENR1_I2C2EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bUSB )      ? RCC_APB1ENR1_USBEN      : 0;
  uTemp |= ( ptClkApb1EnbDef->bFDCan )    ? RCC_APB1ENR1_FDCANEN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bPwr )      ? RCC_APB1ENR1_PWREN      : 0;
  uTemp |= ( ptClkApb1EnbDef->bI2c3 )     ? RCC_APB1ENR1_I2C3EN     : 0;
  uTemp |= ( ptClkApb1EnbDef->bLpTim1 )   ? RCC_APB1ENR1_LPTIM1EN   : 0;
  RCC->APB1ENR1 = uTemp;

  uTemp = ( ptClkApb1EnbDef->bLpUart1 )   ? RCC_APB1ENR2_LPUART1EN  : 0;
  RCC->APB1ENR2 = uTemp;
  
  // build the APB 2 reg value/write it
  uTemp =  ( ptClkApb2EnbDef->bSysCfg )   ? RCC_APB2ENR_SYSCFGEN  : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim1 )     ? RCC_APB2ENR_TIM1EN    : 0;
  uTemp |= ( ptClkApb2EnbDef->bSpi1 )     ? RCC_APB2ENR_SPI1EN    : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim8 )     ? RCC_APB2ENR_TIM8EN    : 0;
  uTemp |= ( ptClkApb2EnbDef->bUsart1 )   ? RCC_APB2ENR_USART1EN  : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim15 )    ? RCC_APB2ENR_TIM15EN   : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim16 )    ? RCC_APB2ENR_TIM16EN   : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim17 )    ? RCC_APB2ENR_TIM17EN   : 0;
  uTemp |= ( ptClkApb2EnbDef->bSai1 )     ? RCC_APB2ENR_SAI1EN    : 0;
  RCC->APB2ENR = uTemp;
}

/******************************************************************************
 * @function ConfigurePerClocks
 *
 * @brief peripheral clock selection
 *
 * This function will configure the periapheral clock selection
 *
 *****************************************************************************/
static void ConfigurePerClocks( void )
{
  PCLKPERSRCDEF ptClkPerSrcDef;
  U32           uTemp;

  // set the pointer to the configuration
  ptClkPerSrcDef = ( PCLKPERSRCDEF )&g_tClkPerSrcDef;

  // build the value/write it
  uTemp =   ( ptClkPerSrcDef->nUsart1  << RCC_CCIPR_USART1SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nUsart2  << RCC_CCIPR_USART2SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nUsart3  << RCC_CCIPR_USART3SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nUart4   << RCC_CCIPR_UART4SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nUart5   << RCC_CCIPR_UART5SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nLpUart1 << RCC_CCIPR_LPUART1SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nI2c1    << RCC_CCIPR_I2C1SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nI2c2    << RCC_CCIPR_I2C2SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nI2c3    << RCC_CCIPR_I2C3SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nLpTim1  << RCC_CCIPR_LPTIM1SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nSAI1    << RCC_CCIPR_SAI1SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nI2S23   << RCC_CCIPR_I2S23SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nFDCan   << RCC_CCIPR_FDCANSEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nClk48   << RCC_CCIPR_CLK48SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nAdc12   << RCC_CCIPR_ADC12SEL_Pos );
  uTemp |=  ( ptClkPerSrcDef->nAdc345  << RCC_CCIPR_ADC345SEL_Pos );
  RCC->CCIPR = uTemp;
}

/******************************************************************************
 * @function ComputeSystemClockFreq
 *
 * @brief compute the system clock frequency
 *
 * This function will compute the system clock frequency
 *
 *****************************************************************************/
static void ComputeSystemClockFreq( void )
{
  // now determine the clock source
  switch(( RCC->CFGR & RCC_CFGR_SWS ) >> RCC_CFGR_SWS_Pos )
  {
    case SYSCLK_SRC_HSE :
      // set the freq to the HSE value
      uSysClkFreq = CLK_HSE_FREQ;
      break;

    case SYSCLK_SRC_PLLR :
      uSysClkFreq = uPllRClkFreq;
      break;

    case SYSCLK_SRC_HSI16 :
    default :
      // set the freq to the HSI value
      uSysClkFreq = CLK_HSI16_FREQ;
      break;
  }

  // apply the prescaler
  uSysClkFreq >>= auAhbPrescale[ ( RCC->CFGR & RCC_CFGR_HPRE ) >> RCC_CFGR_HPRE_Pos ];
  
  // calculate the PCLK1/PCLK2
  uPclk1Freq = uSysClkFreq >>= auApbxPrescale[ ( RCC->CFGR &  RCC_CFGR_PPRE1 ) >> RCC_CFGR_PPRE1_Pos ];
  uPclk2Freq = uSysClkFreq >>= auApbxPrescale[ ( RCC->CFGR &  RCC_CFGR_PPRE2 ) >> RCC_CFGR_PPRE2_Pos ];
}

/******************************************************************************
 * @function GetI2cClockFreq
 *
 * @brief get the clock freqency for I2C
 *
 * This function will return the current clock frequency for a I2C selection
 *
 *  @param[in]  eClkSel   clock mux enumeration
 *
 *  @return     the frequency of the clock
 *
 *****************************************************************************/
static U32 GetI2cClockFreq( I2CCLKSRC eClkSel )
{
  U32 uClkFreq = 0;
  
  // determine the frequency
  switch( eClkSel )
  {
    case I2C_CLKSRC_PCLK1 :
      uClkFreq = uPclk1Freq;
      break;
      
    case I2C_CLKSRC_SYSCLK :
      uClkFreq = uSysClkFreq;
      break;
      
    case I2C_CLKSRC_HSI16 :
      uClkFreq = CLK_HSI16_FREQ;
      break;

    default :
      break;
  }
  
  // return the frequency
  return( uClkFreq );
}

/******************************************************************************
 * @function GetUSARTClockFreq
 *
 * @brief get the clock freqency for I2C
 *
 * This function will return the current clock frequency for a I2C selection
 *
 *  @param[in]  eClkSel   clock mux enumeration
 *
 *  @return     the frequency of the clock
 *
 *****************************************************************************/
static U32 GetUsartClockFreq( CLOCKMUXSEL eClockMuxSel, UARTCLKSRC eClkSel )
{
  U32 uClkFreq = 0;
  
  // determine the frequency
  switch( eClkSel )
  {
    case UART_CLKSRC_PCLKX :
      uClkFreq = ( eClockMuxSel == CLOCK_MUXSEL_USART1 ) ? uPclk2Freq : uPclk1Freq;
      break;
      
    case UART_CLKSRC_SYSCLK :
      uClkFreq = uSysClkFreq;
      break;
      
    case UART_CLKSRC_HSI16 :
      uClkFreq = CLK_HSI16_FREQ;
      break;
      
    case UART_CLKSRC_LSE :
      uClkFreq = CLK_LSE_FREQ;
      break;
      
    default :
      break;
  }
  
  // return the frequency
  return( uClkFreq );
}

/******************************************************************************
 * @function GetAdcClockFreq
 *
 * @brief get the clock freqency for ADC
 *
 * This function will return the current clock frequency for a I2C selection
 *
 *  @param[in]  eClkSel   clock mux enumeration
 *
 *  @return     the frequency of the clock
 *
 *****************************************************************************/
static U32 GetAdcClockFreq( ADCCLKSRC eClkSel )
{
  U32 uClkFreq = 0;
  
  // determine the frequency
  switch( eClkSel )
  {
    ADC_CLKSRC_SYSCLK :
      uClkFreq = uSysClkFreq;
      break;
      
    ADC_CLKSRC_PLLP :
      uClkFreq = uPllPClkFreq;
      break;

    default :
      break;
  }
  
  // return the frequency
  return( uClkFreq );
}

/**@} EOF Clock.c */
