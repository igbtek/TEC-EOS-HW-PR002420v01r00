/**
 ******************************************************************************
 * \file eeprom_cfg.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologías EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologías EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section DESCRIPTION DESCRIPTION:
 *
 * File content description.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F_P06-DP_14
 *   - Revision: 08.
 *   - Date: 31/AUG/2020.
 *   - Author: Ernesto Cocoletzi.
 *   - Aprove: Mariano Centeno Camarena.
 ******************************************************************************
*/

#ifndef  EEPROM_CFG_H_
#define  EEPROM_CFG_H_

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

#define	EEPROM_SDA_PIN									//IOPORT_CREATE_PIN(PIOA, 24)
#define	EEPROM_SCL_PIN									//IOPORT_CREATE_PIN(PIOA, 25)

/*===========================================================================*\
 * TWI Port configuration
\*===========================================================================*/
#define EEPROM_WC_PIN									//IOPORT_CREATE_PIN(PIOA, 26)
#define EEPROM_DEVICE_IDENTIFIER						0x0A
#define EEPROM_TWI										TWI0
#define EEPROM_TWI_MASTER_CLK							sysclk_get_cpu_hz()
#define EEPROM_TWI_STANDARD_MODE_SPEED					200000
#define EEPROM_TWI_FAST_MODE_SPEED						400000
#define EEPROM_TWI_CHIP_ADDR(internal_addr)				( (EEPROM_DEVICE_IDENTIFIER << 3) | ( ((internal_addr >> 16) & 0x07) << 0) )


/*===========================================================================*\
 * Brief description for each block of constant definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macro definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif
 