/******************************************************************************
 * @file BLEManagerZephyr.c
 *
 * @brief BLE Manager for zephyr implementation
 *
 * This file provides the implementation for the BLE Manager
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

// system includes ------------------------------------------------------------
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>

// local includes -------------------------------------------------------------
#include "BLEManagerZephyr/BLEManagerZephyr.h"

// library includes -----------------------------------------------------------
#include "StatusManager/StatusManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static void StatusCallback( S32 lError );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function BLEManagerZephyr_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors detected, false otherwise
 *
 *****************************************************************************/
BOOL BLEManagerZephyr_Initialize( void )
{
  BOOL  bStatus = FALSE;

  // call the local initilaizations
  bStatus = BLEManagerZephyr_LocalInitialize();

  // attempt to initialize
  bStatus |= bt_enable( StatusCallback );

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function BLEManagerZephyr_GetAddress
 *
 * @brief return the address
 *
 * This function will return the current address
 *
 * @param[in]   ptAddress     pointer to store the address
 *
 *****************************************************************************/
void BLEManagerZephyr_GetAddress( PBLEADDR ptAddress )
{
  struct bt_le_oob  tOob = { 0 };

  // get the address
	if ( bt_le_oob_get_local( BT_ID_DEFAULT, &tOob ) == 0 ) 
  {
    // copy it
    memcpy( ptAddress, &tOob.addr, sizeof( bt_addr_le_t ));
	}
}

/******************************************************************************
 * @function StatusCallback
 *
 * @brief callback for status change
 *
 * This function will handle the status callback
 *
 * @param[in]   lError    error code
 *
 *****************************************************************************/
static void StatusCallback( S32 lError )
{
  BLESVCFUNCENUM  eSvcFuncEnum;
  PBLESVCFUNCS    ptFunctions;
  U32             uEvent;
  //int             iReturn;

  // check for valid initializaton
  if ( lError == 0 )
  {
    // now for each service in the config file, start them
    for ( eSvcFuncEnum = 0; eSvcFuncEnum < BLESVC_FUNC_ENUM_MAX; eSvcFuncEnum++ )
    {
      // get the entry
      ptFunctions = ( PBLESVCFUNCS )&g_atBleServices[ eSvcFuncEnum ];

      // now call the start function
      ptFunctions->pvBleServices();
    }
  }
  else
  {
    // set the event
    uEvent = STATUSMANAGER_EVENT_BLE_ERROR;
    
    // post the event to the status manager
    k_msgq_put( &g_tStsMngrQueue, ( PVOID )&uEvent, K_NO_WAIT );
  }
}

/**@} EOF BLEManagerZephyr.c */
