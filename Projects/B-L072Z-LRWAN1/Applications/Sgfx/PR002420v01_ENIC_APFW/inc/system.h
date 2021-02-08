/**
 ******************************************************************************
 * \file system.h
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

#ifndef  __SYSTEM_H
#define  __SYSTEM_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
	#include "stm32l0xx_hal.h"
	#include "timer.h"
	#include "b-l072z-lrwan1.h"
	#include "mlm32l07x01.h"
	#include "hw_msp.h"
	#include "ucp.h"
	#include "mfg.h"
	#include "radio.h"
	#include "hw_spi.h"
	#include "hw_rtc.h"
	#include "hw_tim2.h"
	#include "bsp.h"
	#include "sx1276.h"
	#include "hw_eeprom.h"
	#include "hw_gpio.h"
	#include "utilities_conf.h"
	#include "ul.h"
 	#include "nic.h"
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

	#define DEAFULT_RESTART_MIN_TIMER   			 15
  #define RECONFIG_ENABLE_TIME			  			 TIMER_ms_to_tic(30)	
	#define SYSTEM_AUTO_RESTART_TIMER_TO_TIC   TIMER_min_tic(DEAFULT_RESTART_MIN_TIMER)

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

typedef struct commands
{
	
	uint8_t cmd_buff[20];
	uint8_t cmd_size;
	
}COMMANDS;

typedef struct ul_states{
	
	uint8_t nbr_cmd;			//Cuantas peticiones se haran al medidor
	uint8_t cmd_index;		//Peticion actual
	uint8_t nbr_retries;	//Numero de reintento actual
	uint32_t cmd_delay;		//Delay
	
	bool waiting_ack;			//Se espera respuesta
	bool cmd_ok;					//Se prende cuando se haya recibido respuesta
	bool last_cmd;				//Se prende cuando se haya recibido la ultima respuesta
	
	COMMANDS command[10];
	
}UL_STATES;

extern UL_STATES states;

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

extern void SYSTEM_initialize(void);
extern void SYSTEM_io_reconfig_task(void);
extern void SYSTEM_restart_system(void);

extern void user_button_init(void);				//DEBUG_EC Debug Function
extern void send_test_data(void);			//DEBUG_EC Debug Function
extern void send_data_request(void);
extern void TEST_vExecute_inst_1(void);
extern void TEST_vExecute_inst_2(void);
extern void TEST_vExecute_inst_3(void);

extern void TEST_vSend_cmd(UL_STATES * _states);
extern void TEST_set_cmd_ok(void);
extern void TEST_send_task_success(void);

extern void SYS_config_timers(void); 
extern void SYS_pfail_counter(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
