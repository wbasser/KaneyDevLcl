/******************************************************************************
 * @file I2C.c
 *
 * @brief I2C implementaion
 *
 * This	file provides the implementation
 *
 * @copyright Copyright	(c) 2014 Cyber Integration
 * This	document contains proprietary data and information of Cyber Integration	
 * LLC.	It is the exclusive property of	Cyber Integration, LLC and will	not be 
 * disclosed in	any form to any	party without prior written permission of 
 * Cyber Integration, LLC. This	document may not be reproduced or further used 
 * without the prior written permission	of Cyber Integration, LLC.
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

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------

// enumerations	---------------------------------------------------------------
/// enumerate the clock times
typedef enum _I2CREFCLKENUM
{
  I2C_REFCLK_8MHZ = 0,
  I2C_REFCLK_16MHZ,
  I2C_REFCLK_MAX
} I2CREFCLKENUM;

/// enumerate the transfer direction
typedef enum _I2CXFRDIR
{
  I2C_XFRDIR_WR = 0,
  I2C_XFRDIR_RD,
  I2C_XFRDIR_MAX
} I2CXFRDIR;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef	struct _LCLCTLS
{
  PI2CXFRCTL        ptXfrCtl;   	  ///< transfer control ointer
  I2CXFRDIR         eXfrDir;          ///< transfer direction
  U16               wBufIdx;          ///< buffer index
  U32               uTotalLength;     ///< total data length
  I2CERROR          eError;	          ///< current error
  BOOL              bRunning;	      ///< running state
  PVI2CINTCALLBACK  pvCallBack;       ///< callback
  U8                nBaseIrq;         ///< base IRQ
  I2C_TypeDef*      ptI2c;            ///< pointer to the register set
} LCLCTL, *PLCLCTL;
#define	LCLCTL_SIZE                            sizeof( LCLCTL	)

/// define the baud rate settings
typedef struct _I2CTIMING
{
  U8  nPrescale;          ///< prescale setting
  U8  nSCLL;              ///< SCL Low
  U8  nSCLH;              ///< SCL High
  U8  nSDADel;            ///< SDA Delay
  U8  nSCLDEL;             ///< SCL Delay
} I2CTIMING, *PI2CTIMING;
#define I2CTIMING_SIZE                          sizeof( I2CTIMING )

// local parameter declarations	-----------------------------------------------
static	LCLCTL	    atLclCtls[ I2C_CHAN_MAX ];
static  PLCLCTL     aptLclCtls[ I2C_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  BOOL  SetBaudRate( PI2CDEF ptDef, PLCLCTL ptCtl );
static  void  IrqEventCommon( PLCLCTL ptCtl );
static  void  IrqErrorCommon( PLCLCTL ptCtl );
static	void  ProcessComplete( I2CERROR eError, PLCLCTL ptCtl );
static	void  StartTransfer( PLCLCTL ptCtl );

// constant declarations ------------------------------------------------------
static  const I2CTIMING atTimingSettings[ I2C_REFCLK_MAX ][ 2 ] =
{
  // 8 MHZ ref clock
  {
    { 0x01, 0x13, 0x0F, 0x02, 0x04 }, ///< 100KHZ
    { 0x00, 0x09, 0x03, 0x01, 0x03 }, ///< 400KHZ
  },
  // 16 MHZ ref clock
  {
    { 0x03, 0x13, 0x0F, 0x02, 0x04 }, ///< 100KHZ
    { 0x01, 0x09, 0x03, 0x02, 0x03 }, ///< 400KHZ
  }
};

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This	function all devices in	the table
 *
 * @return    TRUE if errors, FALSE otherwise
 *****************************************************************************/
