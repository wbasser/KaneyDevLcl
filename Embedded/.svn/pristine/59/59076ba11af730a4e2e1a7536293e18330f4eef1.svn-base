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
#define CLK_HSI_FREQ                            ( 16000000ul )
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
static  U32   uSystemClockFreq;
static  U32   uPllPClkFreq;
static  U32   uPllQClkFreq;
static  U32   uPllRClkFreq;
static  U32   uPllSaiPClkFreq;
static  U32   uPllSaiQClkFreq;
static  U32   uPllSaiRClkFreq;
static  U32   uMsiClkFreq;

// local function prototypes --------------------------------------------------
static  void  ConfigureMsi( PCLKSYSDEF ptClkDef );
static  void  ConfigurePll( PCLKSYSDEF ptClkDef );
static  void  ConfigureAhb( void );
static  void  ConfigureApb1( void );
static  void  ConfigureApb2( void );
static  void  ConfigurePerClocks( void );
static  void  ComputeSystemClockFreq( void );
static  U32   GetUartUsartI2cClkFreq( APBSELECT eApbSel, SERCLKSRC eClkSRc );
static  U32   GetLpTimerClkFreq( LPTIMCLKSRC eClkSRc );


// constant parameter initializations -----------------------------------------
// define the MSI clock rqnge
static  const U32 auMsiClockRange[ MSICLK_RANGE_MAX ] =
{
    100000,
    200000,
    400000,
    800000,
   1000000,
   2000000,
   4000000,
   8000000,
  16000000,
  24000000,
  32000000,
  48000000,
};

// define the AHB prescale table
static const U32 auAhbPrescale[ ] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 
  1, 2, 3, 4, 6, 7, 8, 9  
};

