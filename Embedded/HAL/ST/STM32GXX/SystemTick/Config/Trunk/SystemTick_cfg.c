/******************************************************************************
 * @file SystemTick_cfg.c
 *
 * @brief System Tick configuration structures
 *
 * This file conatins the initilization structures
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/


// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick_cfg.h"

// library includes -------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  #include "TaskScheduler/TaskScheduler.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  #include "FreeRTOS.h"
  #include "FreeRTOSConfig.h"
#endif // SYSTEMDEFFIN_OS_SELECTON

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
extern void xPortSysTickHandler(void);

// global parameter declarations ----------------------------------------------
const CODE PVSYSTEMTICKFUNC apvSystemTickFunctions[ ] = 
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager_TickProcess,
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
    TaskScheduler_TickProcess,
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xPortSysTickHandler,
  #endif // SYSTEMDEFFIN_OS_SELECTON
  NULL
};


/**@} EOF SystemTick_cfg.c */
