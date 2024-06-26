/******************************************************************************
 * @file DbgAscCommandHandler.c
 *
 * @brief  Debug Ascii Command Handler
 *
 * This file provides the implementation for the debug ascii commands
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
 * \addtogroup DbgAscCommandHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DbgAscCommandHandler/DbgAscCommandHandler.h"

// library includes -----------------------------------------------------------
#include "AscCommonCommands/AscCommonCommands.h"
//#include "SystemControlManager/SystemControlManager.h"
#include "LPUART/LpUart.h"


// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PC8   pcBuffer;

// local function prototypes --------------------------------------------------
//static  void  LclMsgCallback( DALIBUSMASTERSTS eStatus, U8 nData );

// local command handlers

// constant parameter initializations -----------------------------------------
/// declare the delimiters
const C8 g_szDbgAscCommandHandlerDelims[ ] = 
{ 
  " \t\n\r," 
};

/// command constants

/// string constants

/// responst contants

/// initialize the command table
const CODE ASCCMDENTRY g_atDbgAscCommandHandlerTable[ ] =
{
  // add local command using the below listed macro

  // external command
  ASCCMD_EXTTBL( g_atAscCommonCommandsTable ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/******************************************************************************
 * @function DbgAscCommandHandler_Initialize
 *
 * @brief debug commands initilization
 *
 * This function will initialize the debug command handler
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL DbgAscCommandHandler_Initialize( void )
{
  // get a pointer to the buffer
  AsciiCommandHandler_GetBuffer( ASCCMD_ENUM_DBGCMD, &pcBuffer );

  // output a prompt
  AsciiCommandHandler_OutputPrompt( ASCCMD_ENUM_DBGCMD );

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function DbgAscCommandHandler_ProcessChar
 *
 * @brief process a character event
 *
 * This function will process a character event
 *
 * @param[in]     xArg    task argument
 *
 * @retunr    TRUE to flush events
 * 
 *****************************************************************************/
BOOL DbgAscCommandHandler_ProcessChar( TASKARG xArg )
{
  // handle local
  // AsciiCommandHandler_ProcessChar( ASCCMD_ENUM_DBGCMD, ( U8 )xArg, SystemControlManager_GetMode( ));
  AsciiCommandHandler_ProcessChar( ASCCMD_ENUM_DBGCMD, ( U8 )xArg, 0 );

  // return true
  return( TRUE );
}

/******************************************************************************
 * @function DbgAscCommandHandler_PutChar
 *
 * @brief output a character
 *
 * This function will output a character
 *
 * @param[in]    nChar     character to send
 *
 *****************************************************************************/
void  DbgAscCommandHandler_PutChar( U8 nChar )
{
  DbgAscCommandHandler_Write( &nChar, 1 );
}

/******************************************************************************
 * @function DbgAscCommandHandler_Write
 *
 * @brief output a block of characters
 *
 * This function will output a block of characters to the approrpiate device
 *
 * @param[in]    pcData     pointer to the data to send
 * @param[in]   nLength     length of the data
 *
 *****************************************************************************/
void DbgAscCommandHandler_Write( PU8 pnData, U16 wLength )
{
  U16 wBytesWritten = 0;

  // write a byte of data to the device
  do
  {
    // write a block to the UART
    LpUart_Write( pnData, 1, &wBytesWritten ); 
    wLength -= wBytesWritten;
    pnData += wBytesWritten;
  } while( wLength != 0 ); 
}


/**@} EOF DbgAscCommandHandler.c */