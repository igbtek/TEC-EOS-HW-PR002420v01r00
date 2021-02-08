/**
 ******************************************************************************
 * \file ucp.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologias EOS Confidential.
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
 
#include "ucp.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_dma.h"
#include "debug.h"
#include "low_power_manager.h"
#include "scheduler.h"
#include "command.h"
#include "timeServer.h"
#include "stm32l0xx_ll_usart.h"
#include "cm.h"
 
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
static TimerEvent_t InterCharTimer;
UCP_DLL_TYPE ucp_dll;
static uint8_t ucp_rx_data[50]; //DEBUG_EC_Fix_this_Size
static uint8_t ucp_tx_data[12]; //DEBUG_EC_Fix_this_Size
static uint32_t ucp_logon_timeout;

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
 
//#ifdef VCOM
//#else
	UART_HandleTypeDef mm_uart;
//#endif

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 TimerEvent_t UCP_disble_led_blue_blink_timer;
 TimerEvent_t UCP_rtos_rx_abort_task;
 TimerEvent_t UCP_rtos_tx_abort_task;
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
 * \fn void void UCP_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_initialize(void)
{
//	#ifdef VCOM
//		CMD_Init(); // added by KH
//	#else
	
	UCP_io_initialize();
	UCP_io_configure();
	
	ucp_dll.io_configure = (void_fptr)UCP_io_configure;
	
	//TIMER_reset_ms_timer(UCP_LOGON_TIMER);
	ucp_logon_timeout = UCP_LOGON_TIMEOUT;
	
	ucp_dll.interrupt_context = true;
	
	ucp_dll.rx_state = UCP_RX_IDLE;
	ucp_dll.rx_timer = UCP_RX_TIMER;
	ucp_dll.rx_abort = (void_fptr)UCP_rx_abort;
	ucp_dll.rx_error = 0;
	ucp_dll.rx_eop = false;
	
	ucp_dll.rx_data = (uint8_t*)ucp_rx_data;
	ucp_dll.rx_data_length = 0;
	ucp_dll.rx_data_bytes = 0;
	ucp_dll.rx_data_bytes_remaining = 0;
	memset(ucp_dll.rx_buffer, 0, (unsigned int)sizeof(ucp_dll.rx_buffer));
	ucp_dll.rx_buffer_bytes = 0;
	memset(ucp_rx_data, 0, (unsigned int)sizeof(ucp_rx_data));
	
	ucp_dll.tx_state = UCP_TX_IDLE;
	ucp_dll.tx_timer = UCP_TX_TIMER;
	ucp_dll.tx_abort = (void_fptr)UCP_tx_abort;
	ucp_dll.tx_retry = (void_fptr)UCP_tx_retry;
	ucp_dll.tx_nbr_retries = 0;
	ucp_dll.response_timeout_timer = UCP_RESPONSE_TIMEOUT_TIMER;
	ucp_dll.tx_error = 0;
	ucp_dll.tx_eop = false;
	
	ucp_dll.tx_data = (uint8_t*)ucp_tx_data;
	ucp_dll.tx_data_length = 0;
	ucp_dll.tx_data_bytes = 0;
	ucp_dll.tx_data_bytes_remaining = 0;
	memset(ucp_dll.tx_buffer, 0, (unsigned int)sizeof(ucp_dll.tx_buffer));
	ucp_dll.tx_buffer_bytes = 0;
	memset(&ucp_dll, 0, (unsigned int)sizeof(ucp_tx_data));
	
	ucp_dll.uart_fprt = null_action;
	ucp_dll.uart_task = NO_TASK;
	ucp_dll.trigger_task = false;
	ucp_dll.uart = USART1;								//USART1-> PA9 and PA10 pins
	ucp_dll.ongoing_transmission = false;
	ucp_dll.interrupt_context = false;
	
	TimerInit( &UCP_rtos_rx_abort_task, ucp_dll.rx_abort );
	TimerSetValue( &UCP_rtos_rx_abort_task, UCP_RTOS_INTER_CHARACTER_TIMEOUT);
	TimerInit( &UCP_rtos_tx_abort_task, ucp_dll.tx_abort );
	TimerSetValue( &UCP_rtos_tx_abort_task, UCP_RTOS_INTER_CHARACTER_TIMEOUT);
}

/**
 ******************************************************************************
 * \fn void UCP_io_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_io_initialize(void)
{

}

/**
 ******************************************************************************
 * \fn void UCP_io_configure(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_io_configure(void)
{	
		mm_uart.Instance = USART1;
		mm_uart.Init.BaudRate = 9600;
		mm_uart.Init.WordLength = UART_WORDLENGTH_8B;
		mm_uart.Init.StopBits = UART_STOPBITS_1;
		mm_uart.Init.Parity = UART_PARITY_NONE;
		mm_uart.Init.Mode = UART_MODE_TX_RX;
		mm_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		mm_uart.Init.OverSampling = UART_OVERSAMPLING_16;
		mm_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		mm_uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		mm_uart.AdvancedInit.OverrunDisable = true;
		if (HAL_UART_Init(&mm_uart) != HAL_OK)
		{
		 //_Error_Handler(__FILE__, __LINE__);
		}
		
		HAL_NVIC_SetPriority(USART1_IRQn, 0x01, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		
		/* Computation of UART mask to apply to RDR register */
		__HAL_UART_MASK_COMPUTATION(&mm_uart);  
		/* Enable the UART Data Register not empty Interrupt */
		__HAL_UART_ENABLE_IT(&mm_uart, UART_IT_RXNE);
}

