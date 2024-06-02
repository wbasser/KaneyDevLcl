/******************************************************************************
 * @file TestRoutine.h  
 *
 * @brief test routine declarations
 *
 * This file provides the test routine declarations
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
 * \addtogroup TestRoutine
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TestRoutine/TestRoutine.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function TestRoutine_Initialize
 *
 * @brief initialization
 *
 * This function will perform any initializaiton required
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL TestRoutine_Initialize( void )
{
  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function TestRoutine_ProcessEvent
 *
 * @brief event process task
 *
 * This function will process the event
 *
 * @param[in]   xArg    event 
 *
 * @return      TRUE if event handler, FALSE otherwise
 *
 *****************************************************************************/
BOOL TestRoutine_ProcessEvent( TASKARG xArg )
{
  // process the event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // toggle the status led
      Gpio_Toggle( GPIO_PIN_ENUM_STSLED );
      break;

    default :
      break;
  }

  /// return true to flush events
  return( TRUE );
}
 
/**@} EOF TestRoutine.c */
