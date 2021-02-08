/**
 ******************************************************************************
 * \file rep.h
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

#ifndef  REP_H
#define  REP_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "b-l072z-lrwan1.h"
#include "gwh0079.h"
#include "hw.h"
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
#include "ul.h"
#include "dl.h"
#include "nic.h"
#include "debug.h"
#include "ucp.h"
#include "timeServer.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
//#define ID_SCALING_TARGET {0x00, 0x00, 0x00, 0x00, 0x0E, 0x54}
#define TIME_CONSTANT 1318

#define MAX_MS_TIME_VAL 		54000000
#define REP_SINAMED_DEFAULT 21600000

#define SET_TIME_MS_1HR  		 3600000

#define REP_RECURRING_TIME_DEFAULT 6
/*#define SET_TIME_MS_2HR	 		 7200000
#define SET_TIME_MS_4HR     14400000
#define SET_TIME_MS_6HR 		21600000
#define SET_TIME_MS_8HR 		28800000
*/
#define DEFAULT_TIME_ABORT_VAL 	5000 

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
typedef enum rep_enum{
	
	IDLE_REP =0,
	FIRST_REP,
	RECURRING_REP,

}REP_ENUM;


typedef enum ins_rep_enum{
	
	IDLE_INS_REP =0,
	FIRST_INS_REP,
	SECCOND_INS_REP,
	THIRD_INS_REP,
	END_INS_REP,
}INS_REP_ENUM;


typedef struct rep_type{

uint32_t first_reporter;				/* Period of first report */
uint32_t recurring_reporter;		/* Period of recurring report */

uint8_t  current_rep;						/* Report in execution */
uint8_t  executing_state;				/* Current state on executing */
bool IsEnd_of_rep; 										/* End of report */
bool IsRep_executing;							/* Report on going execute*/
	
}REP_TYPE;

extern REP_TYPE rep;
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

extern void REP_init_first_rep(void);
extern void REP_init_recurring_rep(void);

extern void REP_main_managger(void);
extern void REP_secuence_of_rep(void);

extern void REP_start_recurring_rep (void);

extern void REP_recurrent_rep(void);
extern void REP_stop_timers(void);
extern void REP_reconfig_recurring_rep(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
