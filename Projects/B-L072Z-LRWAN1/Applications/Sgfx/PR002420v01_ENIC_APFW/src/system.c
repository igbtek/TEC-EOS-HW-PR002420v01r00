/**
 ******************************************************************************
 * \file system.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Softek Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup Uart Communication Protocol Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code file functions for Timer Module using ATxmega core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - NAME
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Especificacion CFE GWH00-079. Enero 2020.
 *
 *   - Applicable Standards:
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: FORMAT_NAME
 *   - Revision: REVISION_NBR
 *   - Date: YYYY-MM-DD
 *   - Author: Ernesto Cocoletzi Flores.
 ******************************************************************************
 */
/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */

#include "system.h"
#include "eeprom.h"
#include "gwh0079.h"
#include "scheduler.h"
#include "debug.h"
#include "led.h"
 /*
 ******************************************************************************
 * File level pragmas                                                
 ******************************************************************************
 */ 

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
 
 /*===========================================================================*\
 * Brief description for each constant definition
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * Brief description for each Macro
\*===========================================================================*/

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
 */
 
UL_STATES states;

/*
 ******************************************************************************
 * Global and Const Variable Defining Definitions / Initializations
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 static bool McuInitialized = false;
 static uint8_t button_count = 0;
 static TimerEvent_t ReconfigTimer;
 /*
 ******************************************************************************
 * ROM Const Variables With File Level Scope
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Static Function Prototypes for Private Functions with File Level Scope
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */
 
/**
******************************************************************************
* \fn void SYSTEM_initialize(void)
* \brief Initialize the SYSTEM CLOCK
* 
* \param void
* \retval void
******************************************************************************
*/
void SYSTEM_initialize(void)
{  
	/* STM32 HAL library initialization*/
	HAL_Init( );
	
	/* Configure the system clock*/
	SystemClock_Config();
	
	/* Configure the debug mode*/
	DBG_Init( );
	
	/* Initialise HW modules*/
	HW_Init();
	
	/*Initialise Timer counters*/
	TIMER_initialize();
	
	/* Initialise internal Eeprom factory SEting at device Birth*/
	HW_EEPROM_Init();
	
	/* Initialise external EEPROM Library*/
	EEPROM_initialize();
	
	/*Initialise LED Ports*/
	LED_initialize();
	
	/*initialise user button*/
	user_button_init( );
	
	/*initialise Main Uart Communication Protocol Port*/
	UCP_initialize();
	
	/*initialise Manufacturer Uart Communication Protocol Port*/
	MFG_initialize();
	
	/*Test LED port*/
	LED_test_rgb();
	
	/*Start LED Blink Diagnosis*/
	LED_diagnosis_led_task();
	
	/*Initialize NIC Manager*/
	NIC_initialize();
	
	/*System io reconfig task timer*/
	TimerInit( &ReconfigTimer, SYSTEM_io_reconfig_task );
  TimerSetValue( &ReconfigTimer, 5000);
	TimerStart( &ReconfigTimer );
	
	/*Test Functions*/
	SCH_RegTask( SEND_TEST_TASK, send_test_data ); 								// Push Button
	SCH_RegTask( SET_RX_COMMAND_OK, TEST_set_cmd_ok ); 						// Flag to set Meter Comm rx ok 
	SCH_RegTask( SEND_TASK_SUCCESS, TEST_send_task_success);		  // Send Command OK in UCP Port
}

/**
******************************************************************************
* \fn void SYSTEM_io_reconfig_task(void)
* \brief void
* 
* \param void
* \retval void
******************************************************************************
*/
void SYSTEM_io_reconfig_task(void)
{
	/*If there's no communication in UCP Port*/
	if(ucp_dll.ongoing_transmission == false  )
	{
		UCP_io_configure();
	}
	/*If there's no communication in MFG Port*/
	if(mfg_dll.ongoing_transmission == false)
	{
		MFG_io_configure();
	}
	
	LED_io_configure();
	
	if (HW_GPIO_Read( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN ) == 0)
	{
		user_button_init();		//DEBUG_EC
	}
	if(device.system_state != DEVICE_BUSY_SGFX)
	{
		SX1276IoDeInit();
	}
	
	TimerStart( &ReconfigTimer );
}

