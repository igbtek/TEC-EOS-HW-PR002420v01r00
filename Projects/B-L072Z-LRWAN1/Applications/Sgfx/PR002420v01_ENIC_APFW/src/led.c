/**
 ******************************************************************************
 * \file led.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Softek Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup LED Library
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
GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t LED_PIN[LEDn] = {LED1_PIN, LED2_PIN,LED3_PIN, LED4_PIN};

DEVICE_TYPE device;
LED_STATE led;

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
 TimerEvent_t LED_blink_task;
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
 * \fn void LED_initialize(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_initialize(void)
{
	LED_io_initialize();
	LED_io_configure();
	
	/*Led Timer definition*/
	TimerInit( &LED_blink_task, LED_diagnosis_led_task );
  TimerSetValue( &LED_blink_task, (uint32_t)LED_RTOS_blink_on_time);
	//TimerStart( &LED_blink_task ); 
	
	led.turn_on = false;
	
	device.system_state = DEVICE_INITIALIZING;
	
}
 
/** 
 ******************************************************************************
 * \fn void LED_io_initialize(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_io_initialize(void)
{
	GPIO_InitTypeDef  GPIO_InitStructG;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE( LED_GREEN );

  /* Configure the GPIO_LED pin */
  GPIO_InitStructG.Pin = LED_PIN[LED_GREEN];
  GPIO_InitStructG.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructG.Pull = GPIO_NOPULL;
  GPIO_InitStructG.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(LED_PORT[LED_GREEN], &GPIO_InitStructG);
	
	GPIO_InitTypeDef  GPIO_InitStructB;
  
  /* Enable the GPIO_LED Clock */
	LEDx_GPIO_CLK_ENABLE( LED_BLUE );

  /* Configure the GPIO_LED pin */
  GPIO_InitStructB.Pin = LED_PIN[LED_BLUE];
  GPIO_InitStructB.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructB.Pull = GPIO_NOPULL;
  GPIO_InitStructB.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(LED_PORT[LED_BLUE], &GPIO_InitStructB);
	
	GPIO_InitTypeDef  GPIO_InitStructR;
  
  /* Enable the GPIO_LED Clock */
	LEDx_GPIO_CLK_ENABLE( LED_RED2 );

  /* Configure the GPIO_LED pin */
  GPIO_InitStructR.Pin = LED_PIN[LED_RED2];
  GPIO_InitStructR.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructR.Pull = GPIO_NOPULL;
  GPIO_InitStructR.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  
  HAL_GPIO_Init(LED_PORT[LED_RED2], &GPIO_InitStructR);
	
}

/** 
 ******************************************************************************
 * \fn void LED_io_configure(void)
 * \brief
 *
 * 
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_io_configure(void)
{
	/* RGB LED i/o configuration. */
	//ioport_set_pin_dir(LED_R_IO, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(LED_G_IO, IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(LED_B_IO, IOPORT_DIR_OUTPUT);
}

/**
 ******************************************************************************
 * \fn void LED_turn_on(uint8_t led_color)
 * \brief
 *
 * \param uint8_t led_color
 * \retval
 ******************************************************************************
 */
void LED_turn_on(Led_TypeDef led_color)
{
	LED_io_configure();
	LED_turn_off(LED_ALL);
	
	switch (led_color)
	{
		case LED_RED2:
				HAL_GPIO_WritePin(LED_PORT[LED_RED2], LED_PIN[LED_RED2], GPIO_PIN_SET);
		break;
		
		case LED_GREEN:
				HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_SET); 
		break;
			
		case LED_BLUE:
				HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_SET);
		break;
			
//		case LED_YELLOW:
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_SET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_SET); 
//		break;
//		
//		case LED_CYAN:
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_SET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_SET); 
//		break;
//			
//		case LED_MAGENTA:
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_SET);
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_SET);
//		break;
//		
//		case LED_WHITE:
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_SET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_SET); 
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_SET);
//		break;
			
		case LED_ALL:
			HAL_GPIO_WritePin(LED_PORT[LED_RED2], LED_PIN[LED_RED2], GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_SET); 
			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_SET);
		break;
		
		default:
		break;
	}
}

