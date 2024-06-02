/******************************************************************************
 * @file USART_cfg.c
 *
 * @brief USART configuraiton implementation
 *
 * This file pvoides the USART configuration implementation
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
 * \addtogroup USART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USART/USART_cfg.h"

// library includes -----------------------------------------------------------
#include "DbgAscCommandHandler/DbgAscCommandHandler.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// static function prototypes -------------------------------------------------
static  void  UsartIrqCallback( USARTIRQEVENTS eEvent, U8 nOption );

// global parameter declarations ----------------------------------------------
/// declare the buffers
// USART_BUFFER( name, rxsize, txsize );
USART_BUFFER( DbgChar, DBGASCCMDHANDLER_RCVBUF_SIZE, DBGASCCMDHANDLER_XMTBUF_SIZE ); 

/// device configuration table
const USARTDEF g_tUsartDef = 
{
  // USART_ASYNC_DEF( baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet )
  USART_ASYNC_DEF( 115200, USART_PARITY_NONE, USART_WRDLEN_8, USART_STOPBIT_1, DbgChar, DBGASCCMDHANDLER_RCVBUF_SIZE, DBGASCCMDHANDLER_XMTBUF_SIZE, UsartIrqCallback, ON, OFF, OFF, OFF, OFF )
};  


/******************************************************************************
 * @function USARTIrqCallback
 *
 * @brief USART IRQ callback
 *
 * This function will handle the IRQ events for USART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UsartIrqCallback( USARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case USART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      TaskManager_PostEventIrq( TASK_SCHD_ENUM_DBGHAND, ( TASKARG )nOption );
      break;

    // receive error event
    case USART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case USART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case USART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case USART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/**@} EOF USART_cfg.c */