BOOL I2c_Initialize( void )
{
  BOOL          bStatus = FALSE;
  PI2CDEF	ptDef;
  I2CDEVENUM	eDev;
  PLCLCTL	ptCtl;
  U32		uTemp;
  
  // for each device
  for (	eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev	];
    ptCtl = &atLclCtls[	eDev ];
    aptLclCtls[ I2C_CHAN_1 ] = ptCtl;

    // get the device pointer
    switch( ptDef->eChan )
    {
      case I2C_CHAN_1 :
        ptCtl->ptI2c = I2C1;
        ptCtl->nBaseIrq = I2C1_EV_IRQn;
        break;

      case I2C_CHAN_3 :
        ptCtl->ptI2c = I2C3;
        ptCtl->nBaseIrq = I2C3_EV_IRQn;
        break;

      default :
        bStatus = TRUE;
        break;
    }

    // if valid device
    if ( bStatus == FALSE )
    {
      // ensure it is off
      ptCtl->ptI2c->CR1 = 0;

      // set the baudrate
      if (( bStatus = SetBaudRate( ptDef, ptCtl )) == FALSE )
      {
        // register the interrupt
        NVIC_EnableIRQ( ptCtl->nBaseIrq );
        NVIC_EnableIRQ( ptCtl->nBaseIrq + 1 );

        // configure CR1
        uTemp =  I2C_CR1_ERRIE;
        uTemp |= I2C_CR1_TCIE;
        uTemp |= I2C_CR1_STOPIE;
        uTemp |= I2C_CR1_NACKIE;
        uTemp |= I2C_CR1_RXIE;
        uTemp |= I2C_CR1_TXIE;
        uTemp |= I2C_CR1_PE;
        ptCtl->ptI2c->CR1;

        // copy the callback
        ptCtl->pvCallBack = ptDef->pvCallBack;
      }
    }
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function I2c_CLoseAll
 *
 * @brief I2C close all	channels
 *
 * This	function turns off all I2C channels
 *
 *****************************************************************************/
void I2c_CloseAll( void	)
{
  PLCLCTL	ptCtl;
  I2CDEVENUM	eDev;
  
  // for each device
  for (	eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptCtl = &atLclCtls[ eDev ];

    // diable the interrupts
    NVIC_DisableIRQ( ptCtl->nBaseIrq );
    NVIC_DisableIRQ( ptCtl->nBaseIrq + 1 );

    // disable the control
    ptCtl->ptI2c->CR1 = 0;
  }
}

/******************************************************************************
 * @function I2c_Write
 *
 * @brief write	some characters	to the I2C
 *
 * This	function will write some characters to the I2C buffer if room
 *
 * @param[in]	eDev	    Device enumeration
 * @param[in]	ptXfrCtl    pointero to	the transfer control  
 *
 * @return	appropriate error value
 *
 *****************************************************************************/
I2CERRORI2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR eError = I2C_ERROR_NONE;
  PLCLCTL ptCtl;
  U32	  uReg, uDataLen, uTime;
  U8	  nDataSent;
  BOOL	  bError;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointer to the definition structure
    ptCtl = &atLclCtls[	eDev ];
    
    // copy the	values to the local controls
    ptCtl->ptXfrCtl = ptXfrCtl;
    ptCtl->wBufIdx = 0;

    // set the direction
    ptCtl->eXfrDir = I2C_XFRDIR_WR;

    // clear the error 
    ptCtl->eError = I2C_ERROR_NONE;

    // calculate the length
    ptCtl->uTotalLength = ( ptXfrCtl->nAddrLen + ptXfrCtl->wDataLen );

    // start a transfer
    StartTransfer( ptCtl );

    // wait	till done or timeout
    while( ptCtl->bRunning )
    {
      // check for timeout
      if ( SystemTick_GetTimeMsec( ) >= uTime )
      {
        // timeout occured - flag error
        ptCtl->eError = I2C_ERROR_TIMEOUT;
        ptCtl->bRunning = FALSE;
      }
    }

    // set the return error
    eError = ptCtl->eError;
  }
  else
  {
    eError = I2C_ERROR_ILLDEV;
  }

  // return the	status
  return( eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief read some characters from the	I2C
 *
 * This	function will read bytes from the buffer
 *
 * @param[in]	eDev	    Device enumeration
 * @param[in]	ptXfrCtl    pointero to	the transfer control  
 *
 * @return	appropriate error value
 *
 *****************************************************************************/
I2CERRORI2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR eError = I2C_ERROR_NONE;
  PLCLCTL ptCtl;
  U32	    uTime;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get a pointer to the control structure
    ptCtl = &atLclCtls[	eDev ];
    
    // copy the	values to the local controls
    ptCtl->ptXfrCtl = ptXfrCtl;
    ptCtl->wBufIdx = 0;

    // set the running flag/direction/state
    ptCtl->bRunning = TRUE;
    ptCtl->eError = I2C_ERROR_NONE;
    
    // calculate the length
    ptCtl->uTotalLength = ptXfrCtl->nAddrLen;

    // start the transfer
    StartTransfer( ptCtl );

    // check for a blocking device
    if ( ptCtl->pvCallBack == NULL )
    {
      uTime = SystemTick_GetTimeMsec( ) + ptXfrCtl->uTimeout;
      while( ptCtl->bRunning )
      {
        // check for timeout
       if ( SystemTick_GetTimeMsec( ) >= uTime )
       {
         // timeout occured - flag error
         ptCtl->bRunning = FALSE;
         ptCtl->eError = I2C_ERROR_TIMEOUT;
       }
     }

     // set the return error
     eError = ptCtl->eError;
   }
   else
   {
     // set the error to busy
     eError = I2C_ERROR_BLKING;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the	error
  return(( I2CERROR)eError );
}


/******************************************************************************
 * @function I2c_Ioctl
 *
 * @brief I2C IOCTL functions
 *
 * This	function provides functionality	to modify the I2Cs parameters
 *
 * @param[in]	eDev	    Device enumeration
 * @param[in]	eAction	    action to take
 * @param[io]	pvData	    pointer to data storage/retrieval
 *
 * @return	appropriate error value
 *
 *****************************************************************************/
I2CERRORI2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData )
{
  I2CERROR     eError = I2C_ERROR_NONE;
  PLCLCTL     ptCtl;
  PU32	      puData;
  U32	      uValue;
  PI2CCHKBSY  ptBusyParams;

  // set the pointer to	avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptCtl = &atLclCtls[	eDev ];

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
  
  // return the	error
  return( eError );
}

/******************************************************************************
 * @function I2c_Close
 *
 * @brief Close	the I2C
 *
 * This	function will disable any interrupt, unregister	the interrupt handler
 *
 * @param[in]	eDev	    Device enumeration
 *
 * @return	appropriate error value
 *
 *****************************************************************************/
I2CERRORI2c_Close( I2CDEVENUM eDev )
{
  I2CERROR eError = I2C_ERROR_NONE;
  PLCLCTL ptCtl;
  
  // check for a valid UART
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get the pointer to the USART channel
    ptCtl = &atLclCtls[	eDev ];

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( ptCtl->nBaseIrq );
    NVIC_DisableIRQ( ptCtl->nBaseIrq + 1);

    // turn off	the TWI
    ptCtl->ptI2c->CR1 = 0;

  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the	error
  return( eError );
}

/******************************************************************************
 * @function I2C1_EV_IRQHandler
 *
 * @brief I2C1 event IRQ handler
 *
 * This	function handles the event interrupts from I2C1
 *
 *****************************************************************************/
void I2C1_EV_IRQHandler( void	)
{
  // call the common handler
  IrqEventCommon( aptLclCtls[ I2C_CHAN_1 ] );
}

/******************************************************************************
 * @function I2C1_ER_IRQHandler
 *
 * @brief I2C1 error IRQ handler
 *
 * This	function handles the error interrupts from I2C1
 *
 *****************************************************************************/
void I2C1_ER_IRQHandler( void	)
{
  // call the common handler
  IrqErrorCommon( aptLclCtls[ I2C_CHAN_1 ] );
}

/******************************************************************************
 * @function I2C3_EV_IRQHandler
 *
 * @brief I2C3 event IRQ handler
 *
 * This	function handles the event interrupts from I2C3
 *
 *****************************************************************************/
void I2C3_EV_IRQHandler( void	)
{
  // call the common handler
  IrqEventCommon( aptLclCtls[ I2C_CHAN_3 ] );
}

