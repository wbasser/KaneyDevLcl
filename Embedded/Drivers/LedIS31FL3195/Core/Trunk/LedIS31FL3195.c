/******************************************************************************
 * @file LedIS31FL3195.c
 *
 * @brief LedIS31FL3195 handler implementation
 *
 * This file implements the LedIS31FL3195 handler
 *
  * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 *
 * \addtogroup LedIS31FL3195
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedIS31FL3195/LedIS31FL3195.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define REG_CHIP_ID                             ( 0x00 )
#define REG_SHUTDWN_CTRL                        ( 0x01 )
#define REG_CONFIG_OP_MODE                      ( 0x02 )
#define REG_CHARGE_PUMP1                        ( 0x03 )
#define REG_CHARGE_PUMP2                        ( 0x04 )
#define REG_CURRENT_BAND                        ( 0x05 )
#define REG_HOLD_FUNC                           ( 0x06 )
#define REG_PHASE_DELAY                         ( 0x07 )
#define REG_RED_PWM                             ( 0x40 )
#define REG_GRN_PWM                             ( 0x32 )
#define REG_BLU_PWM                             ( 0x21 )
#define REG_WHT_PWM                             ( 0x10 )
#define REG_CLR_UPDATE                          ( 0x50 )
#define REG_RESET                               ( 0x5F )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static const CODE U8 anChanRegs[ ] =
{
  REG_RED_PWM, REG_GRN_PWM, REG_BLU_PWM, REG_WHT_PWM
};

/******************************************************************************
 * @function LedIS31FL3195_INitialize
 *
 * @brief LedIS31FL3195 handler initialization
 *
 * This function will initialize the LedIS31FL3195 handler
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL LedIS31FL3195_Initialize( void )
{
  BOOL                  bStatus = FALSE;
  LEDIS31FL3195DEVENUM  eDevEnum;
  PLEDIS31FL3195DEF     ptDef;

  // for each defined device
  for( eDevEnum = 0; eDevEnum < LEDIS31FL3195_DEV_ENUM_MAX; eDevEnum++ )
  {
    // get a pointer to the definition
    ptDef = ( PLEDIS31FL3195DEF )&g_atLedIS31FL3195Devs[ eDevEnum ];

    // reset the device
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_RESET, 0xC5 );

    // now set up the device
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_SHUTDWN_CTRL, 0xF1 );
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_CONFIG_OP_MODE, 0 );
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_CHARGE_PUMP1, 0x20 );
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_CHARGE_PUMP2, 0x30 );
    bStatus += LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_CURRENT_BAND, 0xFF );
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function LedIS31FL3195_SetIntensity
 *
 * @brief set an LED's intensity value
 *
 * This function will set the defined channel to a set intensity
 *
 * @param[in]   eDev        device enumeration
 * @param[in[   eChan       channel enumeration
 * @param[in]   nIntensity  desired intensity
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
LEDIS31FL3195ERROR LedIS31FL3195_SetIntensity( LEDIS31FL3195DEVENUM eDev, LEDIS31FL3195CHANENUM eChan, U8 nIntensity )
{
  LEDIS31FL3195ERROR eError = LEDIS31FL3195_ERROR_NONE;
  PLEDIS31FL3195DEF  ptDef;

  // check for valid device
  if ( eDev < LEDIS31FL3195_DEV_ENUM_MAX )
  {
      // now check for a valid channel
      if ( eChan < LEDIS31FL3195_CHAN_ENUM_MAX )
      {
        // get a pointer to the definition
        ptDef = ( PLEDIS31FL3195DEF )&g_atLedIS31FL3195Devs[ eDev ];

        // write the new value
        LedIS31FL3195_WriteReg( ptDef->nDevAddress, anChanRegs[ eChan ], nIntensity );
        LedIS31FL3195_WriteReg( ptDef->nDevAddress, REG_CLR_UPDATE, 0 );
      }
      else
      {
        // set the illegal channel error
        eError = LEDIS31FL3195_ERROR_ILLCHAN;
      }
  }
  else
  {
    // set the illegal device error
    eError = LEDIS31FL3195_ERROR_ILLDEV;
  }

  // return the error
  return ( eError );
}

/**@} EOF LedIS31FL3195.c */
