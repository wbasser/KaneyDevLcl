/******************************************************************************
 * @file LedIS31FL3195_cfg.h
 *
 * @brief IS31FL3195 configuration declarations
 *
 * This file provides the declarations for the IS31FL3195 led driver
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
#ifndef _LedIS31FL3195_CFG_H
#define _LedIS31FL3195_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedIS31FL3195/LedIS31FL3195_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _LEDIS31FL3195DEVENUM
{
  /// add device enumerations here
  LEDIS31FL3195_DEV_ENUM_STS = 0,
  LEDIS31FL3195_DEV_ENUM_USR,

  /// do not remove this entry
  LEDIS31FL3195_DEV_ENUM_MAX
} LEDIS31FL3195DEVENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern const CODE LEDIS31FL3195DEF g_atLedIS31FL3195Devs[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  LedIS31FL3195_WriteReg( U8 nDevAddr, U8 nRegAddr, U8 nData );
extern  BOOL  LedIS31FL3195_ReadReg( U8 nDevAddr, U8 nRegAddr, PU8 pnData );

/**@} EOF LedIS31FL3195_cfg.h */

#endif  // _LedIS31FL3195_CFG_H
