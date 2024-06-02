/******************************************************************************
 * @file TestRoutine.h
 *
 * @brief test routine declarations
 *
 * This file provides the declarations for the Test ROutine
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
 
// ensure only one instantiation
#ifndef _TESTROUTINE_H
#define _TESTROUTINE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the task execution rate in MSECS
#define TSTROUT_EXEC_RATE_MSECS                 ( 500 )
#define TSTROUT_EXEC_RATE_COUNTS                ( TASK_TIME_MSECS( TSTROUT_EXEC_RATE_MSECS ))
#define TSTROUT_NUM_EVENTS                      ( 2 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  TestRoutine_Initialize( void );
extern  BOOL  TestRoutine_ProcessEvent( TASKARG xArg );

/**@} EOF TestRoutine.h */

#endif  // _TestRoutine_H