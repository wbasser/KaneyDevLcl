/******************************************************************************
 * @file BLEManagerZephyr_def.h
 *
 * @brief  BLE manager(ZEPHYR) definitions 
 *
 * This file provides the definitions for the BLE maanger
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
 * \addtogroup BLEManagerZephyr_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BLEMANAGERZEPHYR_DEF_H
#define _BLEMANAGERZEPHYR_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the function pointer for a BLE service
typedef void ( *PVBLESVCINIT )( void );

/// define the structure for the BLE Services
typedef struct _BLESVCFUNCS
{
  PVBLESVCINIT  pvBleServices;
} BLESVCFUNCS, *PBLESVCFUNCS;
#define BLESVCFUNCS_SIZE                        sizeof( BLESVCFUNCS )

/**@} EOF BLEManagerZephyr_def.h */

#endif  // _BLEMANAGERZEPHYR_DEF_H