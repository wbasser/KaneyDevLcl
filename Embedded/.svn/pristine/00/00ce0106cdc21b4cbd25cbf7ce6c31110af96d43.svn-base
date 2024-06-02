/******************************************************************************
 * @file RedirectionManager_cfg.c
 *
 * @brief redirection manager configuration implementation
 *
 * This file provides the configuration implementation for the redirection manager
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
 * \addtogroup RedirectionManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RedirectionManager/RedirectionManager_cfg.h"

// library includes -----------------------------------------------------------
#include "BleCommunicationsManager/BleCommunicationsManager.h"
#include "GPIO/Gpio.h"
#include "UsbCommunicationsManager/UsbCommunicationsManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE PVREDIRECTOUT  g_apvRedirectOuts[ REDIRECT_ENUM_MAX ] =
{
  UsbCommunicationsManager_PutChar,
  BleCommunicationsManager_PutChar,
};

/******************************************************************************
 * @function RedirectionManager_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL RedirectionManager_LocalInitialize( void )
{

  // return OK
  return( FALSE );
}

/******************************************************************************
 * @function RedirectManager_UpdateRedirectStatus
 *
 * @brief update redirect status
 *
 * This function set the redirection indicator
 *
 * @param[in]   bState    current state
 *
 *****************************************************************************/
void RedirectManager_UpdateRedirectStatus( BOOL bState )
{
  // set the LED based on state
  Gpio_Set( GPIO_PIN_ENUM_REDLED, bState );
}

/**@} EOF RedirectionManager_cfg.c */
