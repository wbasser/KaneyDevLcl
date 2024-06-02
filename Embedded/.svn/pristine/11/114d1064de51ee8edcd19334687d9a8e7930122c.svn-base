/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
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
 * $Log: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct PACKED _BUFCTL
{
  U16       wWrIdx;       ///< write index
  U16       wRdIdx;       ///< read index
  U16       wCount;       ///< buffer count
  U16       wTxBufSize;   ///< xmit buffer size
  U16       wRxBufSize;   ///< recv buffer size
  PU8       pnRxBuf;      ///< pointer to the receive buffer
  PU8       pnTxBuf;      ///< pointer to the transmit buffer
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  const struct device*    ptUart;
  BUFCTL                  tRxBuf;       ///< receive buffer control
  BUFCTL                  tTxBuf;       ///< transmit buffer control
  UARTFLAGS               tEventFlags;  ///< event flags
  PVUARTINTCALLBACK       pvCallback;   ///< callback
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  LCLCTL  atUartLclCtls[ UART_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  void  InterruptCallback( const struct device* ptUart, PVOID pvUserData );

/******************************************************************************
 * @function Uart_Initialize
 *
 * @brief UART initialization
 *
 * This function will initialize UART module. Rx will be disabled on initialization,
 * and only enabled when it's needed. Hopefully after all initialization.
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL Uart_Initialize( void )
{
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;
  UARTDEVENUM                 eDev;
  struct uart_config          uartCfg;
  BOOL                        bRet = TRUE;

  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ];

    // check for a valid device
    ptCtl->ptUart = ptDef->ptDevice;
    if (ptCtl->ptUart != NULL)
    {
      // disable the receive interrupt
      uart_irq_rx_disable( ptCtl->ptUart );

      // copy the buffer pointers and sizes
      ptCtl->tRxBuf.pnRxBuf = ptDef->pnRxBuffer;
      ptCtl->tTxBuf.pnTxBuf = ptDef->pnTxBuffer;
      ptCtl->tRxBuf.wRxBufSize = ptDef->wRxBufSize;
      ptCtl->tTxBuf.wTxBufSize = ptDef->wTxBufSize;

      // copy the callback
      ptCtl->pvCallback = ptDef->pvCallback;
      
      // copy the event flags
      memcpy( &ptCtl->tEventFlags, &ptDef->tEventFlags, UARTFLAGS_SIZE );
      
      // clear the receive/transmit buffers
      ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
      ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;

      // fill the config register
      uartCfg.baudrate  = ptDef->uBaudRate;
      uartCfg.parity    = ptDef->eParity;
      uartCfg.stop_bits = ptDef->eStopBit;
      uartCfg.data_bits = ptDef->eWrdLen;
      uartCfg.flow_ctrl = ptDef->eFlowControl;

      // now configure
      bRet = ( uart_configure( ptCtl->ptUart, &uartCfg ) == 0 ) ? FALSE : TRUE;

      // Pass the UART dev enum into the *user_data field
      uart_irq_callback_user_data_set( ptCtl->ptUart, InterruptCallback, ptCtl );
      
      // enable the receive interrupt
      uart_irq_rx_enable( ptCtl->ptUart );
    }
  }

  // return the status
  return( bRet );
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR             eError = UART_ERR_NONE;
  PLCLCTL             ptCtl;
  U16                 wBytesRemaining;
  U16                 wIdx;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptCtl = &atUartLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesRemaining = ptCtl->tTxBuf.wTxBufSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( ptCtl->tTxBuf.pnTxBuf + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= ptCtl->tTxBuf.wTxBufSize;
      ptCtl->tTxBuf.wCount++;
    }
    Interrupt_Enable( );

    // enable the data register enmpty interrupt
    uart_irq_tx_enable( ptCtl->ptUart );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR   eError = UART_ERR_NONE;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptCtl = &atUartLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesAvailable = ptCtl->tRxBuf.wRxBufSize - ptCtl->tRxBuf.wCount;
    *pwBytesRead = MIN( wBytesAvailable, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( pnData + wIdx ) = *( ptCtl->tRxBuf.pnRxBuf + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= ptCtl->tRxBuf.wRxBufSize;
      ptCtl->tRxBuf.wCount--;
    }
    Interrupt_Enable( );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Ioctl
 *
 * @brief UART IOCTL functions
 *
 * This function provides functionality to modify the UARTs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Ioctl( UARTDEVENUM eDev, UARTACTION eAction, PVOID pvData )
{
  UARTERR eError = UART_ERR_NONE;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Close
 *
 * @brief Close the UART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Close( UARTDEVENUM eDev )
{
  UARTERR       eError = UART_ERR_NONE;
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;

  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {

    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ]; 

    // First, ensure that any ongoign transmissions are stopped
    uart_tx_abort(ptCtl->ptUart);

    // Now disable receiving
    uart_rx_disable(ptCtl->ptUart);
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function InterruptCallback
 *
 * @brief common interrupt callback
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void InterruptCallback( const struct device* ptUart, PVOID pvUserData )
{
  U8            nData;
  PLCLCTL       ptLclCtl;
  UARTIRQEVENTS eEvent;

  // cast the user data pointer
  ptLclCtl = ( PLCLCTL )pvUserData;
  
  // update the IRQ status
  if ( uart_irq_update( ptUart ) != 0 )
  {
    // check for receive ready
    if ( uart_irq_rx_ready( ptUart ))
    {
      // read all characters in fifo
      while( uart_fifo_read( ptUart, &nData, 1 ) != 0 )
      {
        // process the bytes read
        if (( ptLclCtl->pvCallback != NULL ) & ( ptLclCtl->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          ptLclCtl->pvCallback( eEvent, nData );
        }
        // check for room in local buffer
        else if ( ptLclCtl->tRxBuf.wCount < ptLclCtl->tRxBuf.wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptLclCtl->tRxBuf.pnRxBuf + ptLclCtl->tRxBuf.wWrIdx++ ) = nData;
          ptLclCtl->tRxBuf.wWrIdx %= ptLclCtl->tRxBuf.wRxBufSize;
          ptLclCtl->tRxBuf.wCount++;
        }
      }
    }
  
    // check for transmit empty
    if ( uart_irq_tx_ready( ptUart ))
    {
      // check for characters available
      if ( ptLclCtl->tTxBuf.wCount != 0 )
      {
        // output the character
        nData = *( ptLclCtl->tTxBuf.pnTxBuf + ptLclCtl->tTxBuf.wRdIdx++ );
        uart_fifo_fill( ptUart, &nData, 1 );
        ptLclCtl->tTxBuf.wRdIdx %= ptLclCtl->tTxBuf.wTxBufSize;
        ptLclCtl->tTxBuf.wCount--;
          
        // check for callback
        if (( ptLclCtl->pvCallback != NULL ) && ( ptLclCtl->tEventFlags.bTxEmp ))
        {
          // call back with the appropriate TX event
          eEvent = UART_IRQ_EVENT_TXEMP;
          nData = 0;
          ptLclCtl->pvCallback( eEvent, nData );
        }
      }
      else
      {
        // turn off the data register empty interrupt
      }
    }
  }
}

/**@} EOF Uart.c */
