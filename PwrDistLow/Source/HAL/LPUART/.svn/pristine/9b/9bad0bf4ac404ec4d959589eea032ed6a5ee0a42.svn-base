/******************************************************************************
 * @file LpUart.h
 *
 * @brief LPUART declarations
 *
 * This file provides the declarations for the LPUART devices
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
 
// ensure only one instantiation
#ifndef _LPUART_CFG_H
#define _LPUART_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LPUART/LpUart_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _LPUARTERR
{
  LPUART_ERR_NONE = 0,        ///< no error
  LPUART_ERR_ILLDEV,          ///< illegal device
  LPUART_ERR_ILLPRM,          ///< illegal parameter
  LPUART_ERR_ILLPER,          ///< illegal peripheral
  LPUART_ERR_ILLACT           ///< illegal IOCTL action
} LPUARTERR;

/// enumerate the IOCTL actions
typedef enum _LPUARTACTION
{
  LPUART_ACTION_NONE = 0,     ///< no action
  LPUART_ACTION_SET_BAUDRATE, ///< set baudrate
  LPUART_ACTION_GET_BAUDRATE, ///< get baudrate
  LPUART_ACTION_SET_PARITY,   ///< set parity
  LPUART_ACTION_GET_PARITY,   ///< get parity
  LPUART_ACTION_SET_WRDLEN,   ///< set wordlength
  LPUART_ACTION_GET_WRDLEN,   ///< get wordlength
  LPUART_ACTION_SET_STOPBITS, ///< set stopbits
  LPUART_ACTION_GET_STOPBITS, ///< get stopbits
  LPUART_ACTION_MAX           ///< maximum action selection
} LPUARTACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL      LpUart_Initialize( void );
extern  void      LpUart_CloseAll( void );
extern  LPUARTERR LpUart_Write( PU8 pnData, U16 wLength, PU16 pwBytesWritten );
extern  LPUARTERR LpUart_Read( PU8 pnData, U16 wLength, PU16 pwBytesRead );
extern  LPUARTERR LpUart_Ioctl( LPUARTACTION eAction, PVOID pvData );
extern  LPUARTERR LpUart_Close( void );

/**@} EOF LpUart_cfg.h */

#endif  // _LPUART_CFG_H