/// define the APBX prescale table
static const U32 auApbxPrescale[ ] =
{
  0, 0, 0, 0, 1, 2, 3, 4
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
  uPllSaiPClkFreq = uPllSaiQClkFreq = uPllSaiRClkFreq = 0;
  uMsiClkFreq = 0;

  // set the pointer
  ptClkDef = ( PCLKSYSDEF )&g_tClkSysDef;
  
  // determine the clock source
  switch( ptClkDef->eSysClkSrc )
  {
    case SYSCLK_SRC_MSI :
      // configure the MSI clock
      ConfigureMsi( ptClkDef );
      break;
      
    case SYSCLK_SRC_HSI16 :
      // turn on the clock/wait ready
      RCC->CR  |= RCC_CR_HSION;
      while(( RCC->CR & RCC_CR_HSIRDY ) != RCC_CR_HSIRDY);
      break;
      
    case SYSCLK_SRC_HSE :
      break;
      
    case SYSCLK_SRC_PLL :
      // determine the source of the 
      switch( ptClkDef->ePllClkSrc )
      {
        case PLLCLK_SRC_MSI :
          // configure the MSI clock
          ConfigureMsi( ptClkDef );
          break;
          
        case PLLCLK_SRC_HSI16 :
          // turn on the clock/wait ready
          RCC->CR  |= RCC_CR_HSION;
          while(( RCC->CR & RCC_CR_HSIRDY ) != RCC_CR_HSIRDY);
          break;
          
        case PLLCLK_SRC_HSE :
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
  while(( RCC->CFGR & RCC_CFGR_SWS ) != BITS( ptClkDef->eSysClkSrc, RCC_CFGR_SWS_Pos ));
  
  // set up the AHB/APB1/APB2 clock enables
  ConfigureAhb( );
  ConfigureApb1( );
  ConfigureApb2( );

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
  return( uSystemClockFreq );
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
  uApbFreq = uSystemClockFreq;

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
  uApbFreq = uSystemClockFreq;

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
U32 Clock_GetPerClkMuxFrequency( CLKMUXENUM eClkMuxEnum )
{
  U32             uClkMuxFreq = 0;

  switch( eClkMuxEnum )
  {
    case CLKMUX_ENUM_USART1 :
      uClkMuxFreq = GetUartUsartI2cClkFreq( APBSELECT_APB2, (( RCC->CCIPR & RCC_CCIPR_USART1SEL ) >> RCC_CCIPR_USART1SEL_Pos ));
      break;

    case CLKMUX_ENUM_LPUART :
      uClkMuxFreq = GetUartUsartI2cClkFreq( APBSELECT_APB1, (( RCC->CCIPR & RCC_CCIPR_LPUART1SEL ) >> RCC_CCIPR_LPUART1SEL_Pos ));
      break;

    case CLKMUX_ENUM_LPTIM1 :
      uClkMuxFreq = GetLpTimerClkFreq((( RCC->CCIPR & RCC_CCIPR_LPTIM1SEL ) >> RCC_CCIPR_LPTIM1SEL_Pos ));
      break;

    case CLKMUX_ENUM_LPTIM2 :
      uClkMuxFreq = GetLpTimerClkFreq((( RCC->CCIPR & RCC_CCIPR_LPTIM2SEL ) >> RCC_CCIPR_LPTIM2SEL_Pos ));
      break;

    case CLKMUX_ENUM_I2C1 :
      uClkMuxFreq = GetUartUsartI2cClkFreq( APBSELECT_APB1, (( RCC->CCIPR & RCC_CCIPR_I2C1SEL ) >> RCC_CCIPR_I2C1SEL_Pos ));
      break;

    case CLKMUX_ENUM_I2C3 :
      uClkMuxFreq = GetUartUsartI2cClkFreq(APBSELECT_APB1, (( RCC->CCIPR & RCC_CCIPR_I2C3SEL ) >> RCC_CCIPR_I2C3SEL_Pos ));
      break;

    case CLKMUX_ENUM_SAI1 :
      // TODO - update this if the clock value is needed
      uClkMuxFreq = 0;
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
 * @function ConfigureMsi
 *
 * @brief MSI configuration
 *
 * This function will configure the MSI clock
 *
 *  @param[in]      ptClkDef    pointer to the clock defitinion
 *
 *****************************************************************************/
static void ConfigureMsi( PCLKSYSDEF ptClkDef )
{
  // turn on MSI/set the range
  RCC->CR = RCC_CR_MSION | BITS( ptClkDef->eMsiClkRange, RCC_CR_MSIRANGE_Pos );
  uMsiClkFreq = auMsiClockRange[ ptClkDef->eMsiClkRange ];
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

  // set up the SAI PLL
  uTemp |= RCC_PLLSAI1CFGR_PLLREN;
  uTemp |= RCC_PLLSAI1CFGR_PLLPEN;
  uTemp |= RCC_PLLSAI1CFGR_PLLQEN;
  uTemp |= BITS( ptClkDef->nPllSaiMult, RCC_PLLSAI1CFGR_PLLN_Pos );
  uTemp |= BITS( ptClkDef->ePllSaiPDiv, RCC_PLLSAI1CFGR_PLLP_Pos );
  uTemp |= BITS( ptClkDef->ePllSaiQDiv, RCC_PLLSAI1CFGR_PLLQ_Pos );
  uTemp |= BITS( ptClkDef->ePllSaiRDiv, RCC_PLLSAI1CFGR_PLLR_Pos );

  // write the config
  RCC->PLLSAI1CFGR = uTemp;

  // turn it on/wait till it is ready
  RCC->CR |= RCC_CR_PLLSAI1ON;
  while(( RCC->CR & RCC_CR_PLLSAI1RDY ) != RCC_CR_PLLSAI1RDY );

  // now set the clock frequency
  switch(( RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC ) >> RCC_PLLCFGR_PLLSRC_Pos )
  {
    case PLLCLK_SRC_MSI :
      uRefClk = uMsiClkFreq;
      break;

    case PLLCLK_SRC_HSI16 :
      uRefClk = CLK_HSI_FREQ;
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
  uTemp = ( uRefClk / ptClkDef->ePllDiv ) * ptClkDef->nPllSaiMult;
  uPllSaiPClkFreq = uTemp / ptClkDef->ePllSaiPDiv;
  uPllSaiQClkFreq = uTemp / ptClkDef->ePllSaiQDiv;
  uPllSaiRClkFreq = uTemp / ptClkDef->ePllSaiRDiv;
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
  uTemp =  ( ptClkAhb1EnbDef->bDMA1 )    ? RCC_AHB1ENR_DMA1EN   : 0;
  uTemp |= ( ptClkAhb1EnbDef->bDMA2 )    ? RCC_AHB1ENR_DMA2EN   : 0;
  uTemp |= ( ptClkAhb1EnbDef->bDMAMUX1 ) ? RCC_AHB1ENR_DMAMUX1EN : 0;
  uTemp |= ( ptClkAhb1EnbDef->bCrc )     ? RCC_AHB1ENR_CRCEN    : 0;
  uTemp |= ( ptClkAhb1EnbDef->bTouch )   ? RCC_AHB1ENR_TSCEN    : 0;
  RCC->AHB1ENR = uTemp;

  // build the AHB2 value/write it
  uTemp =  ( ptClkAhb2EnbDef->bGPIOA ) ? RCC_AHB2ENR_GPIOAEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOB ) ? RCC_AHB2ENR_GPIOBEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOC ) ? RCC_AHB2ENR_GPIOCEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOD ) ? RCC_AHB2ENR_GPIODEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOE ) ? RCC_AHB2ENR_GPIOEEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bGPIOH ) ? RCC_AHB2ENR_GPIOHEN : 0;
  uTemp |= ( ptClkAhb2EnbDef->bADC )   ? RCC_AHB2ENR_ADCEN   : 0;
  uTemp |= ( ptClkAhb2EnbDef->bAES1 )  ? RCC_AHB2ENR_AES1EN  : 0;
  RCC->AHB2ENR = uTemp;

  // build the AHB3 value/write it
  uTemp =  ( ptClkAhb3EnbDef->bQSPI  ) ? RCC_AHB3ENR_QUADSPIEN : 0;
  uTemp |= ( ptClkAhb3EnbDef->bPKA   ) ? RCC_AHB3ENR_PKAEN     : 0;
  uTemp |= ( ptClkAhb3EnbDef->bAES2  ) ? RCC_AHB3ENR_AES2EN    : 0;
  uTemp |= ( ptClkAhb3EnbDef->bRNG   ) ? RCC_AHB3ENR_RNGEN     : 0;
  uTemp |= ( ptClkAhb3EnbDef->bHSEM  ) ? RCC_AHB3ENR_HSEMEN    : 0;
  uTemp |= ( ptClkAhb3EnbDef->bIPCC  ) ? RCC_AHB3ENR_IPCCEN    : 0;
  uTemp |= ( ptClkAhb3EnbDef->bFlash ) ? RCC_AHB3ENR_FLASHEN   : 0;
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
static void ConfigureApb1( void )
{
  PCLKAPB1ENBDEF  ptClkApb1EnbDef;
  U32             uTemp;

  // set the pointer
  ptClkApb1EnbDef = ( PCLKAPB1ENBDEF )&g_tClkApb1EnbDef;
  
  // build the APB1 reg 1 value/write it
  uTemp =  ( ptClkApb1EnbDef->bTim2 )   ? RCC_APB1ENR1_TIM2EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bLcd )    ? RCC_APB1ENR1_LCDEN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bRTC )    ? RCC_APB1ENR1_RTCAPBEN : 0;
  uTemp |= ( ptClkApb1EnbDef->bWWDG )   ? RCC_APB1ENR1_WWDGEN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bSpi2 )   ? RCC_APB1ENR1_SPI2EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bI2c1 )   ? RCC_APB1ENR1_I2C1EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bI2c3 )   ? RCC_APB1ENR1_I2C3EN   : 0;
  uTemp |= ( ptClkApb1EnbDef->bCRS )    ? RCC_APB1ENR1_CRSEN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bUSB )    ? RCC_APB1ENR1_USBEN    : 0;
  uTemp |= ( ptClkApb1EnbDef->bLpTim1 ) ? RCC_APB1ENR1_LPTIM1EN : 0;
  RCC->APB1ENR1 = uTemp;
  
  // build the APB1 reg 2 value/write it
  uTemp =  ( ptClkApb1EnbDef->bLpUart1 ) ? RCC_APB1ENR2_LPUART1EN : 0;
  uTemp |= ( ptClkApb1EnbDef->bLpTim2 )  ? RCC_APB1ENR2_LPTIM2EN  : 0;
  RCC->APB1ENR2 = uTemp;
}

