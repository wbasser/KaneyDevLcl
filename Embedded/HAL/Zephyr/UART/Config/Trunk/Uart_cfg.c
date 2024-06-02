/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuration implementation
 *
 * This file provides the UART configuration implementation
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
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// static function prototypes -------------------------------------------------

// global parameter declarations ----------------------------------------------

/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

// local parameter declarations -----------------------------------------------

/// device configuration table
const UARTDEF g_atUartDefs[ UART_DEV_ENUM_MAX ] = 
{
  // define using the below macro
  // UART_ASYNC_DEF( devName, baudrate, parity, wrdlen, stopbits, flowctl, rxsize, txsize, name, callback, rcvchr, rcverr, txemp, cts, rxto )
};  


#if ( UART_CHAN5_IN_USE == ON )
  /******************************************************************************
   * @function UartChan5IrqCallback
   *
   * @brief UART channel 5 IRQ callback
   *
   * This function will handle the IRQ events for UART channel 00
   *
   * @param[in]     eEvent    IRQ event
   * @param[in]     nOption   optioanal data
   *
   *****************************************************************************/
  static void UartChan5IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
  {
    // process the event
    switch( eEvent )
    {
      // receive character event
      case UART_IRQ_EVENT_RXCHAR :
        break;

      // receive error event
      case UART_IRQ_EVENT_RXERR :
        break;

      // transmit empty event
      case UART_IRQ_EVENT_TXEMP :
        break;

      // transmit complete event
      case UART_IRQ_EVENT_TXCMP :
        break;

      // transmit enable event
      case UART_IRQ_EVENT_TXENB :
        break;

      // break detect event
      case UART_IRQ_EVENT_BRKDET :
        break;

      default :
        break;
    }
  }
#endif

/**@} EOF Uart_cfg.c */
