/******************************************************************************
 * @file LpUart_def.h
 *
 * @brief LpUart definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * LpUart channels
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
 * \addtogroup LpUart
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LPUART_DEF_H
#define _LPUART_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define LPUARTBUFFER( name, rxsize, txsize ) \
  static U8 an ## name ## RxBuf[ rxsize ]; \
  static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a LpUart channel
#define LPUARTASYNCDEF( baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet ) \
  .uBaudRate    = baudrate, \
  .eSamp        = LPUART_SAMP_16X, \
  .eDataOrder   = LPUART_DATAORDER_LSB, \
  .eParity      = parity, \
  .eWrdLen      = wrdlen, \
  .eStopBit     = stopbits, \
  .wRxBufSize   = rxsize, \
  .wTxBufSize   = txsize, \
  .pnRxBuffer   = ( PU8 )&an ## name ## RxBuf, \
  .pnTxBuffer   = ( PU8 )&an ## name ## TxBuf, \
  .pvCallback   = ( PVLPUARTINTCALLBACK )callback, \
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
typedef enum _LPUARTPARITY
{
  LPUART_PARITY_NONE = 0,   ///< no parity
  LPUART_PARITY_EVEN = 2,   ///< even parity
  LPUART_PARITY_ODD,        ///< odd parity
} LPUARTPARITY;

/// enumerate the word length
typedef enum _LPUARTWRDLEN
{
  LPUART_WRDLEN_7 = 0,      ///< 7 bits
  LPUART_WRDLEN_8,          ///< 8 bits
  LPUART_WRDLEN_9,          ///< 9 bits
  LPUART_WRDLEN_MAX
} LPUARTWRDLEN;

/// enumerate the stop bits
typedef enum _LPUARTSTOPBIT
{
  LPUART_STOPBIT_1 = 0,     ///< 1 bit
  LPUART_STOPBIT_0_5,       ///< half a bit
  LPUART_STOPBIT_2,         ///< 2 bits
  LPUART_STOPBIT_1_5,        ///< 1.5 bits
  LPUART_STOPBIT_MAX
} LPUARTSTOPBIT;

/// enumerate the modes
typedef enum _LPUARTMODE
{
  LPUART_MODE_ASYNC = 0,    ///< Async mode, external clock
  LPUART_MODE_SYNC,         ///< sync mode
  LPUART_MODE_LIN,          ///< LIN mode
  LPUART_MODE_SMCARD,       ///< smart card mode
  LPUART_MODE_MAX
} LPUARTMODE;

/// enumerate the sample rate
typedef enum _LPUARTSAMPLE
{
  LPUART_SAMP_16X = 0,       ///< sample 16x
  LPUART_SAMP_8X,            ///< sampe 8X
  LPUART_SAMP_MAX
} LPUARTSAMP;

/// enumerate the LpUart events
typedef enum _LPUARTIRQEVENTS
{
  LPUART_IRQ_EVENT_NONE,    ///< no event
  LPUART_IRQ_EVENT_RXCHAR,  ///< receive character event
  LPUART_IRQ_EVENT_RXERR,   ///< receive error event
  LPUART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  LPUART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  LPUART_IRQ_EVENT_BRKDET   ///< break detect event
} LPUARTIRQEVENTS;

/// enumerate the data order
typedef enum _LPUARTDATAORDER
{
  LPUART_DATAORDER_LSB = 0,   ///< data sent/rcvd with LSB first
  LPUART_DATAORDER_MSB,       ///< data sent/rcvd with MSB first
  LPUART_DATAORDER_MAX,
} LPUARTDATAORDER;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVLPUARTINTCALLBACK )( LPUARTIRQEVENTS, U8 );

/// define the structure to define a LpUart channel
typedef struct _LPUARTDEF
{
  LPUARTMODE            eMode;        ///< LpUart clock mode
  U32                   uBaudRate;    ///< baud rate
  LPUARTSAMP            eSamp;        ///< sampling
  LPUARTPARITY          eParity;      ///< parity
  LPUARTWRDLEN          eWrdLen;      ///< word length
  LPUARTSTOPBIT         eStopBit;     ///< number of stop bits
  LPUARTDATAORDER       eDataOrder;   ///< data order
  U16                   wRxBufSize;   ///< RX buffer size
  U16                   wTxBufSize;   ///< RX buffer size
  PU8                   pnRxBuffer;   ///< RX buffer pointer
  PU8                   pnTxBuffer;   ///< TX buffer pointer
  PVLPUARTINTCALLBACK   pvCallback;   ///< callback
  struct
  {
    BOOL                bRxChar : 1;    ///< event on receive character
    BOOL                bRxErr  : 1;    ///< event on receive error
    BOOL                bTxEmp  : 1;    ///< event on transmit empty
    BOOL                bTxCmp  : 1;    ///< event on transmit shift register empty
    BOOL                bBrkDet : 1;    ///< event on break detect
  } tEventFlags;
} LPUARTDEF, *PLPUARTDEF;
#define LPUARTDEF_SIZE        sizeof( LPUARTDEF )

/**@} EOF LpUart_def.h */

#endif  // _LPUART_DEF_H