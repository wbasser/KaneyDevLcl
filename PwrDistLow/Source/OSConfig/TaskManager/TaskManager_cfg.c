/******************************************************************************
 * @file TaskManager_cfg.c
 *
 * @brief task manager configuration implementation
 *
 * This file contains the task definitions structures
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup TaskManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager_cfg.h"

// library includes -----------------------------------------------------------
#include "DbgAscCommandHandler/DbgAscCommandHandler.h"
#include "TestRoutine/TestRoutine.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// declare event buffers here
// TASKEVENTSTORAGE( bufname, size )
TASKEVENTSTORAGE( DbgChar, DBGASCCOMMANDHANDLER_NUM_EVENTS );
TASKEVENTSTORAGE( TestRoutine, TSTROUT_NUM_EVENTS );

/// scheduled task table declaration
const CODE TASKSCHDDEF  g_atTaskSchdDefs[ TASK_SCHD_MAX ] =
{
  // TASKSCHD( type, taskhandler, numevents, bufname, executionrate, enabled, runoninit )
  TASKSCHD( TASK_TYPE_TIMED_CONTINUOUS, TestRoutine_ProcessEvent,         TSTROUT_NUM_EVENTS,              TestRoutine, TSTROUT_EXEC_RATE_COUNTS, TRUE, FALSE ),
  TASKSCHD( TASK_TYPE_EVENT,            DbgAscCommandHandler_ProcessChar, DBGASCCOMMANDHANDLER_NUM_EVENTS, DbgChar,                            0, TRUE, FALSE ),
};

#if ( TASK_TICK_ENABLE == 1 )
  /// tick task table declaration
  const CODE TASKTICKDEF  g_atTaskTickDefs[ TASK_TICK_MAX ] = 
  {
    // TASKTICK( taskhandler, executionrate, enabled ),
  };
#endif  // TASK_TICK_ENABLE


/**@} EOF TaskManager_cfg.c */
