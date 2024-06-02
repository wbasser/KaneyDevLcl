/******************************************************************************
 * @file I2c.c
 *
 * @brief I2C Sercom implementation
 *
 * This file provides the implementation of the I2C
 *
 * @copyright Copyright (c) 2014 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "hardware/i2c.h"

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------

/// define the stop command value 

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

/// define the local control structure

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function all devices in the table
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL I2c_Initialize( void )
{
  PI2CDEF     ptDef;
  i2c_inst_t* ptI2cInst;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2cInst = ( ptDef->eChan == I2C_CHAN_0 ) ? i2c0 : i2c1;

    // initialize the i2c
    i2c_init( ptI2cInst, ptDef->uBaudrate );
  }

  // return OK
  return( FALSE );
}

/******************************************************************************
 * @function I2c_Close
 *
 * @brief Close the I2C
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Close( I2CDEVENUM eDev )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  i2c_inst_t* ptI2cInst;

  // check for a valid UART
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    ptI2cInst = ( ptDef->eChan == I2C_CHAN_0 ) ? i2c0 : i2c1;

    // initialize the i2c
    i2c_deinit( ptI2cInst );
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function I2c_CLoseAll
 *
 * @brief I2C close all channels
 *
 * This function turns off all I2C channels
 *
 *****************************************************************************/
void I2c_CloseAll( void )
{
  PI2CDEF     ptDef;
  I2CDEVENUM  eDev;
  i2c_inst_t* ptI2cInst;

  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2cInst = ( ptDef->eChan == I2C_CHAN_0 ) ? i2c0 : i2c1;

    // initialize the i2c
    i2c_deinit( ptI2cInst );
  }
}

/******************************************************************************
 * @function I2c_Write
 *
 * @brief write some characters to the I2C
 *
 * This function will write some characters to the I2C buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  int         iReturn;
  i2c_inst_t* ptI2cInst;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2cInst = ( ptDef->eChan == I2C_CHAN_0 ) ? i2c0 : i2c1;

    // check for address's to write
    if ( ptXfrCtl->nAddrLen != 0 )
    {
      // write the address
      iReturn = i2c_write_blocking( ptI2cInst, ptXfrCtl->nDevAddr, ( PU8 )ptXftCtl->tAddress, ptXfrCtl->nAddrLen, ( ptXfrCtl->wDataLen != 0 ));
      if ( iReturn == ptXftCtl->nAddrLen )
      {
        // if there is data to write 
        if ( ptXfrCtl->wDataLen != 0 )
        {
          // write the data
          iReturn = i2c_write_blocking( ptI2cInst, ptXfrCtl->nDevAddr, ( PU8 )ptXftCtl->pnData, ptXfrCtl->wDataLen, FALSE );
          if ( iReturn != ptXfrCtl->wDataLen )
          {
            // set the error
            eError = I2C_ERROR_TIMEOUT;
          }
        }
      }
      else
      {
        eError = I2C_ERROR_ILLDEV;
      }
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }

  // return the status
  return( eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief read some characters from the I2C
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  int         iReturn;
  i2c_inst_t* ptI2cInst;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    // check for address's to write
    if ( ptXfrCtl->nAddrLen != 0 )
    {
      // write the address
      iReturn = i2c_write_blocking( ptI2cInst, ptXfrCtl->nDevAddr, ( PU8 )ptXftCtl->tAddress, ptXfrCtl->nAddrLen, ( ptXfrCtl->wDataLen != 0 ));
      if ( iReturn != ptXftCtl->nAddrLen )
      {
        eError = I2C_ERROR_ILLDEV;
      }
    }

    // if there is data to read 
    if (( ptXfrCtl->wDataLen != 0) && ( eError == I2C_ERROR_NONE )
    {
      // write the data
      iReturn = i2c_read_blocking( ptI2cInst, ptXfrCtl->nDevAddr, ( PU8 )ptXftCtl->pnData, ptXfrCtl->wDataLen, FALSE );
      if ( iReturn != ptXfrCtl->wDataLen )
      {
        // set the error
        eError = I2C_ERROR_TIMEOUT;
      }
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return(( I2CERROR )eError );
}

/******************************************************************************
 * @function I2c_Ioctl
 *
 * @brief I2C IOCTL functions
 *
 * This function provides functionality to modify the I2Cs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  PU32        puData;

  // set the pointer to avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];

    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        break;
        
      case I2C_ACTION_SET_DEVADDR :
        break;
        
      case I2C_ACTION_GET_DEVADDR :
        break;
        
      default :
        // illegal action
        eError = I2C_ERROR_ILLACT;
        break;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return( eError );
}



/**@} EOF I2c.c */
