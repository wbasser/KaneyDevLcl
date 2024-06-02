/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
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
 * $Rev: $
 * 
 *
 * \addtogroup GPIO
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// constant parameter initializations -----------------------------------------
const CODE GPIOPINDEF atGpioPinDefs[ GPIO_PIN_ENUM_MAX ] = 
{
  // declare the pins using one of the helper macros below
  // GPIOOUTPINDEF( port, pin, opendrn, speed, puppdn, invert, init )
  // GPIOINPINDEF( port, pin, opendrn, speed, puppdn, invert, filt, schmidt ) 
  // GPIOFNCDEF( port, pin, opendrn, speed, puppdn, func )
  GPIOOUTPINDEF( GPIO_PORT_D,  0, GPIO_OPENDRN_DSB, GPIO_SPEED_LOW, GPIO_PUPPDN_NONE, TRUE, OFF ),
  GPIOINPINDEF(  GPIO_PORT_C,  5, GPIO_OPENDRN_DSB, GPIO_SPEED_LOW, GPIO_PUPPDN_UP,   TRUE, TRUE, OFF ),
  GPIOFNCDEF(    GPIO_PORT_B,  6, GPIO_OPENDRN_DSB, GPIO_SPEED_HIGH, GPIO_PUPPDN_NONE, GPIO_FUNCMUX_07 ),
  GPIOFNCDEF(    GPIO_PORT_B,  7, GPIO_OPENDRN_DSB, GPIO_SPEED_HIGH, GPIO_PUPPDN_NONE, GPIO_FUNCMUX_07 ),
  GPIOFNCDEF(    GPIO_PORT_B,  9, GPIO_OPENDRN_DSB, GPIO_SPEED_HIGH, GPIO_PUPPDN_NONE, GPIO_FUNCMUX_04 ),
  GPIOFNCDEF(    GPIO_PORT_B, 10, GPIO_OPENDRN_DSB, GPIO_SPEED_HIGH, GPIO_PUPPDN_NONE, GPIO_FUNCMUX_04 ),
};

//const CODE GPIOIRQDEF atGpioIrqDefs[ GPIO_IRQ_ENUM_MAX ] =
//{
//  // declare the pins using one of the helper macros below
//};

/**@} EOF GPIO_cfg.c */
