/**
 ******************************************************************************
 * \file timer.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup TIMER Timer Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all exportable functions for Timer Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE GWH00-79. Enero 2020.
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
#ifndef  __TIMER_H
#define  __TIMER_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
  #include "stm32l0xx_hal.h"
	#include <stdbool.h>
  #include "util.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
#define TIMER_ms_tic						         0.01 // Tick period in seconds.
#define TIMER_ms_to_tic( ms_time )			( ms_time / (TIMER_ms_tic*1000) )
#define TIMER_sec_to_ms_tic( sec_time )	((sec_time * 1000)/TIMER_ms_tic*1000)z

#define TIMER_min_tic(min_time)						(uint32_t)min_time * 60// Tick period in minutes.
#define TIMER_min_to_min_tic(min_time)		(min_time * TIMER_min_tic)
#define TIMER_hr_to_min_tic(hr_time)			(hr_time * 60UL)
#define TIMER_day_to_min_tic(day_time)		(day_time * 60UL * 24UL)

#define HALF_SECOND				(uint16_t)TIMER_ms_to_tic(500)
#define ONE_SECOND				(uint16_t)TIMER_ms_to_tic(1000)
#define TWO_SECONDS				(uint16_t)TIMER_ms_to_tic(2000)
#define THREE_SECONDS			(uint16_t)TIMER_ms_to_tic(3000)
#define FOUR_SECONDS			(uint16_t)TIMER_ms_to_tic(4000)
#define FIVE_SECONDS			(uint16_t)TIMER_ms_to_tic(5000)
#define SIX_SECONDS				(uint16_t)TIMER_ms_to_tic(6000)
#define SEVEN_SECONDS			(uint16_t)TIMER_ms_to_tic(7000)
#define EIGTH_SECONDS			(uint16_t)TIMER_ms_to_tic(8000)
#define NINE_SECONDS			(uint16_t)TIMER_ms_to_tic(9000)
#define TEN_SECONDS				(uint16_t)TIMER_ms_to_tic(10000)

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */
typedef enum ms_timer_id_type
{
	UCP_RX_TIMER = 0,
	UCP_TX_TIMER,
	UCP_LOGON_TIMER,
	UCP_RESPONSE_TIMEOUT_TIMER,
	MFG_RX_TIMER,
	MFG_TX_TIMER,
	MFG_LOGON_TIMER,
	MFG_RESPONSE_TIMEOUT_TIMER,
  IO_MCU_CONFIG_TIMER,
	LED_BLINKING_TIMER,
	LED_BLINKING_ON_GREEN_TIMER,
	LED_BLINKING_ON_RED1_TIMER,
	LED_BLINKING_ON_BLUE_TIMER,
	LED_BLINKING_ON_RED2_TIMER,
	LED_BLINKING_OFF_GREEN_TIMER,
	LED_BLINKING_OFF_RED1_TIMER,
	LED_BLINKING_OFF_BLUE_TIMER,
	LED_BLINKING_OFF_RED2_TIMER,
	SEND_TEST_FRAME_TIMER,
	WAIT_RESPONSE_TIMER,
	FIRST_REPORT_TIMER,
	PERIODIC_REPORT_TO_SINAMED,
  NUMBER_OF_MS_TIMERS
}MS_TIMER_ID_TYPE;

typedef enum min_timer_id_type
{	
	PERIODIC_SYSTEM_COLD_START_TIMER_MM,
	PERIODIC_SYSTEM_COLD_START_TIMER_CCG,
	NUMBER_OF_MIN_TIMERS 
}MIN_TIMER_ID_TYPE;


/*===========================================================================*\
 * Miliseconds timer structure
\*===========================================================================*/
typedef struct ms_timer_type
{
	bool running;
	bool expired;
	uint16_t timer;
	void_fptr fptr;
}MS_TIMER_TYPE;

/*===========================================================================*\
 * Seconds timer structure
\*===========================================================================*/
typedef struct min_timer_type
{
	bool running;
	bool expired;
	uint32_t timer;
	void_fptr fptr;
}MIN_TIMER_TYPE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

extern uint8_t ms_timer_flag;
extern uint16_t TIMER_tic_ms_timer;
extern uint32_t TIMER_tic_sec_timer;
extern MS_TIMER_TYPE TIMER_ms_timers[NUMBER_OF_MS_TIMERS];
extern MIN_TIMER_TYPE TIMER_min_timers[NUMBER_OF_MIN_TIMERS];
/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void TIMER_io_config(void);
extern void TIMER_io_init(void);
extern void TIMER_initialize(void);

extern void TIMER_start_ms_timer (MS_TIMER_ID_TYPE timer_id, uint16_t period, void_fptr function_ptr);
extern bool TIMER_is_ms_timer_running(MS_TIMER_ID_TYPE timer_id);
extern bool TIMER_is_ms_timer_expired(MS_TIMER_ID_TYPE timer_id);
extern void TIMER_reset_ms_timer(MS_TIMER_ID_TYPE timer_id);
extern uint16_t TIMER_get_pending_ms_time(MS_TIMER_ID_TYPE timer_id);
extern void_fptr TIMER_get_timer_fptr(uint8_t timer_id);
extern uint16_t TIMER_get_tic_ms_time(void);
extern void TIMER_start_min_timer (MIN_TIMER_ID_TYPE timer_id, uint32_t period, void_fptr function_ptr);
extern bool TIMER_is_min_timer_running(MIN_TIMER_ID_TYPE timer_id);
extern bool TIMER_is_min_timer_expired(MIN_TIMER_ID_TYPE timer_id);
extern void TIMER_reset_min_timer(MIN_TIMER_ID_TYPE timer_id);
extern uint16_t TIMER_get_pending_min_time(MIN_TIMER_ID_TYPE timer_id);
extern uint16_t TIMER_get_tic_min_time(void);
extern void null_action (void);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif
