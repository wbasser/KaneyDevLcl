/******************************************************************************
 * @file Clock.c
 *
 * @brief clock module implementation
 *
 * This file clock module initialization and up/down scaling
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
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

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   uSystemClockFreq;
static  U32   auXoscFreqs[ OSCCTRL_XOSCS_NUM ];
static  U32   auDpllFreqs[ OSCCTRL_DPLLS_NUM ];
static  U32   uDpll1Freq;

// local function prototypes --------------------------------------------------
static  U32   GetGenClkFreq( CLOCKGENID eId );
static  BOOL  ConfigureExternalOsc( CLOCKOSC eOscSel, PCLOCKXOSCKDEF ptOscDef );
static  BOOL  ConfigureExt32KOsc( PCLOCKOSC32KDEF ptOsc32Def );

/******************************************************************************
 * @function Clock_Initialization
 *
 * @brief clock initialization
 *
 * This function will initialize the clock control module
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL  Clock_Initialize( void )
{
  BOOL              bStatus = FALSE;
  PCLOCKMUXDEF      ptMuxDef;
  PCLOCKGENDEF      ptGenDef;
  PCLOCKOSCDEF      ptOscDef;
  U8                nIdx;
  BOOL              bRunFlag;
  GCLK_GENCTRL_Type tGenCtl;
  GCLK_PCHCTRL_Type tMuxCtl;

  // set the auto wait state mode
  NVMCTRL->CTRLA.bit.AUTOWS = ON;
  
  // set the main clocks dividers
  MCLK->CPUDIV.reg = g_tClockMainDef.nCpuDivide;

  // for now just se the system frequency
  uSystemClockFreq = 48000000;
  auXoscFreqs[ 0 ] = 0;
  auXoscFreqs[ 1 ] = 0;
  auDpllFreqs[ 0 ] = 0;
  auDpllFreqs[ 1 ] = 0;
  
  // now set up the oscillators
  bRunFlag = TRUE;
  nIdx = 0;
  while( bRunFlag == TRUE )
  {
    // get a pointer to an entry in the array
    ptOscDef = ( PCLOCKOSCDEF )&g_atClockOscDefs[ nIdx++ ];

    // determine the oscillator
    switch( ptOscDef->eOscSel )
    {
      case CLOCK_OSC_XOSC0 :      
      case CLOCK_OSC_XOSC1 :
        bStatus = ConfigureExternalOsc( ptOscDef->eOscSel, &ptOscDef->tOscs.tXOscDef );
        break;
        
      case CLOCK_OSC_OSC32K :
        break;
        
      case CLOCK_OSC_XOSC32K :
        bStatus = ConfigureExt32KOsc( &ptOscDef->tOscs.tOsc32Def );
        break;
        
      case CLOCK_OSC_DFLL48M :
        break;
        
      case CLOCK_OSC_DPLL0 :
      case CLOCK_OSC_DPLL1 :
        break;

      default :
        // clear the runflag
        bRunFlag = FALSE;
        break;
    }
  }

  // if there are errors do not continue
  if ( FALSE == bStatus )
  {
    // now set the gens
    bRunFlag = TRUE;
    nIdx = 0;
    while( bRunFlag == TRUE )
    {
      // get a pointer to a generator entry
      ptGenDef = ( PCLOCKGENDEF )&g_atClockGenDefs[ nIdx++ ];
    
      // check for valid
      if ( ptGenDef->eId != CLOCK_GENID_MAX )
      {
        // build the register values
        tGenCtl.bit.SRC = ptGenDef->eClockSrc;
        tGenCtl.bit.GENEN = ON;
        tGenCtl.bit.IDC = ptGenDef->tFlags.bImprovedDutyCycle;
        tGenCtl.bit.OOV = ptGenDef->tFlags.bOutOffEnabled;
        tGenCtl.bit.OE = ptGenDef->tFlags.bOutputEnabled;
        tGenCtl.bit.DIVSEL = ptGenDef->eDivideSelect;
        tGenCtl.bit.RUNSTDBY = ptGenDef->tFlags.bRunInStandby;
        tGenCtl.bit.DIV = ptGenDef->wDivisor;
      
        // now set it
        GCLK->GENCTRL[ ptGenDef->eId ] = tGenCtl;
      
        // wait till synced
        while( GCLK->SYNCBUSY.reg & ( BIT( ptGenDef->eId + 2 )));
      }
      else
      {
        // clear the run flag
        bRunFlag = FALSE;
      }
    }
  
    // now set the muxes
    bRunFlag = TRUE;
    nIdx = 0;
    while( bRunFlag == TRUE )
    {
      // get a pointer to a generator entry
      ptMuxDef = ( PCLOCKMUXDEF )&g_atClockMuxDefs[ nIdx++ ];
    
      // check for valid
      if ( ptMuxDef->eMuxId != CLOCK_MUXID_MAX )
      {
        // build the register values
        tMuxCtl.bit.GEN = ptMuxDef->eClockGenId;
        tMuxCtl.bit.CHEN = ON;
        tMuxCtl.bit.WRTLOCK = ptMuxDef->bWriteLock;
      
        // now set it
        GCLK->PCHCTRL[ ptMuxDef->eMuxId ] = tMuxCtl;
      }
      else
      {
        // clear the run flag
        bRunFlag = FALSE;
      }
    }
  }

  // return status
  return( bStatus );
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
 * @function Clock_GetMainFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetMainClkFreq( )
{
  U32  uMainClkFrequency;

  // get the clock source for the GENCTL 0
  switch( GCLK->GENCTRL[ 0 ].bit.SRC )
  {
    case CLOCK_SRC_XOSC0 :
      uSystemClockFreq = auXoscFreqs[ CLOCK_SRC_XOSC0 ];
      break;

    case CLOCK_SRC_XOSC1 :
      uSystemClockFreq = auXoscFreqs[ CLOCK_SRC_XOSC1 ];
      break;

    case CLOCK_SRC_GCLKIN :
      break;

    case CLOCK_SRC_GCLKGEN1 :
      break;

    case CLOCK_SRC_OSCULP32K :
      uSystemClockFreq = 32768;
      break;

    case CLOCK_SRC_XOSC32K :
      uSystemClockFreq = 32768;
      break;

    case CLOCK_SRC_DFLL48M :
      uSystemClockFreq = 48000000;
      break;

    case CLOCK_SRC_DPLL0 :
      break;

    case CLOCK_SRC_DPLL1 :
      break;

    default :
      break;
  }
  
  // calculate the main clock frequency
  uMainClkFrequency = uSystemClockFreq / MCLK->CPUDIV.reg;

  // return it
  return( uMainClkFrequency );
}

/******************************************************************************
 * @function Clock_GetPeripheralFreq
 *
 * @brief get the current clock of a given peripheral frequency
 *
 * This function will return the current clock frequency
 *
 * @param[in]   eMuxId      multiplexer ID
 * 
 *  @return     the frequency of the clock
 *
 *****************************************************************************/
