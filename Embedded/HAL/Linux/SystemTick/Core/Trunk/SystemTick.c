/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// system includes
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

// library includes
#include "Types/Types.h"
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"
#include "SystemTick/SystemTick_cfg.h"

// local parameter declarations -----------------------------------------------
static  U64               hSystemTime;
static  U32               uTickRateUsec;
static  U32               uDelayTime;
static  struct sigaction  tOldAction;

// local function prototypes
static void SysTickHandler( int iSigNum );

/******************************************************************************
 * @function SystemTick_Initialize
 *
 * @brief this initialized the system tick
 *
 * This function initializes the system tick 
 * 
 * @return	TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL SystemTick_Initialize( void )
{
  BOOL bStatus = FALSE;
  
  struct sigaction tSigAction;
  struct itimerval tTimer;

  // clear the system time
  hSystemTime = 0;
  
  // compute the tick rate in microseconds
  uTickRateUsec = ( 1000000ul / SYSTEMTICK_RATE_HZ );
  
  // Configure the timer to expire after desired rate
  tTimer.it_value.tv_sec = 0;
  tTimer.it_value.tv_usec = uTickRateUsec;
  tTimer.it_interval.tv_sec = 0;
  tTimer.it_interval.tv_usec = uTickRateUsec;
 
  // Start a virtual timer
  if ( setitimer( ITIMER_REAL, &tTimer, NULL ) == 0 )
  {
    // Install timer_handler as the signal handler for SIGVTALRM
    tSigAction.sa_handler = &SysTickHandler;
    tSigAction.sa_flags = SA_NOMASK;
    if ( sigaction( SIGALRM, &tSigAction, &tOldAction ) != 0 )
    {
      // error
      puts( "sigaction error\n" );
      bStatus = TRUE;
    }
  }
  else
  {
    // error
    puts( "setitime error\n" );
  }
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function SystemTick_Close
 *
 * @brief closes the system tick 
 *
 * This function closes the system tick
 * 
 *****************************************************************************/
void SystemTick_Close( void )
{
  struct itimerval tTimer;

 // Configure the timer to expire after desired rate
 tTimer.it_value.tv_sec = 0;
 tTimer.it_value.tv_usec = 0;
 tTimer.it_interval.tv_sec = 0;
 tTimer.it_interval.tv_usec = 0;
 
 // Start a virtual timer
 if ( setitimer( ITIMER_REAL, &tTimer, NULL ) != 0 )
 {
   printf("\nsetitimer() error\n");
 }
 else
 {
   // reset the timer handler
   if ( sigaction( SIGALRM, &tOldAction, NULL ) != 0 )
   {
     printf("\nsigaction() error\n");
   }
 }
}

/******************************************************************************
 * @function SystemTick_GetTickRateUsec
 *
 * @brief gets the tick rate in usecs
 *
 * This function returns the execution rate in USECS
 * 
 * @return  the current tick rate in usecs
 *
 *****************************************************************************/
U32 SystemTick_GetTickRateUsec( void )
{
  // return the system rate in USEC
  return( uTickRateUsec );
}

/******************************************************************************
 * @function SystemTick_GetTimeUsec
 *
 * @brief gets the elapsed system time in USECS
 *
 * This function returns the elapsed time in USECS
 * 
 * @return  the current elapsed time
 *
 *****************************************************************************/
U64 SystemTick_GetTimeUsec( void )
{
  // return the elpased time
  return( hSystemTime );
}

/******************************************************************************
 * @function SystemTick_GetTimeMsec
 *
 * @brief gets the elapsed system time in MSECS
 *
 * This function returns the elapsed time in MSECS
 * 
 * @return  the current elapsed time
 *
 *****************************************************************************/
U32 SystemTick_GetTimeMsec( void )
{
  // return the elapsed time in milliseconds
  return(( U32 )( hSystemTime / 1000 ));
}

/******************************************************************************
 * @function SystemTick_DelayMsec
 *
 * @brief delays for the prescribed milliseconds
 *
 * This function will compute the delay time, and wait till the time expires
 * 
 * @param[in] wMilliSeconds   the time in milliseconds to delay
 *
 *****************************************************************************/
void SystemTick_DelayMsec( U16 wMilliSeconds )
{
  // calculate the delay time
  Interrupt_Disable( );
  uDelayTime = ( wMilliSeconds * 1000 ) / uTickRateUsec;
  Interrupt_Enable( );
}

/******************************************************************************
 * @function SystemTick_IsDelayExpired
 *
 * @brief returns the state of the current delay
 *
 * This function returns TRUE if the current delay time is expired
 * 
 * @return  TRUE if time expired, FALSE if not
 *
 *****************************************************************************/
BOOL SystemTick_IsDelayExpired( void )
{
  // return the state of delay time
  return(( uDelayTime == 0 ) ? TRUE : FALSE );
}

/******************************************************************************
 * @function IrqHandler
 *
 * @brief system tick irq handler
 *
 * This function implements the IRQ handler for the system tick.  It will call
 * any functions listed in the configuration table
 * 
 *****************************************************************************/
static void SysTickHandler( int iSigNum )
{
  U8  nIdx;
  PVSYSTEMTICKFUNC pvTickFunc;
 
  // reset the index
  nIdx = 0;
  
  // adjust the system time
  hSystemTime += uTickRateUsec;

  // process the loop
  while (( pvTickFunc = ( PVSYSTEMTICKFUNC )PGM_RDWORD( apvSystemTickFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvTickFunc( );
  }
}
 
/**@} EOF SystemTick.c */
