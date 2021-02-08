/**
 ******************************************************************************
 * \file ucp.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup UART Communication Protocol Library
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
#ifndef  __UCP_H
#define  __UCP_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
 #include "timer.h"
 #include "util.h"
 #include "utilities_conf.h"
 #include "ucp_conf.h"
 #include "utilities_conf.h"
 #include "util.h"
 #include "stdbool.h"
 #include "stm32l072xx.h"
 #include "stm32l0xx_hal_uart.h"
 #include "stm32l0xx_hal_dma.h"
 
 /*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
 
 //#define USE_FULL_LL_DRIVER
 
#define CARRY_RETURN								0x0D	/* Carry Return */
 
#define UCP_INTER_CHARACTER_TIMEOUT					TIMER_ms_to_tic(510)
#define UCP_LOGON_TIMEOUT										(uint32_t)TIMER_ms_to_tic(90000)	// 1 min

#define UCP_RTOS_INTER_CHARACTER_TIMEOUT		(uint32_t)510

#define	RX_PACKET_SIZE  						30
#define	TX_PACKET_SIZE  						20

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
 
  #ifdef VCOM
#else
	extern UART_HandleTypeDef mm_uart;
#endif
 
 /*===========================================================================*\
 * UCP Communications state
\*===========================================================================*/
typedef struct ucp_dll_type{
	
	uint8_t rx_state;
	uint8_t rx_timer;
	uint8_t rx_error;
	uint8_t rx_buffer[RX_PACKET_SIZE];
	//uint8_t rx_buffer[30];						//DEBUG_EC FIX
	uint16_t rx_buffer_bytes;
	uint8_t rx_data_bytes;
	uint16_t rx_data_bytes_remaining;
	bool rx_eop;
	
	uint8_t *rx_data;
	uint16_t rx_data_length;
	
	void_fptr rx_abort;
	void_fptr io_configure;
	
	uint8_t tx_state;
	uint8_t tx_timer;
	uint8_t tx_error;
	bool tx_eop;	
	
	uint8_t *tx_data;
	uint16_t tx_data_length;
	
	uint8_t tx_buffer[TX_PACKET_SIZE];		//DEBUG_EC FIX
	//uint8_t tx_buffer[20];
	uint16_t tx_buffer_bytes;
	uint8_t tx_data_bytes;
	uint16_t tx_data_bytes_remaining;
	uint8_t tx_nbr_retries;
	bool wait_for_ack;

	void_fptr tx_abort;
	void_fptr tx_retry;
	uint8_t response_timeout_timer;
	
	void_fptr uart_fprt;
	TASKS_ENUM	uart_task;
	bool trigger_task;
	USART_TypeDef *uart;
	bool interrupt_context;
	bool ongoing_transmission;
	
}UCP_DLL_TYPE;
 
/*===========================================================================*\
 * GWH0079 Data link layer state
\*===========================================================================*/
typedef enum ucp_dll_state
{
  UCP_RX_IDLE = 0x00,
 	UCP_RX_STP,
	
 	UCP_TX_IDLE,
	UCP_TX_STP,
	UCP_TX_DATA,
} UCP_DLL_STATE;
 
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
 
extern void UCP_rx_abort(void);
extern void UCP_initialize(void);
extern void UCP_io_initialize(void);
extern void UCP_io_configure(void);
extern void UCP_tx_abort(void);
extern void UCP_tx_retry(void);
extern void UCP_echo_task(void);
extern void UCP_wait_response_task(void);
extern void UCP_send_frame(uint8_t* buffer, uint8_t size);
extern void UCP_tx_packet(UCP_DLL_TYPE *ucp);
extern void UCP_IRQHandler(void);
extern void UCP_rx_int_handler(UCP_DLL_TYPE * ucp);
extern void UCP_tx_int_handler(UCP_DLL_TYPE * ucp);

extern UCP_DLL_TYPE ucp_dll;


/*===========================================================================*\
 * GWH0079 Communication states
\*===========================================================================*/
typedef enum ucp_communication_state
{
	BASE_STATE		 = 0x00,
	ID_STATE,						
	SESSION_STATE,
	NBR_STATES				
} UCP_COMMUNICATION_STATE;
 
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