U32 Clock_GetPeripheralFreq( CLOCKMUXID eMuxId )
{
  // get the generator clock frequency
  return( GetGenClkFreq( GCLK->PCHCTRL[ eMuxId ].bit.GEN ));
}

/******************************************************************************
 * @function Clock_PeripheralControl
 *
 * @brief peripheral clock control
 *
 * This function will enable/disable a peripheral clock
 *
 * @param[in]   ePeriphId     peripheral ID
 * @param[in]   bState        state of the peripheral
 * 
 *****************************************************************************/
void Clock_PeripheralControl( CLOCKPERIPH ePeriphId, BOOL bState )
{
  switch( ePeriphId )
  {
    case CLOCK_PERIPH_AC :
      MCLK->APBCMASK.bit.AC_ = bState;
      break;
      
    case CLOCK_PERIPH_ADC0 :
      MCLK->APBDMASK.bit.ADC0_ = bState;
      break;
      
    case CLOCK_PERIPH_ADC1 :
      MCLK->APBDMASK.bit.ADC1_ = bState;
      break;
      
    case CLOCK_PERIPH_AES :
      MCLK->APBCMASK.bit.AES_ = bState;
      break;
      
    case CLOCK_PERIPH_BRIDGEA :
      MCLK->AHBMASK.bit.HPB0_ = bState;
      break;
      
    case CLOCK_PERIPH_BRIDGEB :
      MCLK->AHBMASK.bit.HPB1_ = bState;
      break;
      
    case CLOCK_PERIPH_BRIDGEC :
      MCLK->AHBMASK.bit.HPB2_ = bState;
      break;
      
    case CLOCK_PERIPH_BRIDGED :
      MCLK->AHBMASK.bit.HPB3_ = bState;
      break;
      
    case CLOCK_PERIPH_CCL :
      MCLK->APBCMASK.bit.CCL_ = bState;
      break;
      
    case CLOCK_PERIPH_CMCC :
      MCLK->AHBMASK.bit.CMCC_ = bState;
      break;
      
    case CLOCK_PERIPH_DAC :
      MCLK->APBDMASK.bit.DAC_ = bState;
      break;
      
    case CLOCK_PERIPH_DMAC :
      MCLK->AHBMASK.bit.DMAC_ = bState;
      break;
      
    case CLOCK_PERIPH_DSU :
      MCLK->AHBMASK.bit.DSU_ = bState;
      MCLK->APBBMASK.bit.DSU_ = bState;
      break;
      
    case CLOCK_PERIPH_EIC :
      MCLK->APBAMASK.bit.EIC_ = bState;
      break;
      
    case CLOCK_PERIPH_EVSYS :
      MCLK->APBBMASK.bit.EVSYS_ = bState;
      break;
      
    case CLOCK_PERIPH_FREQM :
      MCLK->APBAMASK.bit.FREQM_ = bState;
      break;
      
    case CLOCK_PERIPH_GCLK :
      MCLK->APBAMASK.bit.GCLK_ = bState;
      break;
      
    case CLOCK_PERIPH_ICM :
      MCLK->AHBMASK.bit.ICM_ = bState;
      MCLK->APBCMASK.bit.ICM_ = bState;
      break;
      
    case CLOCK_PERIPH_I2S :
      MCLK->APBDMASK.bit.I2S_ = bState;
      break;
      
    case CLOCK_PERIPH_MCLK :
      MCLK->APBAMASK.bit.MCLK_ = bState;
      break;
      
    case CLOCK_PERIPH_NVMCTRL :
      MCLK->AHBMASK.bit.NVMCTRL_ = bState;
      MCLK->APBBMASK.bit.NVMCTRL_ = bState;
      break;
      
    case CLOCK_PERIPH_OSCCTRL :
      MCLK->APBAMASK.bit.OSCCTRL_ = bState;
      break;
      
    case CLOCK_PERIPH_OSC32KCTRL :
      MCLK->APBAMASK.bit.OSC32KCTRL_ = bState;
      break;
      
    case CLOCK_PERIPH_PAC :
      MCLK->AHBMASK.bit.PAC_ = bState;
      MCLK->APBAMASK.bit.PAC_ = bState;
      break;
      
     case CLOCK_PERIPH_PCC :
      MCLK->APBDMASK.bit.PCC_ = bState;
      break;
      
    case CLOCK_PERIPH_PDEC :
      MCLK->APBCMASK.bit.PDEC_ = bState;
      break;
    
    case CLOCK_PERIPH_PM :
      MCLK->APBAMASK.bit.PM_ = bState;
      break;
      
    case CLOCK_PERIPH_PORT :
      MCLK->APBBMASK.bit.PORT_ = bState;
      break;
      
    case CLOCK_PERIPH_PUKCC :
      MCLK->AHBMASK.bit.PUKCC_ = bState;
      break;
      
    case CLOCK_PERIPH_QSPI :
      MCLK->AHBMASK.bit.QSPI_ = bState;
      MCLK->APBCMASK.bit.QSPI_ = bState;
      break;
      
    case CLOCK_PERIPH_QSPI2X :
      break;
      
    case CLOCK_PERIPH_RAMECC :
      MCLK->APBBMASK.bit.RAMECC_ = bState;
      break;
      
    case CLOCK_PERIPH_RSTC :
      MCLK->APBAMASK.bit.RSTC_ = bState;
      break;
      
    case CLOCK_PERIPH_RTC :
      MCLK->APBAMASK.bit.RTC_ = bState;
      break;
      
    case CLOCK_PERIPH_SDHC0 :
      MCLK->AHBMASK.bit.SDHC0_ = bState;
      break;
      
    case CLOCK_PERIPH_SDHC1 :
      MCLK->AHBMASK.bit.SDHC1_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM0 :
      MCLK->APBAMASK.bit.SERCOM0_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM1 :
      MCLK->APBAMASK.bit.SERCOM1_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM2 :
      MCLK->APBBMASK.bit.SERCOM2_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM3 :
      MCLK->APBBMASK.bit.SERCOM3_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM4 :
      MCLK->APBDMASK.bit.SERCOM4_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM5 :
      MCLK->APBDMASK.bit.SERCOM5_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM6 :
      MCLK->APBDMASK.bit.SERCOM6_ = bState;
      break;
      
    case CLOCK_PERIPH_SERCOM7 :
      MCLK->APBDMASK.bit.SERCOM7_ = bState;
      break;
      
    case CLOCK_PERIPH_SUPC :
      MCLK->APBAMASK.bit.SUPC_ = bState;
      break;
      
    case CLOCK_PERIPH_TC0 :
      MCLK->APBAMASK.bit.TC0_ = bState;
      break;
      
    case CLOCK_PERIPH_TC1 :
      MCLK->APBAMASK.bit.TC1_ = bState;
      break;
      
    case CLOCK_PERIPH_TC2 :
      MCLK->APBBMASK.bit.TC2_ = bState;
      break;
      
    case CLOCK_PERIPH_TC3 :
      MCLK->APBBMASK.bit.TC3_ = bState;
      break;
      
    case CLOCK_PERIPH_TC4 :
      MCLK->APBCMASK.bit.TC4_ = bState;
      break;
      
    case CLOCK_PERIPH_TC5 :
      MCLK->APBCMASK.bit.TC5_ = bState;
      break;
      
    case CLOCK_PERIPH_TC6 :
      MCLK->APBDMASK.bit.TC6_ = bState;
      break;
      
    case CLOCK_PERIPH_TC7 :
      MCLK->APBDMASK.bit.TC7_ = bState;
      break;
      
    case CLOCK_PERIPH_TCC0 :
      MCLK->APBBMASK.bit.TCC0_ = bState;
      break;
      
    case CLOCK_PERIPH_TCC1 :
      MCLK->APBBMASK.bit.TCC1_ = bState;
      break;
      
    case CLOCK_PERIPH_TCC2 :
      MCLK->APBCMASK.bit.TCC2_ = bState;
      break;
      
    case CLOCK_PERIPH_TCC3 :
      MCLK->APBCMASK.bit.TCC3_ = bState;
      break;
      
    case CLOCK_PERIPH_TCC4 :
      MCLK->APBDMASK.bit.TCC4_ = bState;
      break;
      
    case CLOCK_PERIPH_USB :
      MCLK->AHBMASK.bit.USB_ = bState;
      MCLK->APBBMASK.bit.USB_ = bState;
      break;
      
    case CLOCK_PERIPH_TRNG :
      MCLK->APBCMASK.bit.TRNG_ = bState;
      break;
      
    case CLOCK_PERIPH_WDT :
      MCLK->APBAMASK.bit.WDT_ = bState;
      break;
      
    default :
      break;
  }
}

