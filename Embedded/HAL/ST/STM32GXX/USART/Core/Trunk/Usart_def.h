/******************************************************************************
 * @file USART_def.h
 *
 * @brief USART definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * USART channels
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup USART
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USART_DEF_H
#define _USART_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define USART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a USART channel
#define USART_ASYNC_DEF( baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet ) \
    .uBaudRate    = baudrate, \
    .eSamp        = USART_SAMP_16X, \
    .eDataOrder   = USART_DATAORDER_LSB, \
    .eParity      = parity, \
    .eWrdLen      = wrdlen, \
    .eStopBit     = stopbits, \
    .wRxBufSize   = rxsize, \
    .wTxBufSize   = txsize, \
    .pnRxBuffer   = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer   = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback   = ( PVUSARTINTCALLBACK )callback, \
    .tEventFlags  = \
    { \
      .bRxChar    = rcvchr, \
      .bRxErr     = rcverr, \
      .bTxEmp     = txemp, \
      .bTxCmp     = txcmp, \
      .bBrkDet    = brkdet \
    }
  
// enumerations ---------------------------------------------------------------
/// enumerate the parity selection
typedef enum _USARTPARITY
{
  USART_PARITY_NONE = 0,   ///< no parity
  USART_PARITY_EVEN = 2,   ///< even parity
  USART_PARITY_ODD,        ///< odd parity
} USARTPARITY;

/// enumerate the word length
typedef enum _USARTWRDLEN
{
  USART_WRDLEN_7 = 0,          ///< 7 bits
  USART_WRDLEN_8,          ///< 8 bits
  USART_WRDLEN_9,          ///< 9 bits
  USART_WRDLEN_MAX
} USARTWRDLEN;

/// enumerate the stop bits
typedef enum _USARTSTOPBIT
{
  USART_STOPBIT_1 = 0,     ///< 1 bit
  USART_STOPBIT_0_5,       ///< half a bit
  USART_STOPBIT_2,         ///< 2 bits
  USART_STOPBIT_1_5,        ///< 1.5 bits
  USART_STOPBIT_MAX
} USARTSTOPBIT;

/// enumerate the modes
typedef enum _USARTMODE
{
  USART_MODE_ASYNC = 0,    ///< Async mode, external clock
  USART_MODE_SYNC,         ///< sync mode
  USART_MODE_LIN,          ///< LIN mode
  USART_MODE_SMCARD,       ///< smart card mode
  USART_MODE_MAX
} USARTMODE;

/// enumerate the sample rate
typedef enum _USARTSAMPLE
{
  USART_SAMP_16X = 0,       ///< sample 16x
  USART_SAMP_8X,            ///< sampe 8X
  USART_SAMP_MAX
} USARTSAMP;

/// enumerate the USART events
typedef enum _USARTIRQEVENTS
{
  USART_IRQ_EVENT_NONE,    ///< no event
  USART_IRQ_EVENT_RXCHAR,  ///< receive character event
  USART_IRQ_EVENT_RXERR,   ///< receive error event
  USART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  USART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  USART_IRQ_EVENT_BRKDET   ///< break detect event
} USARTIRQEVENTS;

/// enumerate the data order
typedef enum _USARTDATAORDER
{
  USART_DATAORDER_LSB = 0,   ///< data sent/rcvd with LSB first
  USART_DATAORDER_MSB,       ///< data sent/rcvd with MSB first
  USART_DATAORDER_MAX,
} USARTDATAORDER;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUSARTINTCALLBACK )( USARTIRQEVENTS, U8 );

/// define the structure to define a USART channel
typedef struct _USARTDEF
{
  USARTMODE           eMode;        ///< USART clock mode
  U32                 uBaudRate;    ///< baud rate
  USARTSAMP           eSamp;        ///< sampling
  USARTPARITY         eParity;      ///< parity
  USARTWRDLEN         eWrdLen;      ///< word length
  USARTSTOPBIT        eStopBit;     ///< number of stop bits
  USARTDATAORDER      eDataOrder;   ///< data order
  U16                 wRxBufSize;   ///< RX buffer size
  U16                 wTxBufSize;   ///< RX buffer size
  PU8                 pnRxBuffer;   ///< RX buffer pointer
  PU8                 pnTxBuffer;   ///< TX buffer pointer
  PVUSARTINTCALLBACK  pvCallback;   ///< callback
  struct
  {
    BOOL              bRxChar : 1;    ///< event on receive character
    BOOL              bRxErr  : 1;    ///< event on receive error
    BOOL              bTxEmp  : 1;    ///< event on transmit empty
    BOOL              bTxCmp  : 1;    ///< event on transmit shift register empty
    BOOL              bBrkDet : 1;    ///< event on break detect
  } tEventFlags;
} USARTDEF, *PUSARTDEF;
#define USARTDEF_SIZE        sizeof( USARTDEF )

/**@} EOF USART_def.h */

#endif  // _USART_DEF_H