/**
******************************************************************************
* \fn void user_button_init(void)
* \brief Initialize the SYSTEM CLOCK
* 
* \param void
* \retval void
******************************************************************************
*/
void user_button_init(void)
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode =GPIO_MODE_IT_RISING;
  initStruct.Pull = GPIO_PULLUP;
  initStruct.Speed = GPIO_SPEED_HIGH;
  HW_GPIO_Init( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, &initStruct );
  
  /* send everytime button is pushed */
  HW_GPIO_SetIrq( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, 1, send_data_request );
}

/**
******************************************************************************
* \fn void send_data_request(void)
* \brief void
* 
* \param void
* \retval void
******************************************************************************
*/
void send_data_request(void)
{
  /* send task to background*/
	
	//SCH_SetTask(REP_TIMER);					//DEBUG_KH
  SCH_SetTask( SEND_TEST_TASK );	//DEBUG_EC
}

/**
 ******************************************************************************
 * \fn void send_test_data( void )
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void send_test_data( void )
{
	if (button_count == 0x00){
		device.system_state = DEVICE_BUSY_HOST;
	}
	else if (button_count == 0x10){
		device.system_state = DEVICE_BUSY_SGFX;
	}
	else if (button_count == 0x20){
		device.system_state = DEVICE_MFG_MODE;
	}
	
	switch (button_count)
	{
		//case INST_1:
		case 0x00:
			TEST_vExecute_inst_1();
		break;
		
		//case INST_2:
		case 0x10:
			TEST_vExecute_inst_2();
		break;
			
		//case INST_3:
		case 0x20:
			TEST_vExecute_inst_3();
		break;
		
		default:
		break;
	}
	
	button_count += 0x10;
	
	if (button_count >= 0x30)
	{
		button_count = 0x00;
		//device.system_state = DEVICE_RUNNING;
	}
	
	HW_GPIO_SetIrq( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, 1, send_data_request );
}

/**
 ******************************************************************************
 * \fn void TEST_vExecute_inst_1( void )
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_vExecute_inst_1(void)
{
	uint8_t nbr_tries = 0;
	bool inst_ok = false;
	
	//Copy First Command
	memcpy(&states.command[0].cmd_buff[0], "MEDIDOR \r", 9);
	states.command[0].cmd_size = 9;
	
	states.waiting_ack = true;
	states.nbr_cmd = 1;
	states.cmd_index = 0;
	
	TEST_vSend_cmd(&states);
	//TIMER_start_ms_timer(WAIT_RESPONSE_TIMER, TIMER_ms_to_tic(5000), null_action );
}

/**
 ******************************************************************************
 * \fn void TEST_vExecute_inst_2( void )
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_vExecute_inst_2(void)
{
	uint8_t nbr_tries = 0;
	bool inst_ok = false;
	
	//Copy First Command
	memcpy(&states.command[0].cmd_buff[0], "ENERGIA\r", 8);
	states.command[0].cmd_size = 8;
	
	//Copy Second Command
	memcpy(&states.command[1].cmd_buff[0], "ENERGIA R\r", 10);
	states.command[1].cmd_size = 10;
	
	states.waiting_ack = true;
	states.nbr_cmd = 2;
	states.cmd_index = 0;
	//SCH_SetTask( UART_RX_WAIT_RESPONSE );
	
	TEST_vSend_cmd(&states);
	//TIMER_start_ms_timer(WAIT_RESPONSE_TIMER, TIMER_ms_to_tic(5000), null_action );
}

/**
 ******************************************************************************
 * \fn void TEST_vExecute_inst_3( void )
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_vExecute_inst_3(void)
{
	uint8_t nbr_tries = 0;
	bool inst_ok = false;
	
	//Copy First Command
	memcpy(&states.command[0].cmd_buff[0], "VOLTAJE A\r", 10);
	states.command[0].cmd_size = 10;
	
	//Copy Second Command
	memcpy(&states.command[1].cmd_buff[0], "CORRIENTE A\r", 12);
	states.command[1].cmd_size = 12;
	
	//Copy Second Command
	memcpy(&states.command[2].cmd_buff[0], "POTENCIA A\r", 11);
	states.command[2].cmd_size = 11;
	
	states.waiting_ack = true;
	states.nbr_cmd = 3;
	states.cmd_index = 0;
	//SCH_SetTask( UART_RX_WAIT_RESPONSE );
	
	TEST_vSend_cmd(&states);
	//TIMER_start_ms_timer(WAIT_RESPONSE_TIMER, TIMER_ms_to_tic(5000), null_action );
}

/**
 ******************************************************************************
 * \fn bool TEST_vSend_cmd(SEQ_CMD command)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_vSend_cmd(UL_STATES * _states)
{
	_states->cmd_ok = false;
	
	ucp_dll.trigger_task = true;
	ucp_dll.uart_task = SET_RX_COMMAND_OK;
	UCP_send_frame(_states->command[_states->cmd_index].cmd_buff, _states->command[_states->cmd_index].cmd_size);
}

/**
 ******************************************************************************
 * \fn void TEST_set_cmd_ok(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_set_cmd_ok(void)
{
	if(states.waiting_ack == true)
	{
		states.cmd_ok = true;
		states.waiting_ack = false;
		states.cmd_index ++;
		if(states.cmd_index < states.nbr_cmd)
		{
			states.waiting_ack = true;
			TEST_vSend_cmd(&states);
		}
		else
		{
			states.last_cmd = true;
			SCH_SetTask(SEND_TASK_SUCCESS);
		}
	}
}

/**
 ******************************************************************************
 * \fn void TEST_send_task_success(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void TEST_send_task_success(void)
{
	if(states.last_cmd == true)
	{
		uint8_t response_ok[20];
		memcpy (&response_ok[0], "Test_Inst_0x", 12);
		response_ok[12] = (button_count >> 4) | 0x30;
		response_ok[13] = (button_count & 0xF0) | 0x30;
		memcpy (&response_ok[14],"_ok\r",4);
		ucp_dll.trigger_task = false;
		ucp_dll.uart_task = NO_TASK;
		states.last_cmd = false;
		UCP_send_frame((&response_ok[0]), 18);
	}
}
/**
 ******************************************************************************
 * \fn void SYS_config_timers(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SYS_config_timers(void)
{
	/* Initialize first reporter*/
 REP_init_first_rep();						
	/* Initialize recurring reporter*/
 REP_init_recurring_rep();
	/* Set up for timer */
 CM_init_cmd_timeout_timer();
	/* Set up for timer to refresh nic apl data */
 NIC_initialize_timer_refresh_data();
}
/*
 ******************************************************************************
 * \fn void SYS_pfail_counter (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SYS_pfail_counter(void)	
{
	uint32_t counter =0;
	uint8_t temp_val =0;
	uint8_t read_bufer[3];
	/* Read eeprom */
	EEPROM_reader(EEPROM_SGFX_RESET_FLAG_ADDR,&temp_val, EEPROM_SGFX_RESET_FLAG_SIZE);
	/* Verify if it is empty the flag */
	if(temp_val!=FLG_RESET)
	{
		EEPROM_reader(EEPROM_SGFX_RESET_COUNTER_ADDR,&read_bufer[0], EEPROM_SGFX_RESET_COUNTER_SIZE);
		
		/* Passing an array to uint32_t */
		counter |= read_bufer[2]<< 16;
		counter |= read_bufer[1]<< 8;
		counter |= read_bufer[0];
		
		/* Increase the counter */
		counter++;
		
		/* Reset counter */
		if(counter==0x00FFFFFF)
		{
		/* Set to 0 the counter*/
		temp_val=0x00;
		memcpy(&read_bufer[0],&temp_val,EEPROM_SGFX_RESET_COUNTER_SIZE);
		EEPROM_writer(EEPROM_SGFX_RESET_COUNTER_ADDR,&read_bufer[0], EEPROM_SGFX_RESET_COUNTER_SIZE);
		}
		/* Passing 3 bytes of uint32_t uint32_t to array */
		read_bufer[2] = counter >> 16;
		read_bufer[1] = counter >> 8;
		read_bufer[0] = counter;
		
		/* Write the new count */
		EEPROM_writer(EEPROM_SGFX_RESET_COUNTER_ADDR,&read_bufer[0], EEPROM_SGFX_RESET_COUNTER_SIZE);
	}
	/* Clean reset flag */
	temp_val = 0;
	EEPROM_writer(EEPROM_SGFX_RESET_FLAG_ADDR,&temp_val, EEPROM_SGFX_RESET_FLAG_SIZE);
}
 /**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