static U32 GetGenClkFreq( CLOCKGENID eId )
{
  U32                 uGenFreq;
  GCLK_GENCTRL_Type*  ptGenCtl;

  // get a pointer to the generator control
  ptGenCtl = ( GCLK_GENCTRL_Type* )&GCLK->GENCTRL[ eId ];

  // get the frequency
  switch ( ptGenCtl->bit.SRC )
  {
    case CLOCK_SRC_XOSC0 :
      uGenFreq = auXoscFreqs[ 0 ];
      break;

    case CLOCK_SRC_XOSC1 :
      uGenFreq = auXoscFreqs[ 1 ];
      break;

    case CLOCK_SRC_GCLKIN :
      break;

    case CLOCK_SRC_GCLKGEN1 :
      uGenFreq = GetGenClkFreq ( CLOCK_GENID_01 );
      break;

    case CLOCK_SRC_OSCULP32K :
      uGenFreq = 32768;
      break;

    case CLOCK_SRC_XOSC32K :
      uGenFreq = 32768;
      break;

    case CLOCK_SRC_DFLL48M :
      uGenFreq = 48000000;
      break;

    case CLOCK_SRC_DPLL0 :
      uGenFreq = auDpllFreqs[ 0 ];
      break;

    case CLOCK_SRC_DPLL1 :
      uGenFreq = auDpllFreqs[ 1 ];
      break;

    default :
      uGenFreq = 0;
      break;
  }

  // if freqency is not zero
  if ( uGenFreq != 0 )
  {
    // calculate the divisor
    if ( ptGenCtl->bit.DIVSEL )
    {
      // power of 2
      uGenFreq /= ( U32 )( BIT( ptGenCtl->bit.DIV ) + 1 );
    }
    else
    {
      // straight divisor
      uGenFreq /= ptGenCtl->bit.DIV;
    }
  }

  // enable/disable the peripheral clocks
  MCLK->AHBMASK.reg = g_tClockMainDef.tMclkAhb.reg;
  MCLK->APBAMASK.reg = g_tClockMainDef.tMclkApbA.reg;
  MCLK->APBBMASK.reg = g_tClockMainDef.tMclkApbB.reg;
  MCLK->APBCMASK.reg = g_tClockMainDef.tMclkApbC.reg;
  MCLK->APBDMASK.reg = g_tClockMainDef.tMclkApbD.reg;

  // return the generator frequency
  return( uGenFreq );
}

