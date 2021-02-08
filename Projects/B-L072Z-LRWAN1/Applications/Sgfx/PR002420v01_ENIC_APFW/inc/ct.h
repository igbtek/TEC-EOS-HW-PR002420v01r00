/**
 ******************************************************************************
 * \file ct.h
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

#ifndef  CT_H
#define  CT_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "ul.h"
#include "dl.h"
#include "gwh0079.h"
#include "low_power_manager.h"
#include "scheduler.h"
#include "st_sigfox_api.h"
#include "radio.h"
#include "sgfx_credentials.h"
#include "hw_eeprom.h"
#include "bsp.h"
#include "timer.h"
#include "at.h"
#include "command.h"
#include "vcom.h"
#include "timer.h"
#include "rep.h"
#include "cm.h"
#include "ucp.h"
#include "timeServer.h"
#include "system.h"
#include "eeprom.h"
#include "stm32l0xx_hal.h"
#include "cm.h"
#include "stm32l0xx_hal.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

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


/*===========================================================================*\
 * TIEMPO DE RESPUESTA DEL MEDIDOR: COMMAND FOR INTRUCTION 0X01
\*===========================================================================*/


/*===========================================================================*\
 * FRECUENCIA DE REPORTEO: COMMAND FOR INTRUCTION 0X02
\*===========================================================================*/
/* Time structure for recuring reporter */
typedef enum recurring_rep_time_type
{
	SET_TIME_1HR =1,
	SET_TIME_2HR,
	SET_TIME_4HR =4,
	SET_TIME_6HR =6,
	SET_TIME_8HR =8,
}RECURRING_REP_TIME_TYPE;


/*===========================================================================*\
 * CONFIRMACION DE REINICIO: COMMAND FOR INTRUCTION 0X04
\*===========================================================================*/
#define SHUTDOWN_SIZE 	9
static const uint8_t SHUTDOWN[SHUTDOWN_SIZE] = {0x73, 0x68, 0x75, 0x74, 0x20, 0x64, 0x6F, 0x77, 0x6E};

/*===========================================================================*\
 * INTERRUPCIONES DE ENERGÍA: COMMAND FOR INTRUCTION 0X05
\*===========================================================================*/
#define FLG_RESET (uint8_t)0xAA
/*===========================================================================*\
 * VALORES PROGRAMADOS: COMMAND FOR INTRUCTION 0X06
\*===========================================================================*/



/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/
extern void CT_execute_instruction(void);
extern void CT_reset_scaling_board(void);

extern void CT_instruction_01(void);
extern void CT_instruction_02(void);
extern void CT_instruction_04(void);
extern void CT_instruction_05(void);
extern void CT_instruction_06(void);
/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
