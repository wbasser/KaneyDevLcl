/******************************************************************************
 * @file LpUart.c
 *
 * @brief LPUART implementation
 *
 * This file provides the implemntation for the LPUART peripherals
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
 * \addtogroup LPUART
 * @{
 *****************************************************************************/

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "LPUART/LpUart.h"

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  BUFCTL            tRxBuf;       ///< receive buffer control
  BUFCTL            tTxBuf;       ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  LCLCTL      tLclCtl;

// local function prototypes --------------------------------------------------
static  U16             ComputeBaudrate( PLPUARTDEF ptDef );

// constant parameter initializations -----------------------------------------
static const U16  awPrescaleValues[ ] =
{
  1,
  2,
  4,
  6,
  8,
  10,
  12,
  16,
  32,
  64,
  128,
  256
};

/******************************************************************************
 * @function LpUart_Initialize
 *
 * @brief LPUART initialization
 *
 * This function will initialize a given device 
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LpUart_Initialize( void )
{
  PLPUARTDEF       ptDef;
  PLCLCTL         ptCtl;
  U16             wBaudRate;
  U32             uTemp;
  
  // get pointer to the definition/control structures
  ptDef = ( PLPUARTDEF )&g_tLpUartDef;
  ptCtl = &tLclCtl;

  // clear the receive/transmit buffers
  ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
  ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
  
  // now compute the baudrate
  ComputeBaudrate( ptDef );

  // set up the control registers
  LPUART1->CR2 = ( ptDef->eStopBit << LPUART_CR2_STOP_Pos );
  LPUART1->CR3 = LPUART_CR3_EIE;

  // enable the interrupts
  NVIC_EnableIRQ( LPUART1_IRQn );
  
  // now setup CR1
  uTemp = LPUART_CR1_RE | LPUART_CR1_TE | LPUART_CR1_RXNEIE;
  uTemp |= BITS( ptDef->eParity, LPUART_CR1_PS_Pos );
  switch( ptDef->eWrdLen )
  {
    case LPUART_WRDLEN_7 :
      uTemp |= LPUART_CR1_M1;
      break;

    case LPUART_WRDLEN_9 :
      uTemp |= LPUART_CR1_M0;
      break;

    default :
      break;
  }
  uTemp |= ( ptDef->eSamp == LPUART_SAMP_8X ) ? LPUART_CR1_OVER8 : 0;
  uTemp |= LPUART_CR1_UE;
  LPUART1->CR1 = uTemp;

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function LpUart_CloseAll
 *
 * @brief LPUART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void LpUart_CloseAll( void )
{
}

/******************************************************************************
 * @function LpUart_Write
 *
 * @brief write some characters to the LPUART
 *
 * This function will write some characters to the LPUART buffer if room
 *
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
LPUARTERR LpUart_Write( PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  LPUARTERR        eError = LPUART_ERR_NONE;
  PLPUARTDEF       ptDef;
  PLCLCTL         ptCtl;
  U16             wBytesRemaining, wIdx;
 
  // set the bytes written to zero
  *pwBytesWritten = 0;

  // get pointers to the control/def structure
  ptDef = ( PLPUARTDEF )&g_tLpUartDef;
  ptCtl = &tLclCtl;
  
  // compute the bytes written
  wBytesRemaining = ptDef->wTxBufSize - ptCtl->tTxBuf.wCount;
  *pwBytesWritten = MIN( wBytesRemaining, wLength );
  
  // for each byte
  Interrupt_Disable( );
  for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
  {
    // put in buffer/adjust pointer/increment count
    *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
    ptCtl->tTxBuf.wWrIdx %= ptDef->wTxBufSize;
    ptCtl->tTxBuf.wCount++;
  }

  // enable the data register enmpty interrupt
  LPUART1->CR1 |= ( LPUART_CR1_TXEIE | LPUART_CR1_TCIE );

  // enable the interrupt
  Interrupt_Enable( );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function LpUart_Read
 *
 * @brief read some characters from the LPUART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
LPUARTERR LpUart_Read(PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  LPUARTERR  eError = LPUART_ERR_NONE;
  PLPUARTDEF ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // get pointers to the control/def structure
  ptDef = ( PLPUARTDEF )&g_tLpUartDef;
  ptCtl = &tLclCtl;
  
  // compute the bytes written
  wBytesAvailable = ptDef->wRxBufSize - ptCtl->tRxBuf.wCount;
  *pwBytesRead = MIN( wBytesAvailable, wLength );
  
  // for each byte
  Interrupt_Disable( );
  for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
  {
    // put in buffer/adjust pointer/increment count
    *( pnData + wIdx ) = *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wRdIdx++ );
    ptCtl->tRxBuf.wRdIdx %= ptDef->wRxBufSize;
    ptCtl->tRxBuf.wCount--;
  }
  Interrupt_Enable( );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function LpUart_Ioctl
 *
 * @brief LPUART IOCTL functions
 *
 * This function provides functionality to modify the LPUARTs parameters
 *
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
LPUARTERR LpUart_Ioctl( LPUARTACTION eAction, PVOID pvData )
{
  LPUARTERR eError = LPUART_ERR_NONE;
  // return the error
  return( eError );
}

/******************************************************************************
 * @function LpUart_Close
 *
 * @brief Close the LPUART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
LPUARTERR LpUart_Close(  )
{
  LPUARTERR eError = LPUART_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function LPUART_Handler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 *****************************************************************************/
