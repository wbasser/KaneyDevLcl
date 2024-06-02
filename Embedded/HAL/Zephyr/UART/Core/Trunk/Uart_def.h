/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * UART channels
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Uart_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a UART channel
#define UART_DEF( device, baudrate, parity, wrdlen, stopbits, flowctl, rxsize, txsize, name, callback, rcvchr, rcverr, txemp, cts, rxto ) \
  { \
    .ptDevice = DEVICE_DT_GET(DT_NODELABEL(device)), \
    .uBaudRate = baudrate, \
    .eParity = parity, \
    .eWrdLen = wrdlen, \
    .eStopBit = stopbits, \
    .eFlowControl = flowctl, \
    .wRxBufSize = rxsize, \
    .wTxBufSize = txsize, \
    .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback = ( PVUARTINTCALLBACK )callback, \
    .tEventFlags.bRxChar = rcvchr, \
    .tEventFlags.bRxErr = rcverr, \
    .tEventFlags.bTxEmp = txemp, \
    .tEventFlags.bCts = cts, \
    .tEventFlags.bRxto = rxto, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the parity selection
typedef enum _UARTPARITY
{
  UART_PARITY_NONE = 0,   ///< no parity
  UART_PARITY_ODD,        ///< odd parity
  UART_PARITY_EVEN,       ///< even parity
  UART_PARITY_MARK,       ///< mark parity
  UART_PARITY_SPACE,      ///< mark space,
  UART_PARITY_MAX
} UARTPARITY;

/// enumerate the word length
typedef enum _UARTWRDLEN
{
  UART_WRDLEN_5 = 0,      ///< 5 bits
  UART_WRDLEN_6,          ///< 6 bits
  UART_WRDLEN_7,          ///< 7 bits
  UART_WRDLEN_8,          ///< 8 bits
  UART_WRDLEN_9,          ///< 9 bits
  UART_WRDLEN_MAX
} UARTWRDLEN;

/// enumerate the stop bits
typedef enum _UARTSTOPBIT
{
  UART_STOPBIT_05 = 0,    ///< 1/2 bit
  UART_STOPBIT_1,         ///< 1 bit
  UART_STOPBIT_15,        ///< 1 bit
  UART_STOPBIT_2,         ///< 2 bits
  UART_STOPBIT_MAX
} UARTSTOPBIT;

/// enumerate the UART events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_NONE = 0,  ///< no event
  UART_IRQ_EVENT_RXCHAR,    ///< receive character event
  UART_IRQ_EVENT_RXERR,     ///< receive error event
  UART_IRQ_EVENT_TXEMP,     ///< transmit empty event
  UART_IRQ_EVENT_CTS,       ///< CTS event
  UART_IRQ_EVENT_RXTO,      ///< Receive Timeout event
  UART_IRQ_EVENT_MAX
} UARTIRQEVENTS;

/// enumerate the flow control
typedef enum _UARTFLOWCTL
{
  UART_FLOWCTL_NONE = 0,  ///< no flow control
  UART_FLOWCTL_RTSCTS,    ///< RTS-CTS
  UART_FLOWCTL_DTRDSR,    ///< DTR-DSR
  UART_FLOWCTL_RS485,     ///< RS485
} UARTFLOWCTL;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS, U8 );

/// define the flag structure
typedef struct PACKED _UARTFLAGS
{
  BOOL            bRxChar : 1;      ///< event on receive character
  BOOL            bRxErr  : 1;      ///< event on receive error
  BOOL            bTxEmp  : 1;      ///< event on transmit empty
  BOOL            bCts    : 1;      ///< event on change CTS
  BOOL            bRxto   : 1;      ///< RX timeout
} UARTFLAGS, *PUARTFLAGS;
#define UARTFLAGS_SIZE                          sizeof( UARTFLAGS )

/// define the structure to define a UART channel
typedef struct PACKED _UARTDEF
{
  const struct device*  ptDevice;       ///< pointer to the device
  U32                   uBaudRate;      ///< baud rate
  UARTPARITY            eParity;        ///< parity
  UARTWRDLEN            eWrdLen;        ///< word length
  UARTSTOPBIT           eStopBit;       ///< number of stop bits
  UARTFLOWCTL           eFlowControl;   ///< flow control
  U16                   wRxBufSize;     ///< RX buffer size
  U16                   wTxBufSize;     ///< RX buffer size
  PVUARTINTCALLBACK     pvCallback;     ///< callback
  UARTFLAGS             tEventFlags;    ///< event flags
  PU8                   pnRxBuffer;     ///< RX buffer pointer
  PU8                   pnTxBuffer;     ///< TX buffer pointer
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE                          sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H
