/******************************************************************************
 * @file LpUart_cfg.c
 *
 * @brief LPUART configuraiton implementation
 *
 * This file pvoides the LPUART configuration implementation
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
 * $Rev: $
 * 
 *
 * \addtogroup LPUART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LPUART/LpUart_cfg.h"

// library includes -----------------------------------------------------------
#include "DbgAscCommandHandler/DbgAscCommandHandler.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// static function prototypes -------------------------------------------------
static  void  LpUartIrqCallback( LPUARTIRQEVENTS eEvent, U8 nOption );

// global parameter declarations ----------------------------------------------
/// declare the buffers
// LPUART_BUFFER( name, rxsize, txsize );
LPUARTBUFFER( DbgChar, DBGASCCMDHANDLER_RCVBUF_SIZE, DBGASCCMDHANDLER_XMTBUF_SIZE ); 

/// device configuration table
const LPUARTDEF g_tLpUartDef = 
{
  // LPUART_ASYNC_DEF( baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet )
  LPUARTASYNCDEF( 115200, LPUART_PARITY_NONE, LPUART_WRDLEN_8, LPUART_STOPBIT_1, DbgChar, DBGASCCMDHANDLER_RCVBUF_SIZE, DBGASCCMDHANDLER_XMTBUF_SIZE, LpUartIrqCallback, ON, OFF, OFF, OFF, OFF )
};  


/******************************************************************************
 * @function LpUartIrqCallback
 *
 * @brief LPUART IRQ callback
 *
 * This function will handle the IRQ events for LPUART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void LpUartIrqCallback( LPUARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case LPUART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      TaskManager_PostEventIrq( TASK_SCHD_ENUM_DBGHAND, ( TASKARG )nOption );
      break;

    // receive error event
    case LPUART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case LPUART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case LPUART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case LPUART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/**@} EOF LPUART_cfg.c */
