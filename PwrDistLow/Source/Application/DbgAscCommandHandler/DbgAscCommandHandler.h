/******************************************************************************
 * @file DbgAscCommandHandler.h
 *
 * @brief Debug command Handler
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of Cyber
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
 
// ensure only one instantiation
#ifndef _DBGASCCOMMANDHANDLER_H
#define _DBGASCCOMMANDHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the RX/TX buffer sizes
#define DBGASCCMDHANDLER_RCVBUF_SIZE      ( 0 )
#define DBGASCCMDHANDLER_XMTBUF_SIZE      ( 80 )

/// define the number of events/arguments
#define DBGASCCOMMANDHANDLER_NUM_EVENTS       ( 80u )
#define DBGASCCOMMANDHANDLER_NUM_ARGS         ( 8u ) 

/// define the end of line character
#define DBGASCCOMMANDHANDLER_EOL_CHAR         ( '\r' )

/// define the prompt character
#define DBGASCCOMMANDHANDLER_PROMPT_CHAR      ( '>' )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE ASCCMDENTRY g_atDbgAscCommandHandlerTable[ ];
extern  const CODE C8          g_szDbgAscCommandHandlerDelims[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  DbgAscCommandHandler_Initialize( void );
extern  BOOL  DbgAscCommandHandler_ProcessChar( TASKARG xArg );
extern  void  DbgAscCommandHandler_PutChar( U8 nChar );
extern  void  DbgAscCommandHandler_Write( PU8 pnData, U16 wLength );

/**@} EOF DbgAscCommandHandler.h */

#endif  // _DBGASCCOMMANDHANDLER_H