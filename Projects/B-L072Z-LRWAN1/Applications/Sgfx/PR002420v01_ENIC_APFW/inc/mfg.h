/**
 ******************************************************************************
 * \file mfg.h
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

#ifndef  MFG_H
#define  MFG_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
 #include "ucp.h"
 #include "timer.h"
 #include "util.h"
 #include "utilities_conf.h"
 #include "ucp_conf.h"
 #include "utilities_conf.h"
 #include "util.h"
 #include <stdbool.h>
 #include <string.h>
 #include "stm32l072xx.h"
 #include "stm32l0xx_hal_uart.h"
 #include "stm32l0xx_hal_dma.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

#define MFG_INTER_CHARACTER_TIMEOUT					TIMER_ms_to_tic(510)
#define MFG_LOGON_TIMEOUT										(uint32_t)TIMER_ms_to_tic(90000)	// 1 min

#define MFG_RTOS_INTER_CHARACTER_TIMEOUT					(uint32_t)510

#define	MFG_RX_PACKET_SIZE  						160
#define	MFG_TX_PACKET_SIZE  						128

/*MFG UART Protocol*/
#define MFG_STP													0xA5								//START_PACKET
#define MFG_OK													0x00								//OK
#define MFG_ERROR												0x01								//ERROR

#define MFG_PROC_COMPLETED							0x00
#define MFG_PROC_ACCEPTED_NOT_COMPLETED	0x01
#define MFG_PROC_INVALID_PARAMETER			0x02
#define MFG_PROC_CONFLICT_WITH_SET_UP		0x03								

#define MFG_SNS													0x02								//SERVICE NOT SUPPORTED
#define MFG_IAR													0x05								//INAPROPIATED ACTION REQUEST

#define MFG_STP_INDEX										0x00
#define MFG_LEN_INDEX										0x01
#define MFG_DATA_INDEX									0x02
#define MFG_PROCEDURE_DATA_INDEX				0x00

#define MFG_HEADER_SIZE									0x03

#define MFG_RTOS_mfg_mode_time					60000

/*MFG Commands*/
#define READ_VS_WRITE_FLAG_bm			(uint16_t)0x8000	// 0x8000 = 0b1000000000000000
#define PROC_FLAG_bm							(uint16_t)0x4000	// 0x4000 = 0b0100000000000000
#define CMD_ID_bm									(uint16_t)0x0FFF	// 0x0FFF = 0b0000111111111111

#define READ_VS_WRITE_FLAG_bp			(uint8_t)15
#define PROC_FLAG_bp							(uint8_t)14
#define CMD_ID_bp									(uint8_t)0

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
extern UART_HandleTypeDef mfg_uart;

/*===========================================================================*\
 * MFG Communications state
\*===========================================================================*/
typedef struct mfg_dll_type{
	
	uint8_t rx_state;
	uint8_t rx_timer;
	uint8_t rx_error;
	uint8_t rx_buffer[MFG_RX_PACKET_SIZE];
	//uint8_t rx_buffer[30];						//DEBUG_EC FIX
	uint16_t rx_buffer_bytes;
	uint8_t rx_data_bytes;
	uint16_t rx_data_bytes_remaining;
	bool rx_eop;
	
	uint8_t *rx_data;
	uint16_t rx_data_length;
	
	void_fptr rx_abort;
	void_fptr io_configure;
	
	uint16_t cmd_id;
	bool proc_flag;
	bool read_vs_write_flag;
	uint8_t mfg_data_buffer[50];
	uint8_t parameter_size;
	uint8_t parameter_data[40];
	bool (*execute_parameter_reading)(uint8_t*);
	bool (*execute_parameter_writing)(uint8_t*);
	bool (*execure_procedure)(uint8_t*, uint8_t*, uint8_t*);
	
	uint8_t tx_state;
	uint8_t tx_timer;
	uint8_t tx_error;
	bool tx_eop;	
	
	uint8_t *tx_data;
	uint16_t tx_data_length;
	
	uint8_t tx_buffer[MFG_TX_PACKET_SIZE];		//DEBUG_EC FIX
	//uint8_t tx_buffer[20];
	uint16_t tx_buffer_bytes;
	uint8_t tx_data_bytes;
	uint16_t tx_data_bytes_remaining;
	uint8_t tx_nbr_retries;
	bool wait_for_ack;

	void_fptr tx_abort;
	void_fptr tx_retry;
	uint8_t response_timeout_timer;
	
	TASKS_ENUM	uart_task;
	bool trigger_task;
	USART_TypeDef *uart;
	bool interrupt_context;
	bool ongoing_transmission;
	
}MFG_DLL_TYPE;
/*===========================================================================*\
 * GWH0079 Data link layer state
\*===========================================================================*/
typedef enum mfg_dll_state
{
  MFG_RX_IDLE = 0x00,
 	MFG_RX_STP,
	MFG_RX_LENGTH_H,
	MFG_RX_DATA,
	MFG_RX_CS,
	
 	MFG_TX_IDLE,
	MFG_TX_STP,
	MFG_TX_DATA,
	
} MFG_DLL_STATE;
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
extern void MFG_initialize(void);
extern void MFG_io_initialize(void);
extern void MFG_io_configure(void);
extern void MFG_rx_task(void);
extern void MFG_rx_abort(void);
extern void MFG_tx_abort(void);
extern void MFG_tx_retry(void);
extern void MFG_echo_task(void);
extern void MFG_wait_response_task(void);
extern void MFG_send_frame(uint8_t* buffer, uint8_t size);
extern void MFG_tx_packet(MFG_DLL_TYPE *mfg);
extern void MFG_IRQHandler(void);
extern void MFG_rx_int_handler(MFG_DLL_TYPE * mfg);
extern void MFG_tx_int_handler(MFG_DLL_TYPE * mfg);

extern MFG_DLL_TYPE mfg_dll;

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