void LPUART1_IRQHandler( void )
{
  PLPUARTDEF           ptDef;
  PLCLCTL             ptCtl;
  U8                  nOption;
  PVLPUARTINTCALLBACK  pvCallBack;
  U32                 uIntFlags;
  LPUARTIRQEVENTS      eEvent = LPUART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PLPUARTDEF )&g_tLpUartDef;
  ptCtl = &tLclCtl;
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = LPUART1->ISR;

  // was this a RX interrupt
  if ( uIntFlags & LPUART_ISR_RXNE )
  {
    // get the data
    nOption = ( U8 )LPUART1->RDR;

    // check for an error
    if ( uIntFlags & LPUART_ISR_ORE )
    {
      // clear the error flag
      LPUART1->ICR = LPUART_ICR_ORECF;
    }
    else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = LPUART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( uIntFlags & LPUART_ISR_TXE )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      LPUART1->TDR = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = LPUART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      LPUART1->CR1 &= ~LPUART_CR1_TXEIE;
    }
  }  
    
  // check for a transmit complete interrupt
  if ( uIntFlags & LPUART_ISR_TC )
  {
    // clear the interrupt
    LPUART1->ICR = LPUART_ICR_TCCF;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = LPUART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );

        // turn off the transmit complete interrupt
        LPUART1->CR1 &= ~LPUART_CR1_TCIE;
      }
    }
  }
}

/******************************************************************************
 * @function ComputeBaudRate
 *
 * @brief computes the step and divisor for a given baudrate
 *
 * This function will determine the best step and divisor for a given
 * baud rate
 *
 * @param[in]   uBaudrate   desired baudrate
 *
 *****************************************************************************/
static U16 ComputeBaudrate( PLPUARTDEF ptDef )
{
  U32   uPeripheralClock, uTemp;
  U8    nPrescale = -1;
  U16   wBaudRate = 0;
  
  // get the peripheral clock
  uPeripheralClock = Clock_GetPerClkMuxFrequency( CLKMUX_ENUM_LPUART1 );
   
  // check for baud rate outside of normal range
  do
  {
    nPrescale++;
    uTemp = uPeripheralClock / ptDef->uBaudRate / awPrescaleValues[ nPrescale ];
  } while ( uTemp > 65535 );
 
  // set the baudrate
  LPUART1->BRR = uTemp;
  LPUART1->PRESC = nPrescale;  
}

/**@} EOF LpUart.c */
