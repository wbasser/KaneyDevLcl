/******************************************************************************
 * @file BLEManagerZephyr_cfg.c
 *
 * @brief BLE manager (ZEPHYR) configuration implmentation
 *
 * This file provides the configruation implmentation
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup BLEManagerZephyr_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BLEManagerZephyr/BLEManagerZephyr_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const BLESVCFUNCS g_atBleServices[ BLESVC_FUNC_ENUM_MAX ] =
{
};

/******************************************************************************
 * @function BLEManagerZephyr_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors detected, false otherwise
 *
 *****************************************************************************/
BOOL BLEManagerZephyr_LocalInitialize( void )
{
  BOOL  bStatus = FALSE;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 
/**@} EOF BLEManagerZephyr_cfg.c */