/******************************************************************************
 * @function ConfigureApb2
 *
 * @brief Apb2 configuration
 *
 * This function will configure the APB2
 *
 *****************************************************************************/
static void ConfigureApb2( void )
{
  PCLKAPB2ENBDEF  ptClkApb2EnbDef;
  U32             uTemp;
  
  // set the pointer
  ptClkApb2EnbDef = ( PCLKAPB2ENBDEF )&g_tClkApb2EnbDef;
  
  // build the APB1 reg 1 value/write it
  uTemp =  ( ptClkApb2EnbDef->bTim1 )   ? RCC_APB2ENR_TIM1EN   : 0;
  uTemp |= ( ptClkApb2EnbDef->bSpi1 )   ? RCC_APB2ENR_SPI1EN   : 0;
  uTemp |= ( ptClkApb2EnbDef->bUsart1 ) ? RCC_APB2ENR_USART1EN : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim16 )  ? RCC_APB2ENR_TIM16EN  : 0;
  uTemp |= ( ptClkApb2EnbDef->bTim17 )  ? RCC_APB2ENR_TIM17EN  : 0;
  uTemp |= ( ptClkApb2EnbDef->bSai1 )   ? RCC_APB2ENR_SAI1EN   : 0;
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
  PCLKPERSRCDEF   ptClkPerSrcDef;
  U32             uTemp;

  // set the pointer to the configuration
  ptClkPerSrcDef = ( PCLKPERSRCDEF )&g_tClkPerSrcDef;

  // build the value/write it
  uTemp = ( ptClkPerSrcDef->nUsart1ClkSrc << RCC_CCIPR_USART1SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nLpUart1ClkSrc << RCC_CCIPR_LPUART1SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nI2c1ClkSrc << RCC_CCIPR_I2C1SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nI2c3ClkSrc << RCC_CCIPR_I2C3SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nLpTim1ClkSrc << RCC_CCIPR_LPTIM1SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nLpTim2ClkSrc << RCC_CCIPR_LPTIM2SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nSaiClkSrc << RCC_CCIPR_SAI1SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nUsbClkSrc << RCC_CCIPR_CLK48SEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nAdcClkSrc << RCC_CCIPR_ADCSEL_Pos );
  uTemp |= ( ptClkPerSrcDef->nRngClkSrc << RCC_CCIPR_RNGSEL_Pos );
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
    case SYSCLK_SRC_HSI16 :
      // set the freq to the HSI value
      uSystemClockFreq = CLK_HSI_FREQ;
      break;

    case SYSCLK_SRC_HSE :
      // set the freq to the HSE value
      uSystemClockFreq = CLK_HSE_FREQ;
      break;

    case SYSCLK_SRC_PLL :
      uSystemClockFreq = uPllRClkFreq;
      break;

    case SYSCLK_SRC_MSI :
    default :
      // set to MSI
      uSystemClockFreq = uMsiClkFreq;
      break;
  }

  // apply the prescaler
  uSystemClockFreq >>= auAhbPrescale[ ( RCC->CFGR & RCC_CFGR_HPRE ) >> RCC_CFGR_HPRE_Pos ];
}