/**
 ******************************************************************************
 * \fn void UCP_rx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_rx_abort(void)
{
	ucp_dll.interrupt_context = true;
	
	ucp_dll.ongoing_transmission = false;
	ucp_dll.uart = USART1;												//DEBUG_EC Delete
	//ucp_dll.rx_buffer_bytes = 0;
	ucp_dll.rx_state = UCP_RX_IDLE;
	ucp_dll.rx_timer = UCP_RX_TIMER;
	ucp_dll.rx_abort = UCP_rx_abort;
	ucp_dll.rx_data = (uint8_t*)ucp_rx_data;
	//TIMER_reset_ms_timer(ucp_dll.rx_timer);
	
	ucp_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void UCP_tx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_tx_abort(void)
{
	ucp_dll.interrupt_context = true;
	
	ucp_dll.ongoing_transmission = false;
	ucp_dll.tx_state = UCP_TX_IDLE;
	ucp_dll.tx_timer = UCP_TX_TIMER;
	ucp_dll.tx_abort = UCP_tx_abort;
	ucp_dll.tx_data = (uint8_t*)ucp_tx_data;
	//TIMER_reset_ms_timer(ucp_dll.tx_timer);
	
	ucp_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void UCP_tx_retry(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_tx_retry(void)
{
	if ( ucp_dll.tx_nbr_retries < 0)
	{
		//ucp_resend_packet(&opr_c1218_dll);
		UCP_tx_packet(&ucp_dll);
		ucp_dll.tx_nbr_retries++;
	}
	else
	{
		//OPR_reset_logon_session();
	}
}

/**
 ******************************************************************************
 * \fn void UCP_echo_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_echo_task(void)
{
	if((ucp_dll.rx_eop) && (!ucp_dll.rx_error) )
	{
		//uint8_t debug8[4];
		volatile uint32_t timestamp, date;
		bool send_ok = false;
		
		//send_ok =  m_serial_send(&ucp_dll.rx_buffer[0], ucp_dll.rx_buffer_bytes);
		
		//TimerInit(&UCP_disble_led_blue_blink_timer, BSP_LED_blink_stop_led_blue);
		//TimerSetValue(&UCP_disble_led_blue_blink_timer, 200);
		//TimerStart( &UCP_disble_led_blue_blink_timer ); 
		
		timestamp = (uint32_t)TimerGetCurrentTime;
		//date = (uint32_t)TimerGetCurrentDate;
		
		//debug8[0] = (uint8_t)(timestamp << 24);
		//debug8[1] = (uint8_t)(timestamp << 16);
		//debug8[2] = (uint8_t)(timestamp << 8);
		//debug8[3] = (uint8_t)(timestamp);
		
		//send_ok =  m_serial_send(&debug8[0], 4);
		UCP_send_frame(&ucp_dll.rx_buffer[0], ucp_dll.rx_buffer_bytes);
		
		ucp_dll.interrupt_context = true;
		ucp_dll.rx_eop = false;
		
		//UCP_rx_abort();
		//gwh0079_dll.rx_eop = false;
	}
}

/**
 ******************************************************************************
 * \fn void UCP_wait_response_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_wait_response_task(void)
{
	if((ucp_dll.rx_eop) && (!ucp_dll.rx_error) )
	{
		ucp_dll.interrupt_context = false;
		ucp_dll.rx_eop = false;
		//SCH_PauseTask(ucp_dll.uart_task);
	}
}

/**
 ******************************************************************************
 * \fn void UCP_send_frame(uint8_t* buffer, uint8_t size)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_send_frame(uint8_t* buffer, uint8_t size)
{
	bool tx_ok;
	memcpy(&ucp_dll.tx_buffer[0], buffer, size);
	ucp_dll.tx_data_bytes = size;
	UCP_tx_packet(&ucp_dll);
	tx_ok = ucp_dll.tx_eop;
}

/**
 ******************************************************************************
 * \fn void UCP_tx_packet(void)
 * \brief UCP Protocol Uart Rx Interrupt handler
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UCP_tx_packet(UCP_DLL_TYPE *ucp)
{
	if( ucp->tx_state == UCP_TX_IDLE )
	{
		ucp->interrupt_context = true; 
		ucp->ongoing_transmission = true;
		ucp->tx_eop = false;
		ucp->tx_buffer_bytes = 0;
		ucp->tx_data_bytes_remaining = (uint16_t)(ucp->tx_data_bytes);
		ucp->tx_state = UCP_TX_STP;
		LL_USART_EnableIT_TXE(USART1);
		ucp->interrupt_context = false;  
	}
}

/**
******************************************************************************
* \fn void UCP_IRQHandler(void)
* \brief UCP Protocol Uart Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
**/
void UCP_IRQHandler(void)
{
		null_action();
}

