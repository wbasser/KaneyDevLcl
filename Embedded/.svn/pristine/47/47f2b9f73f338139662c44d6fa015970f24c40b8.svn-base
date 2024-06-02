/******************************************************************************
 * @file RedirectionManager_cfg.H
 *
 * @brief redirection manager configuration declarations
 *
 * This file provides the configuration definitions for the redirection manager
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RedirectionManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _REDIRECTIONMANAGER_CFG_H
#define _REDIRECTIONMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the exit character
#define REDIRECT_EXIT_CHAR                  ( 0x01 )    ///< CTRL-A

/// define the macro to enable debug commands
#define REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS  ( ON )

// enumerations ---------------------------------------------------------------
/// enumerate the redirections here
typedef enum _REDIRECTENUM
{
  // add redirections here
  REDIRECT_ENUM_DBG = 0,
  REDIRECT_ENUM_BLE,
  
  // do not remove the below entry
  REDIRECT_ENUM_MAX,
  REDIRECT_ENUM_ILLEGAL = 0xFF
} REDIRECTENUM;

// structures -----------------------------------------------------------------
/// define the redirection output function
typedef void ( *PVREDIRECTOUT )( U8 );

// global parameter declarations -----------------------------------------------
extern  const CODE PVREDIRECTOUT  g_apvRedirectOuts[ REDIRECT_ENUM_MAX ];

// global function prototypes --------------------------------------------------
extern  BOOL  RedirectionManager_LocalInitialize( void );
extern  void  RedirectManager_UpdateRedirectStatus( BOOL bState );


/**@} EOF RedirectionManager_cfg.h */

#endif  // _REDIRECTIONMANAGER_H