/******************************************************************************
 * @function I2C3_ER_IRQHandler
 *
 * @brief I2C3 error IRQ handler
 *
 * This	function handles the error interrupts from I2C3
 *
 *****************************************************************************/
void I2C3_ER_IRQHandler( void	)
{
  // call the common handler
  IrqErrorCommon( aptLclCtls[ I2C_CHAN_3 ] );
}

/******************************************************************************
 * @function IrqEventCommon
 *
 * @brief common event interrupt handler
 *
 * This	function checks	for the	type of	interrupts and processes them
 * appropriately
 *
 * @param[in]	ptCtl   pointer to the control
 *
 *****************************************************************************/
static void IrqEventCommon( PLCLCTL ptCtl )
{
  U32	    uStatus;
  U8        nData;
  I2CERROR   eError = I2C_ERROR_NONE;
  
  // get the status register
  uStatus = ptCtl->ptI2c->ISR;

  // test for bus error
  if (( uStatus & I2C_ISR_NACKF ) == I2C_ISR_NACKF )
  {
    // clear the error
    ptCtl->ptI2c->ICR = I2C_ICR_NACKCF;

    // set the error and exit
    ProcessComplete( I2C_ERROR_SLVNAK, ptCtl );
  }
  else if (( uStatus & I2C_ISR_TXIS ) == I2C_ISR_TXIS )
  {
    // output the next byte of data
    if( ptCtl->ptXfrCtl->nAddrLen != 0 )
    {
      // get the address byte/decrement the length/set send data
      nData = ptCtl->ptXfrCtl->tAddress.anValue[ ptCtl->ptXfrCtl->nAddrLen - 1 ];
      ptCtl->ptXfrCtl->nAddrLen--;
    }
    else
    {
      nData = *( ptCtl->ptXfrCtl->pnData + ptCtl->wBufIdx++ );
    }

    // output the data
    ptCtl->ptI2c->TXDR = nData;
  } 
  else if (( uStatus & I2C_ISR_TC ) == I2C_ISR_TC )
  {
    // clear the interrupt
    ptCtl->ptI2c->CR2 = I2C_CR2_STOP;

    // set the error and exit
    ProcessComplete( I2C_ERROR_NONE, ptCtl );
  }
  else if (( uStatus & I2C_ISR_TCR ) == I2C_ISR_TCR )
  {
    //  start another trasfer
      StartTransfer( ptCtl );
  }
  else if (( uStatus & I2C_ISR_RXNE ) == I2C_ISR_RXNE )
  {
    // copy the data
    *( ptCtl->ptXfrCtl->pnData + ptCtl ->wBufIdx++ ) = ptCtl->ptI2c->RXDR;
  }
}

/******************************************************************************
 * @function IrqErrorCommon
 *
 * @brief common error interrupt handler
 *
 * This	function checks	for the	type of	interrupts and processes them
 * appropriately
 *
 * @param[in]	ptCtl   pointer to the control
 *
 *****************************************************************************/
static void IrqErrorCommon( PLCLCTL ptCtl )
{
  U32	    uStatus;
  I2CERROR   eError = I2C_ERROR_NONE;
  
  // get the status register
  uStatus = ptCtl->ptI2c->ISR;

  // test for bus error
  if (( uStatus & I2C_ISR_BERR ) == I2C_ISR_BERR )
  {
    // clear the error
    ptCtl->ptI2c->ICR = I2C_ICR_BERRCF;

    // set the error and exit
    ProcessComplete( I2C_ERROR_BUSFAULT, ptCtl );
  }
  else if (( uStatus & I2C_ISR_ARLO ) == I2C_ISR_ARLO )
  {
    // clear the error
    ptCtl->ptI2c->ICR = I2C_ICR_ARLOCF;

    // set the error and exit
    ProcessComplete( I2C_ERROR_ARBLOST, ptCtl );
  } 
}

/******************************************************************************
 * @function SetBaudrate
 *
 * @brief compute and set the baudrate clock
 *
 * This	function computes the value for	the baud rate register
 *
 * @param[in]	ptDef     pointer to the definitions
 * @param[in]	ptCtl     pointer to the control strucdture
 *
 * @return  TRUE if error sdetected, FALSE otherwise
 *
 *****************************************************************************/
