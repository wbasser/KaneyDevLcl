/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
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
 * $Rev: $
 * 
 *
 * \addtogroup GPIO
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// library includes -----------------------------------------------------------
#include "hardware/gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Gpio_Initialize
 *
 * @brief initialize the GPIO handler
 *
 * This function initialized the GPIO handler, configures each pin in the list
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL Gpio_Initialize( void )
{
  PGPIOPINDEF   ptDef;
  PGPIOFNCDEF   ptFnc;
  PGPIOIRQDEF   ptIrq;
  U8            nIdx;
  
  // for each pin def in the list
  for( nIdx = 0; nIdx < GPIO_PIN_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ nIdx ];
    
    // initialize it
    gpio_init( ptDef->nPin );
    
    // determine mode
    switch( ptDef->eMode )
    {
      case GPIO_MODE_INPUT :
        break;
        
      case GPIO_MODE_INPUT_PULLDN :
        gpio_set_pulls( ptDef->nPin, FALSE, TRUE );
        break;
        
      case GPIO_MODE_INPUT_PULLUP :
        gpio_set_pulls( ptDef->nPin, TRUE, FALSE );
        break;
        
      case GPIO_MODE_INPUT_PULLUPDN :
        gpio_set_pulls( ptDef->nPin, TRUE, TRUE );
        break;
        
      case GPIO_MODE_OUTPUT_SLOW :
      case GPIO_MODE_OUTPUT_FAST :
        // set the direction/slew rate/output strength
        gpio_set_dir( ptDef->nPin, GPIO_DIR_OUT );
        gpio_set_slew_rate( ptDef->nPin, ( ptDef->eMode == GPIO_MODE_OUTPUT_FAST ));
        gpio_set_drive_strength( ptDef->nPin, ptDef->eDrive );
        break;
        
      default :
        break;
    }
    
    // now check for initial on
    gpio_put( ptDef->nPin, ptDef->bInitialState ^ ptDef->bInvert );
  }

  // set up the function pins
  for ( nIdx = 0; nIdx < GPIO_FNC_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptFnc = ( PGPIOFNCDEF )&g_atGpioFncDefs[ nIdx ];
    
    // set the function
    gpio_set_function( ptFnc->nPin, ptFnc->eFunc );
  }
  
  // for each interrupt
  for ( nIdx = 0; nIdx < GPIO_IRQ_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ nIdx ];
  }
  
  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function Gpio_Close
 *
 * @brief close all GPIO functions
 *
 * This function will turn off all GPIO interrups
 *
 *****************************************************************************/
void Gpio_Close( void )
{
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // check for an output pin
    if (( ptDef->eMode != GPIO_MODE_OUTPUT_FAST ) || ( ptDef->eMode != GPIO_MODE_OUTPUT_SLOW ))
    {
      // set the appropriate state
      gpio_put( ptDef->nPin, ( bState ^ ptDef->bInvert ) ? OFF : ON );
    }
    else
    {
      // set the error
      eError = GPIO_ERR_ILLDIR;
    }    
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Get
 *
 * @brief get a GPIO pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];
    
    // get the value
    *pbState = (( gpio_get( ptDef->nPin ) ^ ptDef->bInvert ));   
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Toggle
 *
 * @brief toggle a GPIO pin
 *
 * This function will toggle the state of a GPIO pin
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  U32         uMask;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // get the mask
    uMask = BIT( ptDef->nPin );
    gpio_xor_mask( uMask);
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Ioctl
 *
 * @brief GPIO I/O control
 *
 * This function allows the state, interrupt level, interrupt callback and
 * other parameters to be set/retrieved
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   eGpioAct  IOCTL action
 * @param[i0]   pvData    pointer to the data to set/retrieve
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData )
{
  GPIOERR           eError = GPIO_ERR_NONE;
  PGPIOFNCDEF       ptFnc;
  PGPIOPINDEF       ptPin;
  PGPIOIRQDEF       ptIrq;
  GPIODIR           eDir;
  BOOL              bAction;
  U8                nPinOfs;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // perform the action
    switch( eGpioAct )
    {
      case GPIO_ACT_SETDIR :
        break;
        
      case GPIO_ACT_SETMODE :
        break;
        
      case GPIO_ACT_ENBDSBIRQ :
        break;

      case GPIO_ACT_SETFUNCMUX :
        break;

      default :
        // set the error
        eError = GPIO_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Refresh
 *
 * @brief refresh the state of all the GPIO pins
 *
 * This function sell update the direction and current state of all GPIO pins
 *
  *****************************************************************************/
void Gpio_Refresh( void )
{
}

/**@} EOF Gpio.c */
