/******************************************************************************
 * @file Usart.c
 *
 * @brief USART implementation
 *
 * This file provides the implemntation for the USART peripherals
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
 * \addtogroup USART
 * @{
 *****************************************************************************/

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "USART/Usart.h"

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
static  U16             ComputeBaudrate( PUSARTDEF ptDef );

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
 * @function Usart_Initialize
 *
 * @brief USART initialization
 *
 * This function will initialize a given device 
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL Usart_Initialize( void )
{
  PUSARTDEF       ptDef;
  PLCLCTL         ptCtl;
  U16             wBaudRate;
  U32             uTemp;
  
  // get pointer to the definition/control structures
  ptDef = ( PUSARTDEF )&g_tUsartDef;
  ptCtl = &tLclCtl;

  // clear the receive/transmit buffers
  ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
  ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
  
  // now compute the baudrate
  ComputeBaudrate( ptDef );

  // set up the control registers
  USART1->CR2 = ( ptDef->eStopBit << USART_CR2_STOP_Pos );
  USART1->CR3 = USART_CR3_EIE;

  // enable the interrupts
  NVIC_EnableIRQ( USART1_IRQn );
  
  // now setup CR1
  uTemp = USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
  uTemp |= BITS( ptDef->eParity, USART_CR1_PS_Pos );
  switch( ptDef->eWrdLen )
  {
    case USART_WRDLEN_7 :
      uTemp |= USART_CR1_M1;
      break;

    case USART_WRDLEN_9 :
      uTemp |= USART_CR1_M0;
      break;

    default :
      break;
  }
  uTemp |= ( ptDef->eSamp == USART_SAMP_8X ) ? USART_CR1_OVER8 : 0;
  uTemp |= USART_CR1_UE;
  USART1->CR1 = uTemp;

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function Usart_CloseAll
 *
 * @brief USART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void Usart_CloseAll( void )
{
}

/******************************************************************************
 * @function Usart_Write
 *
 * @brief write some characters to the USART
 *
 * This function will write some characters to the USART buffer if room
 *
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
USARTERR Usart_Write( PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  USARTERR        eError = USART_ERR_NONE;
  PUSARTDEF       ptDef;
  PLCLCTL         ptCtl;
  U16             wBytesRemaining, wIdx;
 
  // set the bytes written to zero
  *pwBytesWritten = 0;

  // get pointers to the control/def structure
  ptDef = ( PUSARTDEF )&g_tUsartDef;
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
  USART1->CR1 |= ( USART_CR1_TXEIE | USART_CR1_TCIE );

  // enable the interrupt
  Interrupt_Enable( );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Usart_Read
 *
 * @brief read some characters from the USART
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
USARTERR Usart_Read(PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  USARTERR  eError = USART_ERR_NONE;
  PUSARTDEF ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // get pointers to the control/def structure
  ptDef = ( PUSARTDEF )&g_tUsartDef;
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
 * @function Usart_Ioctl
 *
 * @brief USART IOCTL functions
 *
 * This function provides functionality to modify the USARTs parameters
 *
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
USARTERR Usart_Ioctl( USARTACTION eAction, PVOID pvData )
{
  USARTERR eError = USART_ERR_NONE;
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Usart_Close
 *
 * @brief Close the USART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
USARTERR Usart_Close(  )
{
  USARTERR eError = USART_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function USART_Handler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 *****************************************************************************/
void USART1_IRQHandler( void )
{
  PUSARTDEF           ptDef;
  PLCLCTL             ptCtl;
  U8                  nOption;
  PVUSARTINTCALLBACK  pvCallBack;
  U32                 uIntFlags;
  USARTIRQEVENTS      eEvent = USART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUSARTDEF )&g_tUsartDef;
  ptCtl = &tLclCtl;
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = USART1->ISR;

  // was this a RX interrupt
  if ( uIntFlags & USART_ISR_RXNE )
  {
    // get the data
    nOption = ( U8 )USART1->RDR;

    // check for an error
    if ( uIntFlags & USART_ISR_ORE )
    {
      // clear the error flag
      USART1->ICR = USART_ICR_ORECF;
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
          eEvent = USART_IRQ_EVENT_RXCHAR;
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
  if ( uIntFlags & USART_ISR_TXE )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      USART1->TDR = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = USART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      USART1->CR1 &= ~USART_CR1_TXEIE;
    }
  }  
    
  // check for a transmit complete interrupt
  if ( uIntFlags & USART_ISR_TC )
  {
    // clear the interrupt
    USART1->ICR = USART_ICR_TCCF;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = USART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );

        // turn off the transmit complete interrupt
        USART1->CR1 &= ~USART_CR1_TCIE;
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
static U16 ComputeBaudrate( PUSARTDEF ptDef )
{
  U32   uPeripheralClock, uTemp;
  U8    nPrescale = -1;
  U16   wBaudRate = 0;
  
  // get the peripheral clock
  uPeripheralClock = Clock_GetPerClkMuxFrequency( CLKMUX_ENUM_USART1 );
   
  // check for baud rate outside of normal range
  do
  {
    nPrescale++;
    uTemp = uPeripheralClock / ptDef->uBaudRate / awPrescaleValues[ nPrescale ];
  } while ( uTemp > 65535 );
 
  // set the baudrate
  USART1->BRR = uTemp;
  USART1->PRESC = nPrescale;  
}

/**@} EOF Usart.c */