/**
 ******************************************************************************
 * \fn void LED_turn_off(uint8_t led_color)
 * \brief
 *
 * \param uint8_t led_color
 * \retval void
 ******************************************************************************
 */
void LED_turn_off(Led_TypeDef led_color)
{
	LED_io_configure();
	
	switch (led_color)
	{
		case LED_RED2:
				HAL_GPIO_WritePin(LED_PORT[LED_RED2], LED_PIN[LED_RED2], GPIO_PIN_RESET);
		break;
			break;
		
		case LED_GREEN:
			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_RESET);
			break;
		
		case LED_BLUE:
			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_RESET);
			break;
		
//		case LED_YELLOW:
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_RESET);
//			break;
//		
//		case LED_CYAN:
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_RESET);
//			break;
//		
//		case LED_MAGENTA:
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_RESET);;
//			break;
//		
//		case LED_WHITE:
//			HAL_GPIO_WritePin(LED_PORT[LED_RED], LED_PIN[LED_RED], GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_RESET);
//			break;
		
		case LED_ALL:
			HAL_GPIO_WritePin(LED_PORT[LED_RED2], LED_PIN[LED_RED2], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_PORT[LED_GREEN], LED_PIN[LED_GREEN], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_PORT[LED_BLUE], LED_PIN[LED_BLUE], GPIO_PIN_RESET);
			break;
		
		default:
			break;
	}
}

/**
 ******************************************************************************
 * \fn void LED_test_rgb(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_test_rgb(void)
{
	LED_turn_on(LED_GREEN);
	DelayMs(700);
	//WATCHDOG_APP_vRestart();
	//PFAIL_APP_vTask();
	
	LED_turn_on(LED_BLUE);
	DelayMs(700);
	//WATCHDOG_APP_vRestart();
	//PFAIL_APP_vTask();
	
	LED_turn_on(LED_RED2);
	DelayMs(700);	
	//WATCHDOG_APP_vRestart();
	//PFAIL_APP_vTask();
	
	LED_turn_off(LED_ALL);
	DelayMs(700);		
	//WATCHDOG_APP_vRestart();
	//PFAIL_APP_vTask();
	device.system_state = DEVICE_RUNNING;
}

/**
 ******************************************************************************
 * \fn void LED_diagnosis_led_task(void)
 * \brief LED Diagnosis Task
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_diagnosis_led_task(void)
{
	if(led.turn_on == true){
		LED_turn_off(LED_ALL);
		led.turn_on = false;
		TimerSetValue( &LED_blink_task, (uint32_t)LED_RTOS_blink_off_time);
	}
	else{
		
		if ( device.system_state == DEVICE_RUNNING ) {
			
			LED_turn_on(LED_GREEN);
		}
		else if (device.system_state == DEVICE_BUSY_HOST){
			
			LED_turn_on(LED_RED2);
		}
		else if (device.system_state == DEVICE_BUSY_SGFX){
			
			LED_turn_on(LED_BLUE);
		}
		else if (device.system_state == DEVICE_MFG_MODE){
			
			LED_turn_on(LED_ALL);
		}
		
		led.turn_on = true;
		TimerSetValue( &LED_blink_task, (uint32_t)LED_RTOS_blink_on_time);
	}
	TimerStart(&LED_blink_task);
}

/**
 ******************************************************************************
 * \fn void LED_turn_off_all_delay_diagnosis_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void LED_turn_off_all_delay_diagnosis_task(void)
{
	LED_turn_off(LED_ALL);
	TIMER_start_ms_timer(LED_BLINKING_TIMER, LED_OFF_PERIOD, LED_diagnosis_led_task);
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
