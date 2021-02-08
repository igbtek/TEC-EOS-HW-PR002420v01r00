/**
 ******************************************************************************
 * \file timer.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Softek Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup TIMER Timer Library
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
#include "stm32l0xx_hal_cortex.h"
#include "timer.h"
#include "util.h"
#include "stm32l0xx_hal_tim.h"

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
  uint8_t ms_timer_flag;
  uint16_t TIMER_tic_ms_timer;
  uint32_t TIMER_tic_sec_timer;

  MS_TIMER_TYPE TIMER_ms_timers[NUMBER_OF_MS_TIMERS];
  MIN_TIMER_TYPE TIMER_min_timers[NUMBER_OF_MIN_TIMERS];

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
 * \fn void TIMER_initialize(void) ISR( TCC0_OVF_vect )
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_initialize(void)
{
	TIMER_io_init();
	TIMER_io_config();
	
	ms_timer_flag = 0;
	
	for (uint8_t i=0; i < NUMBER_OF_MS_TIMERS; i++)
	{
		TIMER_ms_timers[i].running = false;
		TIMER_ms_timers[i].expired = false;
		TIMER_ms_timers[i].timer = 0;
		TIMER_ms_timers[i].fptr = null_action;
	}
	
	for (uint8_t i=0; i < NUMBER_OF_MIN_TIMERS; i++)
	{
		TIMER_min_timers[i].running = false;
		TIMER_min_timers[i].expired = false;
		TIMER_min_timers[i].timer = 0;
		TIMER_min_timers[i].fptr = null_action;
	}
} 

/**
 ******************************************************************************
 * \fn void TIMER_io_init(void)
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void TIMER_io_init(void)
{
  
}
/**
 ******************************************************************************
 * \fn void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr)
{
	if (NUMBER_OF_MS_TIMERS > timer_id)
	{
		TIMER_ms_timers[timer_id].running = true;
		TIMER_ms_timers[timer_id].expired = false;
		TIMER_ms_timers[timer_id].timer = TIMER_tic_ms_timer + period;
		TIMER_ms_timers[timer_id].fptr = function_ptr;
		
		if ( period == 0x0000 )
		{
			TIMER_ms_timers[timer_id].running = false;
			TIMER_ms_timers[timer_id].expired = true;
			(*TIMER_ms_timers[timer_id].fptr)();		//execute the desired call back function
		}
	}
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id)
{
	return TIMER_ms_timers[timer_id].running;
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */ 
bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id)
{
	return TIMER_ms_timers[timer_id].expired;
}

/**
 ******************************************************************************
 * \fn void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id)
{
	TIMER_ms_timers[timer_id].running = false;
	TIMER_ms_timers[timer_id].expired = false;
	TIMER_ms_timers[timer_id].timer = 0;
	TIMER_ms_timers[timer_id].fptr = null_action;
	
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id)
 * \brief returns the pending time of a timer
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id)
{
  uint16_t pending_time = 0;

  if (NUMBER_OF_MS_TIMERS > timer_id)
  {
      if ( TIMER_tic_ms_timer < TIMER_ms_timers[timer_id].timer)
      {
          pending_time = TIMER_ms_timers[timer_id].timer - TIMER_tic_ms_timer;
      }
  }
  return(pending_time);
}

/**
 ******************************************************************************
 * \fn void_fptr TIMER_get_timer_fptr(uint8_t timer_id)
 * \brief
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void_fptr TIMER_get_timer_fptr(uint8_t timer_id)
{
	void_fptr fptr = NULL;
	
	if (NUMBER_OF_MS_TIMERS > timer_id)
	{
		fptr = TIMER_ms_timers[timer_id].fptr;
	}
	return fptr;
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_tic_ms_time(void)
 * \brief Returns the current tick time
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_tic_ms_time(void)
{
	return TIMER_tic_ms_timer;
}

/**
 ******************************************************************************
 * \fn void TIMER_start_min_timer (MIN_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_start_min_timer (MIN_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr)
{  
	if (NUMBER_OF_MIN_TIMERS > timer_id)
	{	
		TIMER_min_timers[timer_id].running = true;
		TIMER_min_timers[timer_id].expired = false;
		TIMER_min_timers[timer_id].timer = TIMER_tic_sec_timer + (uint32_t)period*60;
		TIMER_min_timers[timer_id].fptr = function_ptr;
	  
		if ( period == 0x0000 )
		{
		TIMER_min_timers[timer_id].running = false;
		TIMER_min_timers[timer_id].expired = true;
		(*TIMER_min_timers[timer_id].fptr)();		//execute the desired call back function
		}	
  }
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_min_timer_running(MIN_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
bool TIMER_is_min_timer_running(MIN_TIMER_ID_TYPE timer_id)
{
	return TIMER_min_timers[timer_id].running;
}

/**
 ******************************************************************************
 * \fn bool TIMER_is_min_timer_expired(MIN_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
bool TIMER_is_min_timer_expired(MIN_TIMER_ID_TYPE timer_id)
{
	return TIMER_min_timers[timer_id].expired;
}

/**
 ******************************************************************************
 * \fn void TIMER_reset_min_timer(MIN_TIMER_ID_TYPE timer_id)
 * \brief 
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
void TIMER_reset_min_timer(MIN_TIMER_ID_TYPE timer_id)
{
	TIMER_min_timers[timer_id].running = false;
	TIMER_min_timers[timer_id].expired = false;
	TIMER_min_timers[timer_id].timer = 0;
	TIMER_min_timers[timer_id].fptr = null_action;
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_pending_min_time(MIN_TIMER_ID_TYPE timer_id)
 * \brief returns the pending time of a timer
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_pending_min_time(MIN_TIMER_ID_TYPE timer_id)
{
  uint16_t pending_time = 0;

  if (NUMBER_OF_MIN_TIMERS > timer_id)
  {
      if ( TIMER_tic_sec_timer < TIMER_min_timers[timer_id].timer)
      {
          pending_time = TIMER_min_timers[timer_id].timer - TIMER_tic_sec_timer;
      }
  }
  return(pending_time/60);
}

/**
 ******************************************************************************
 * \fn uint16_t TIMER_get_tic_min_time(void)
 * \brief Returns the current tick time
 *
 * \param
 * \retval
 *
 ******************************************************************************
 */
uint16_t TIMER_get_tic_min_time(void)
{
	return TIMER_tic_sec_timer/60;
}

/**
 ******************************************************************************
 * \fn void null_action (void)
 * \brief Null action routine
 *
 * \param	None
 * \retval	None
 *
 ******************************************************************************
 */
void null_action (void)
{
	__NOP();
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
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef base_timer;

/* TIM3 init function */
void TIMER_io_config(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  base_timer.Instance = TIM3;
  base_timer.Init.Prescaler = 32000+1;
  base_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  base_timer.Init.Period = 10-1;
  base_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&base_timer);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&base_timer, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&base_timer, &sMasterConfig);
  
  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_Base_Start_IT(&base_timer) != HAL_OK){}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