/**
******************************************************************************
* \fn void UCP_rx_int_handler(UCP_DLL_TYPE * ucp)
* \brief UCP Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
**/
void UCP_rx_int_handler(UCP_DLL_TYPE * ucp)
{
	volatile uint8_t data;
	
  ucp->interrupt_context = true;
	ucp->ongoing_transmission = true;
	
	TimerStop( &UCP_rtos_rx_abort_task);
  data = ucp->uart->RDR;
	
	switch( ucp->rx_state )
	{
		case UCP_RX_IDLE:
			ucp->rx_eop = false;
			ucp->rx_buffer_bytes = 0;
			ucp->rx_buffer[ucp->rx_buffer_bytes] = data;
			ucp->rx_buffer_bytes++;
			ucp->rx_data_length = 0;
			ucp->rx_error = 0;
			ucp->rx_state = UCP_RX_STP;
		break;
		
		case UCP_RX_STP:
			
			ucp->rx_buffer[ucp->rx_buffer_bytes] = data;
			ucp->rx_buffer_bytes++;
		
			if(data == CARRY_RETURN)
			{
				ucp->rx_eop = true;
				ucp->rx_error = false;
				ucp->ongoing_transmission = false;
				ucp->rx_state = UCP_RX_IDLE;
			}
			else if(ucp->rx_buffer_bytes > RX_PACKET_SIZE)		//DEBUG_EC Test Function
			{
				ucp->rx_eop = true;
				ucp->rx_error = false;
				ucp->ongoing_transmission = false;
				ucp->rx_state = UCP_RX_IDLE;
				ucp->rx_abort();
			}
		break;
		
		default:
			
			ucp->rx_state = UCP_RX_IDLE;
			ucp->ongoing_transmission = false;
			
		break;
	}
	
	if ( ucp->ongoing_transmission == true )
	{
		TimerStart( &UCP_rtos_rx_abort_task);
	}
	
	if(ucp->rx_eop == true)
	{
		//ucp->trigger_task = true;
		//CM_rx_data_analize();
		if(ucp->trigger_task == true)
		{
			/* Passing to structure the task to analize data received from meter */
			//ucp->uart_task = CM_DATA_RX;		//DEBUG_EC Kevin Revisa esta linea de codigo
			SCH_RegTask( (ucp->uart_task), ucp->uart_fprt);
			SCH_SetTask( (ucp->uart_task));
			ucp->rx_abort();
		}
	}
	
	ucp->interrupt_context = false;
}

/**
******************************************************************************
* \fn void UCP_tx_int_handler(UCP_DLL_TYPE * ucp)
* \brief GWH0079 Protocol Uart Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
*/
void UCP_tx_int_handler(UCP_DLL_TYPE * ucp)
{
  ucp->interrupt_context = true;
	ucp->ongoing_transmission = true;
	
	TimerStop( &UCP_rtos_tx_abort_task);
	
	switch( ucp->tx_state )
	{
		case UCP_TX_STP:
			
			ucp->tx_eop = false;
			ucp->uart->TDR = ucp->tx_buffer[ucp->tx_buffer_bytes];
			ucp->tx_buffer_bytes++;
			ucp->tx_data_bytes_remaining--;
			ucp->tx_state = UCP_TX_DATA;	
		break;
		
		case UCP_TX_DATA:
			ucp->uart->TDR = ucp->tx_buffer[ucp->tx_buffer_bytes];
			ucp->tx_buffer_bytes++;
			ucp->tx_data_bytes_remaining--;
			ucp->tx_state = UCP_TX_DATA;
				
			//if((ucp->uart->TDR == CARRY_RETURN) || (ucp->tx_data_bytes_remaining == 0))
			if(ucp->tx_data_bytes_remaining == 0)
			{
				ucp->tx_eop = true;
				ucp->ongoing_transmission = false;
				ucp->tx_state = UCP_TX_IDLE;
				ucp->uart->ISR = HAL_UART_STATE_READY; // UART disable TXRDY interrupt
				ucp->wait_for_ack = true;
				LL_USART_DisableIT_TXE(USART1);
				TimerStop(&UCP_rtos_tx_abort_task);
			}
		
		break;
		
		default:
			ucp->ongoing_transmission = false;
			ucp->tx_state = UCP_TX_IDLE;
			LL_USART_DisableIT_TXE(USART1);
		break;
		
		if ( ucp->ongoing_transmission == true )
		{
			TimerStart( &UCP_rtos_tx_abort_task);
		}
	}
}

/**
******************************************************************************
* \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
* --------------------------------------------------------------------------------------------
* - Revision 01 / Author / YYYY-MM-DD
*   - Change 1:  Description.
*   - Change 2:  Description.
******************************************************************************
**/