/******************************************************************************
 * @function GetUartUsartI2cClkFreq
 *
 * @brief get the selecter reference clock frequency
 *
 * This function will return the selected clock frequency for the SERIAL
 *
 * @param[in]   eApbSel     APB select
 * @param[in]   eClkSrc     clock source
 *
 * @return  selected clock source frequency
 *
 *****************************************************************************/
static U32 GetUartUsartI2cClkFreq( APBSELECT eApbSel, SERCLKSRC eClkSRc )
{
  U32 uClkFreq;

  switch( eClkSRc )
  {
    case SER_CLKSRC_PCLK :
      if ( eApbSel == APBSELECT_APB1 )
        uClkFreq = Clock_GetAPB1Frequency( );
      else
        uClkFreq = Clock_GetAPB2Frequency( );
      break;

    case SER_CLKSRC_SYSCLK :
      uClkFreq = uSystemClockFreq;
      break;

    case SER_CLKSRC_HSI16 :
      uClkFreq = CLK_HSI_FREQ;
      break;

    case SER_CLKSRC_LSE :
      uClkFreq = CLK_LSE_FREQ;
      break;

    default:
      break;
  }

  // return it
  return( uClkFreq );
}

/******************************************************************************
 * @function GetLpTimerClkFreq
 *
 * @brief get the selecter reference clock frequency
 *
 * This function will return the selected clock frequency for the LPTIMX
 *
 * @param[in]   eClkSrc     clock source
 *
 * @return  selected clock source frequency
 *
 *****************************************************************************/
static U32 GetLpTimerClkFreq( LPTIMCLKSRC eClkSRc )
{
  U32 uClkFreq;

  switch( eClkSRc )
  {
    case LPTIM_CLKSRC_PCLK :
      uClkFreq = Clock_GetAPB1Frequency( );
      break;

    case LPTIM_CLKSRC_HSI16 :
      uClkFreq = CLK_HSI_FREQ;
      break;

    case LPTIM_CLKSRC_LSI :
      uClkFreq = CLK_LSI_FREQ;
      break;

    case LPTIM_CLKSRC_LSE :
      uClkFreq = CLK_LSE_FREQ;
      break;

    default:
      break;
  }

  // return it
  return( uClkFreq );
}

/**@} EOF Clock.c */