static BOOL SetBaudrate( PI2CDEF ptDef, PLCLCTL ptCtl )
{
  U32           uPeripheralClock, uReg;
  CLKMUXENUM    eClkMuxEnum;
  BOOL          bStatus = FALSE;
  PI2CTIMING    ptTimingDefs;
  I2CREFCLKENUM eRefClkEnum;

  // determine channel
  switch( ptDef->eChan )
  {
    case I2C_CHAN_1 :
      eClkMuxEnum = CLKMUX_ENUM_I2C1;
      break;

    case I2C_CHAN_3 :
      eClkMuxEnum = CLKMUX_ENUM_I2C3;
      break;

    default :
      // set error
      bStatus = TRUE;
      break;
  }
  
  // if valid channel
  if ( bStatus == FALSE )
  {
    // get the peripheral clock
    uPeripheralClock = Clock_GetPerClkMuxFrequency( eClkMuxEnum );
    switch( uPeripheralClock )
    {
      case 8000000ul :
        eRefClkEnum = I2C_REFCLK_8MHZ;
        break;

      case 16000000ul :
        eRefClkEnum = I2C_REFCLK_16MHZ;
        break;

      default :
        // set the error
        bStatus = TRUE;
    }

    // if valid
    if ( bStatus == FALSE )
    {
      // get the pointer
      ptTimingDefs = ( PI2CTIMING )&atTimingSettings[ eRefClkEnum ][ ptDef->bFastSpeed ];

      // build the variable and set it
      uReg = ( ptTimingDefs->nPrescale << I2C_TIMINGR_PRESC_Pos );
      uReg |= ( ptTimingDefs->nSCLDEL << I2C_TIMINGR_SCLDEL_Pos );
      uReg |= ( ptTimingDefs->nSDADel << I2C_TIMINGR_SDADEL_Pos );
      uReg |= ( ptTimingDefs->nSCLH << I2C_TIMINGR_SCLH_Pos );
      uReg |= ( ptTimingDefs->nSCLL << I2C_TIMINGR_SCLL_Pos );
      ptCtl->ptI2c->TIMINGR = uReg;
    }
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ProcessComplete
 *
 * @brief process the read/write complete
 *
 * This	function clears	the funning flag, resets the state, and	posts an event
 * appropriately
 *
 * @param[in]	eError	  error
 * @param[in]	ptCtl	  pointer to the control structure
 *
 *****************************************************************************/
static void ProcessComplete( I2CERROR eError, PLCLCTL ptCtl )
{
  // clear the funning flag/set	the state to idle/set the error
  ptCtl->bRunning = FALSE;
  ptCtl->eError	= eError;
  
  // check to see if the callback is not null
  if ( ptCtl->pvCallBack != NULL )
  {
    // call it
    ptCtl->pvCallBack( eError );
  }
}

/******************************************************************************
 * @function StartTransfer
 *
 * @brief start a transfer
 *
 * This	function starts a tranfer
 *
 * @param[in]	ptCtl	  pointer to the control structure
 *
 *****************************************************************************/
static void StartTransfer( PLCLCTL ptCtl )
{
  U8  nDataSent;
  U32 uReg;

  // compute the actual data sent/adjust the total length
  nDataSent = ptCtl->uTotalLength & 0xFF;
  ptCtl->uTotalLength -= nDataSent;

  // set the reload based on data size
  uReg = ( ptCtl->uTotalLength > 256 ) ? I2C_CR2_RELOAD : I2C_CR2_AUTOEND;

  // set the address and data length
  uReg |= ( ptCtl->ptXfrCtl->nDevAddr << 1 );
  uReg |= ( ptCtl->eXfrDir == I2C_XFRDIR_RD ) ? I2C_CR2_RD_WRN : 0;
  uReg |= ( nDataSent << I2C_CR2_NBYTES_Pos );
  uReg |= I2C_CR2_START;

  // now write it
  ptCtl->ptI2c->CR2 = uReg;
}

/**@} EOF I2c.c	*/
