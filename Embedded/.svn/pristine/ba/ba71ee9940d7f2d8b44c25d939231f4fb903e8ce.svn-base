/******************************************************************************
 * @file BLEManagerZephyr
 *
 * @brief BLE managber - zephyr OS
 *
 * This file provides the BLE manager definitions
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
 * \addtogroup BLEManagerZephyr
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BLEMANAGERZEPHYR_H
#define _BLEMANAGERZEPHYR_H

// system includes ------------------------------------------------------------
#include <zephyr/bluetooth/addr.h>

// local includes -------------------------------------------------------------
#include "BLEManagerZephyr/BLEManagerZephyr_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the BLE address structure
typedef bt_addr_le_t            BLEADDR;
typedef bt_addr_le_t*           PBLEADDR;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  BLEManagerZephyr_Initialize( void );
extern  void  BLEManagerZephyr_GetAddress( PBLEADDR ptAddress );

/**@} EOF BLEManagerZephyr.h */

#endif  // _BLEMANAGERZEPHYR_H