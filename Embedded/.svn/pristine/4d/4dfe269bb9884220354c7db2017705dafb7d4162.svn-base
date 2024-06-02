/******************************************************************************
 * @file LedIS31FL3195.h
 *
 * @brief IS31FL3195  declarations
 *
 * This file provides the declarations fof the IS31FL3195 led driver
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
 *
 * \addtogroup LedIS31FL3195
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _LedIS31FL3195_H
#define _LedIS31FL3195_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedIS31FL3195/LedIS31FL3195_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _LEDIS31FL3195ERROR
{
  LEDIS31FL3195_ERROR_NONE = 0,
  LEDIS31FL3195_ERROR_ILLDEV,
  LEDIS31FL3195_ERROR_ILLCHAN,
  LEDIS31FL3195_ERROR_ILLPWMVAL
} LEDIS31FL3195ERROR;

/// enumerate the channels
typedef enum _LEDIS31FL3195CHANENUM
{
  LEDIS31FL3195_CHAN_ENUM_RED = 0,///< LEDIS31FL3195_CHAN_ENUM_RED
  LEDIS31FL3195_CHAN_ENUM_GRN,    ///< LEDIS31FL3195_CHAN_ENUM_GRN
  LEDIS31FL3195_CHAN_ENUM_BLU,    ///< LEDIS31FL3195_CHAN_ENUM_BLU
  LEDIS31FL3195_CHAN_ENUM_WHT,    ///< LEDIS31FL3195_CHAN_ENUM_WHT
  LEDIS31FL3195_CHAN_ENUM_MAX     ///< LEDIS31FL3195_CHAN_ENUM_MAX
} LEDIS31FL3195CHANENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL                LedIS31FL3195_Initialize( void );
extern  LEDIS31FL3195ERROR  LedIS31FL3195_SetIntensity( LEDIS31FL3195DEVENUM eDev, LEDIS31FL3195CHANENUM eChan, U8 nIntensity );

/**@} EOF LedIS31FL3195.h */

#endif  // _LedIS31FL3195_H