/******************************************************************************
 * @function ConfigureExternalOsc
 *
 * @brief configure the external oscillator
 *
 * This function will configure the external oscillaotr
 *
 * @param[in]   ptOscDef    pointer to oscillator definition
 * @param[in]   eOscSel     oscillator select
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
static BOOL ConfigureExternalOsc( CLOCKOSC eOscSel, PCLOCKXOSCKDEF ptOscDef )
{
  BOOL                    bStatus = FALSE;
  OSCCTRL_XOSCCTRL_Type*  ptOscCtrl;
  OSCCTRL_XOSCCTRL_Type   tOscCtrl;
  U8                      nOscIndex;

  // get the oscillator control 
  switch( eOscSel )
  {
    case CLOCK_SRC_XOSC0 :
      ptOscCtrl = ( OSCCTRL_XOSCCTRL_Type* )&REG_OSCCTRL_XOSCCTRL0;
      nOscIndex = 0;
      break;

    case CLOCK_SRC_XOSC1 :
      ptOscCtrl = ( OSCCTRL_XOSCCTRL_Type* )&REG_OSCCTRL_XOSCCTRL1;
      nOscIndex = 1;
      break;

    default :
      ptOscCtrl = NULL;
      break;
  }

  // check for valid oscillator
  if ( FALSE == bStatus )
  {
    // now set the values
    tOscCtrl.bit.ENABLE     = ON;
    tOscCtrl.bit.XTALEN     = ptOscDef->tFlags.bXtalEnable;
    tOscCtrl.bit.RUNSTDBY   = ptOscDef->tFlags.bRunInStandby;
    tOscCtrl.bit.ONDEMAND   = ptOscDef->tFlags.bOnDemand;
    tOscCtrl.bit.LOWBUFGAIN = ptOscDef->tFlags.bLowBufGainEnable;
    tOscCtrl.bit.ENALC      = ptOscDef->tFlags.bEnableLoopControl;
    tOscCtrl.bit.CFDEN      = ptOscDef->tFlags.bClkFailDetEnable;
    tOscCtrl.bit.SWBEN      = ptOscDef->tFlags.bClkFailSwitchBack;
    tOscCtrl.bit.IPTAT      = ptOscDef->nOscIReference;
    tOscCtrl.bit.IMULT      = ptOscDef->nOscIMultiplier;
    tOscCtrl.bit.CFDPRESC   = ptOscDef->eFailPrescale;

    // write it
    ptOscCtrl->reg = tOscCtrl.reg;

    // set the clock frequency
    auXoscFreqs[ nOscIndex ] = ptOscDef->uXtalFrequency;
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ConfigureExt32KOsc
 *
 * @brief configure the external 32K oscillator
 *
 * This function will configure the external 32K oscillaotr
 *
 * @param[in]   ptOscDef    pointer to oscillator definition
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
static BOOL ConfigureExt32KOsc( PCLOCKOSC32KDEF ptOsc32Def )
{
  BOOL                    bStatus = FALSE;
  OSC32KCTRL_XOSC32K_Type tOscCtrl;

  // fill in the structure
  tOscCtrl.bit.ENABLE   = ON;
  tOscCtrl.bit.XTALEN   = ptOsc32Def->tFlags.bXtalEnable;
  tOscCtrl.bit.EN32K    = ptOsc32Def->tFlags.bEnable32KOut;
  tOscCtrl.bit.EN1K     = ptOsc32Def->tFlags.bEnable1KOut;
  tOscCtrl.bit.RUNSTDBY = ptOsc32Def->tFlags.bRunInStandby;
  tOscCtrl.bit.ONDEMAND = ptOsc32Def->tFlags.bOnDemand;
  tOscCtrl.bit.STARTUP  = ptOsc32Def->eClkStartup;
  tOscCtrl.bit.CGM      = ptOsc32Def->eGainMode;

  // write it
   OSC32KCTRL->XOSC32K = tOscCtrl;

  // return the status
  return( bStatus );
}

/**@} EOF Clock.c */
