/**
 ******************************************************************************
 * \file mfg.c
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
#include "ucp.h"
#include "mfg.h"
#include "scheduler.h"
#include "command.h"
#include "timeServer.h"
#include "stm32l0xx_ll_usart.h"
#include "eeprom.h"

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

MFG_DLL_TYPE mfg_dll;
static uint8_t mfg_rx_data[MFG_RX_PACKET_SIZE];
static uint8_t mfg_tx_data[MFG_TX_PACKET_SIZE];
static uint32_t mfg_logon_timeout;

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
UART_HandleTypeDef mfg_uart;
/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 TimerEvent_t MFG_rtos_rx_abort_task;
 TimerEvent_t MFG_rtos_tx_abort_task;
 TimerEvent_t MFG_rtos_rx_callback_task;
 TimerEvent_t MFG_rtos_exit_mgf_mode_task;
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
static void MFG_execute_read(void);
static void MFG_execute_write(void);
static void MFG_execute_procedure(void);
static void MFG_send_iar(void);
static void MFG_send_sns(void);
static void MFG_send_invalid_frame(void);

static void MFG_manufacturer_sn_read(void);
static void MFG_manufacturer_sn_write(void);
static void MFG_ed_model_read(void);
static void MFG_ed_model_write(void);
static void MFG_hw_version_number_read(void);
static void MFG_hw_version_number_write(void);
static void MFG_hw_revision_number_read(void);
static void MFG_hw_revision_number_write(void);
static void MFG_fw_version_number_read(void);
//static void MFG_fw_version_number_write(void);				//Not able to change FW Numbers
static void MFG_fw_revision_number_read(void);
//static void MFG_fw_revision_number_write(void);				//Not able to change FW Numbers
static void MFG_mfg_serial_number_read(void);
static void MFG_mfg_serial_number_write(void);

static void MFG_fw_part_number_read(void);
static void MFG_hw_part_number_read(void);
static void MFG_hw_part_number_write(void);
//static void MFG_ed_ideitity_read(void);								//Device don't have ED identity
//static void MFG_ed_ideitity_write(void);							//Device don't have ED identity
static void MFG_board_assembly_sn_read(void);
static void MFG_board_assembly_sn_write(void);
static void MFG_mfg_module_serial_number_read(void);
static void MFG_mfg_module_serial_number_write(void);
static void MFG_bootloader_part_number_read(void);
static void MFG_device_id_read(void);
static void MFG_device_pac_read(void);
static void MFG_sigfox_index_read(void);
static void MFG_sigfox_index_write(void);

/*Procedures*/
static void MFG_mfg_2_mode_procedure(void);

static bool MFG_mfg_sn_read(uint8_t *parameter_buffer);
static bool MFG_mfg_sn_write(uint8_t *parameter_buffer);
static bool MFG_ed_read(uint8_t *parameter_buffer);
static bool MFG_ed_write(uint8_t *parameter_buffer);
static bool MFG_hw_ver_read(uint8_t *parameter_buffer);
static bool MFG_hw_ver_write(uint8_t *parameter_buffer);
static bool MFG_hw_rev_read(uint8_t *parameter_buffer);
static bool MFG_hw_rev_write(uint8_t *parameter_buffer);
static bool MFG_fw_ver_nbr_read(uint8_t *parameter_buffer);
static bool MFG_fw_rev_nbr_read(uint8_t *parameter_buffer);
static bool MFG_mfg_serial_nbr_read(uint8_t *parameter_buffer);
static bool MFG_mfg_serial_nbr_write(uint8_t *parameter_buffer);
static bool MFG_fw_pn_read(uint8_t *parameter_buffer);
static bool MFG_hw_pn_read(uint8_t *parameter_buffer);
static bool MFG_hw_pn_write(uint8_t *parameter_buffer);
static bool MFG_board_assy_sn_read(uint8_t *parameter_buffer);
static bool MFG_board_assy_sn_write(uint8_t *parameter_buffer);
static bool MFG_mfg_module_sn_read(uint8_t *parameter_buffer);
static bool MFG_mfg_module_sn_write(uint8_t *parameter_buffer);
static bool MFG_bootloader_pn_read(uint8_t *parameter_buffer);
static bool MFG_dev_id_read(uint8_t *parameter_buffer);
static bool MFG_dev_pac_read(uint8_t *parameter_buffer);
static bool MFG_sgfx_index_read(uint8_t *parameter_buffer);
static bool MFG_sgfx_index_write(uint8_t *parameter_buffer);

static bool MFG_mfg_2_mode(uint8_t *parameter_buffer, uint8_t* proc_response, uint8_t* proc_response_size);

static void MFG_exit_mfg_mode(void);

/*
 ******************************************************************************
 * Function Definitions
 ******************************************************************************
 */
 
 /**
 ******************************************************************************
 * \fn void void MFG_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_initialize(void)
{
	MFG_io_initialize();
	MFG_io_configure();
	
	mfg_dll.io_configure = (void_fptr)MFG_io_configure;
	
	mfg_logon_timeout = MFG_LOGON_TIMEOUT;
	
	mfg_dll.interrupt_context = true;
	
	mfg_dll.rx_state = MFG_RX_IDLE;
	mfg_dll.rx_timer = MFG_RX_TIMER;
	mfg_dll.rx_abort = (void_fptr)MFG_rx_abort;
	mfg_dll.rx_error = 0;
	mfg_dll.rx_eop = false;
	
	mfg_dll.rx_data = (uint8_t*)mfg_rx_data;
	mfg_dll.rx_data_length = 0;
	mfg_dll.rx_data_bytes = 0;
	mfg_dll.rx_data_bytes_remaining = 0;
	memset(mfg_dll.rx_buffer, 0, (unsigned int)sizeof(mfg_dll.rx_buffer));
	mfg_dll.rx_buffer_bytes = 0;
	memset(mfg_rx_data, 0, (unsigned int)sizeof(mfg_rx_data));
	
	memset(mfg_dll.mfg_data_buffer, 0, 50); //DEBUG_EC_Fix_this_size
	
	mfg_dll.tx_state = MFG_TX_IDLE;
	mfg_dll.tx_timer = MFG_TX_TIMER;
	mfg_dll.tx_abort = (void_fptr)MFG_tx_abort;
	mfg_dll.tx_retry = (void_fptr)MFG_tx_retry;
	mfg_dll.tx_nbr_retries = 0;
	mfg_dll.response_timeout_timer = MFG_RESPONSE_TIMEOUT_TIMER;
	mfg_dll.tx_error = 0;
	mfg_dll.tx_eop = false;
	
	mfg_dll.tx_data = (uint8_t*)mfg_tx_data;
	mfg_dll.tx_data_length = 0;
	mfg_dll.tx_data_bytes = 0;
	mfg_dll.tx_data_bytes_remaining = 0;
	memset(mfg_dll.tx_buffer, 0, (unsigned int)sizeof(mfg_dll.tx_buffer));
	mfg_dll.tx_buffer_bytes = 0;
	memset(&mfg_dll, 0, (unsigned int)sizeof(mfg_tx_data));
	
	mfg_dll.uart_task = NO_TASK;
	mfg_dll.trigger_task = false;
	mfg_dll.uart = USART2;				//USART2
	mfg_dll.ongoing_transmission = false;
	mfg_dll.interrupt_context = false;
	
	TimerInit( &MFG_rtos_rx_abort_task, mfg_dll.rx_abort );
	TimerSetValue( &MFG_rtos_rx_abort_task, MFG_RTOS_INTER_CHARACTER_TIMEOUT);
	TimerInit( &MFG_rtos_tx_abort_task, mfg_dll.tx_abort );
	TimerSetValue( &MFG_rtos_tx_abort_task, MFG_RTOS_INTER_CHARACTER_TIMEOUT);
	
	TimerInit( &MFG_rtos_exit_mgf_mode_task, MFG_exit_mfg_mode);
	TimerSetValue(&MFG_rtos_exit_mgf_mode_task, MFG_RTOS_mfg_mode_time);
	SCH_RegTask( MFG_CALLBACK_TASK, MFG_rx_task );
	
	/* Manufacturer UART Test Task*/
	SCH_RegTask( MFG_TX_ECHO_TASK, MFG_echo_task );
}
 
/**
 ******************************************************************************
 * \fn void MFG_io_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_io_initialize(void)
{
	
}

/**
 ******************************************************************************
 * \fn void MFG_io_configure(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_io_configure(void)
{
	mfg_uart.Instance = USART2;
	mfg_uart.Init.BaudRate = 9600;
	mfg_uart.Init.WordLength = UART_WORDLENGTH_8B;
	mfg_uart.Init.StopBits = UART_STOPBITS_1;
	mfg_uart.Init.Parity = UART_PARITY_NONE;
	mfg_uart.Init.Mode = UART_MODE_TX_RX;
	mfg_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	mfg_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	mfg_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	mfg_uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	mfg_uart.AdvancedInit.OverrunDisable = true;
	if (HAL_UART_Init(&mfg_uart) != HAL_OK)
	{
	 //_Error_Handler(__FILE__, __LINE__);
	}
	
	HAL_NVIC_SetPriority(USART2_IRQn, 0x01, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	
	/* Computation of UART mask to apply to RDR register */
	__HAL_UART_MASK_COMPUTATION(&mfg_uart);  
	/* Enable the UART Data Register not empty Interrupt */
	__HAL_UART_ENABLE_IT(&mfg_uart, UART_IT_RXNE);
}

/**
 ******************************************************************************
 * \fn void MFG_rx_task(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_rx_task(void)
{
	volatile uint8_t test = 0;
	
	mfg_dll.cmd_id  = (uint16_t)(mfg_dll.rx_buffer[MFG_DATA_INDEX + 0] << 8 );			/*Get CMD ID Bitmask*/
	mfg_dll.cmd_id |= (uint16_t)(mfg_dll.rx_buffer[MFG_DATA_INDEX + 1] << 0 );			/*Get CMD ID Bitmask*/
	mfg_dll.read_vs_write_flag = (bool)(mfg_dll.cmd_id & READ_VS_WRITE_FLAG_bm);		//& PROC_FLAG_bm		/* if 0 READ else WRITE*/ 
	mfg_dll.proc_flag = (bool)(uint16_t)(mfg_dll.cmd_id & PROC_FLAG_bm);						/* if 1 Procedure*/
	
	mfg_dll.cmd_id = (mfg_dll.cmd_id & CMD_ID_bm);																	/*Extracts Command ID*/
	
	if(mfg_dll.rx_error == false)
	{
		if((mfg_dll.read_vs_write_flag == false) && (mfg_dll.proc_flag == false))
		{
			
			if(mfg_dll.rx_buffer[MFG_LEN_INDEX] == 0x03)
			{
				/*Read Commands*/
				switch(mfg_dll.cmd_id)
				{
					case EEPROM_MANUFACTURER_SN_ID:
						MFG_manufacturer_sn_read();
					break;
					
					case EEPROM_ED_MODEL_ID:
						MFG_ed_model_read();
					break;
					
					case EEPROM_HW_VERSION_NUMBER_ID:
						MFG_hw_version_number_read();
					break;
					
					case EEPROM_HW_REVISION_NUMBER_ID:
						MFG_hw_revision_number_read();
					break;
					
					case EEPROM_FW_VERSION_NUMBER_ID:
						MFG_fw_version_number_read();
					break;
					
					case EEPROM_FW_REVISION_NUMBER_ID:
						MFG_fw_revision_number_read();
					break;
		
					case EEPROM_MFG_SERIAL_NUMBER_ID:
						MFG_mfg_serial_number_read();
					break;
					
					case EEPROM_FW_PART_NUMBER_ID:
						MFG_fw_part_number_read();
					break;
					
					case EEPROM_HW_PART_NUMBER_ID:
						MFG_hw_part_number_read();
					break;
					
					//case EEPROM_ED_IDENTITY_ID:
					//
					//break;
					
					case EEPROM_BOARD_ASSEMBLY_SN_ID:
						MFG_board_assembly_sn_read();
					break;
					
					case EEPROM_MFG_MODULE_SERIAL_NUMBER_ID:
						MFG_mfg_module_serial_number_read();
					break;
					
					case EEPROM_BOOTLOADER_PART_NUMBER_ID:
						MFG_bootloader_part_number_read();
					break;
					
					case EEPROM_SGFX_DEVICE_ID_ID:
						MFG_device_id_read();
					break;
					
					case EEPROM_SGFX_DEVICE_PAC_ID:
						MFG_device_pac_read();
					break;
					
					case EEPROM_SGFX_INDEX_ID:
						MFG_sigfox_index_read();
					break;
					
					default:
						MFG_send_iar();
					break;
				}
			}
			else
			{
				MFG_send_sns();
			}
		}
		else if(mfg_dll.read_vs_write_flag == true)
		{
			/*Write/Procedure Commands*/
			if(mfg_dll.proc_flag == false)
			{
				/*Write Commands*/
				switch(mfg_dll.cmd_id)
				{
					case EEPROM_MANUFACTURER_SN_ID:
						MFG_manufacturer_sn_write();
					break;
					
					case EEPROM_ED_MODEL_ID:
						MFG_ed_model_write();
					break;
					
					case EEPROM_HW_VERSION_NUMBER_ID:
						MFG_hw_version_number_write();
					break;
					
					case EEPROM_HW_REVISION_NUMBER_ID:
						MFG_hw_revision_number_write();
					break;
					
					case EEPROM_MFG_SERIAL_NUMBER_ID:
						MFG_mfg_serial_number_write();
					break;
					
					case EEPROM_HW_PART_NUMBER_ID:
						MFG_hw_part_number_write();
					break;
					
					case EEPROM_BOARD_ASSEMBLY_SN_ID:
						MFG_board_assembly_sn_write();
					break;
					
					case EEPROM_MFG_MODULE_SERIAL_NUMBER_ID:
						MFG_mfg_module_serial_number_write();
					break;
					
					case EEPROM_SGFX_INDEX_ID:
						MFG_sigfox_index_write();
					break;
					
					default:
						MFG_send_iar();
					break;
				}
			}
			else
			{
				/*Procedure Commands*/
				switch(mfg_dll.cmd_id)
				{
					/*TEST 1*/
					case EEPROM_MFG_2_MODE_ID:
						MFG_mfg_2_mode_procedure();
					break;
					/*TEST 2*/
					//case EEPROM_MFG_MODE_DISABLE_ID:
					//	MFG_mfg_mode_disable();
					break;
					/*TEST 3*/
					case 8:
						break;
					
					default:
						MFG_send_iar();
					break;
				}
			}
		}
	}
	else
	{
		MFG_send_invalid_frame();
	}
}

/**
 ******************************************************************************
 * \fn bool MFG_execute_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_execute_read(void)
{
	bool is_ok = false;
	uint8_t k = 0;
	
	mfg_dll.tx_data[k++] = MFG_STP;
	mfg_dll.tx_data[k++] = mfg_dll.parameter_size + MFG_HEADER_SIZE - 1;
	mfg_dll.tx_data[k++] = MFG_OK;
	
	is_ok = mfg_dll.execute_parameter_reading(&mfg_dll.parameter_data[0]);
	memcpy(&mfg_dll.tx_data[k++],&mfg_dll.parameter_data[0],mfg_dll.parameter_size);
	k+= mfg_dll.parameter_size - 1;
	mfg_dll.tx_data[k] = CRC_2s_complement(&mfg_dll.tx_data[0], k);
	k++;
	MFG_send_frame(mfg_dll.tx_data, k);
}

/**
 ******************************************************************************
 * \fn bool MFG_execute_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_execute_write(void)
{
	bool is_ok = false;
	uint8_t k = 0;
	
	mfg_dll.tx_data[k++] = MFG_STP;
	mfg_dll.tx_data[k++] = 0x02;
	
	if(mfg_dll.rx_buffer[MFG_LEN_INDEX] == (mfg_dll.parameter_size + MFG_HEADER_SIZE) )
	{
		memcpy(&mfg_dll.parameter_data[0], &mfg_dll.rx_buffer[MFG_DATA_INDEX + 2], mfg_dll.parameter_size);
		mfg_dll.tx_data[k++] = MFG_OK;
		is_ok = mfg_dll.execute_parameter_writing(&mfg_dll.parameter_data[0]);
	}
	else
	{
		mfg_dll.tx_data[k++] = MFG_ERROR;
	}
	mfg_dll.tx_data[k] = CRC_2s_complement(&mfg_dll.tx_data[0], k);
	k++;
	MFG_send_frame(mfg_dll.tx_data, k);
}

/**
 ******************************************************************************
 * \fn bool MFG_execute_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_execute_procedure(void)
{
	bool is_ok = false;
	uint8_t k = 0;
	uint8_t proc_response[40];
	uint8_t proc_response_size = 0;
	
	memset(&proc_response[0], 0x00, (unsigned int) sizeof (proc_response));
	
	mfg_dll.tx_data[k++] = MFG_STP;
	
	if(mfg_dll.rx_buffer[MFG_LEN_INDEX] == (mfg_dll.parameter_size + MFG_HEADER_SIZE) )
	{
		memcpy(&mfg_dll.parameter_data[0], &mfg_dll.rx_buffer[MFG_DATA_INDEX + 2], mfg_dll.parameter_size);
		//mfg_dll.tx_data[k++] = MFG_OK;
		if (mfg_dll.execure_procedure(&mfg_dll.parameter_data[0], &proc_response[0], &proc_response_size) == true)
		{
			mfg_dll.tx_data[k++] = MFG_HEADER_SIZE - 1 + proc_response_size;
			memcpy(&mfg_dll.tx_data[k], &proc_response[0],proc_response_size);
			k += proc_response_size;
			is_ok = true;
		}
		else
		{
			mfg_dll.tx_data[k++] = MFG_HEADER_SIZE - 1 + proc_response_size;
			mfg_dll.tx_data[k++] = MFG_ERROR;
			
		}
	}
	else
	{
		mfg_dll.tx_data[k++] = MFG_HEADER_SIZE - 1 + proc_response_size;
		mfg_dll.tx_data[k++] = MFG_ERROR;
	}
	mfg_dll.tx_data[k] = CRC_2s_complement(&mfg_dll.tx_data[0], k);
	k++;
	MFG_send_frame(mfg_dll.tx_data, k);
}

/**
 ******************************************************************************
 * \fn bool MFG_send_iar(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_send_iar(void)
{
	uint8_t k = 0;
	
	mfg_dll.tx_data[k++] = MFG_STP;
	mfg_dll.tx_data[k++] = 0x02;
	mfg_dll.tx_data[k++] = MFG_IAR;
	
	mfg_dll.tx_data[k] = CRC_2s_complement(&mfg_dll.tx_data[0], k);
	k++;
	MFG_send_frame(mfg_dll.tx_data, k);
}

/**
 ******************************************************************************
 * \fn bool MFG_send_sns(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_send_sns(void)
{
	uint8_t k = 0;
	
	mfg_dll.tx_data[k++] = MFG_STP;
	mfg_dll.tx_data[k++] = 0x02;
	mfg_dll.tx_data[k++] = MFG_SNS;
	
	mfg_dll.tx_data[k] = CRC_2s_complement(&mfg_dll.tx_data[0], k);
	k++;
	MFG_send_frame(mfg_dll.tx_data, k);
}

/**
 ******************************************************************************
 * \fn bool MFG_send_iar(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_send_invalid_frame(void)
{
		mfg_dll.uart->TDR = 0x15;
}

/**
 ******************************************************************************
 * \fn void MFG_manufacturer_sn_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_manufacturer_sn_read(void)
{
	mfg_dll.parameter_size = EEPROM_MANUFACTURER_SN_SIZE;
	mfg_dll.execute_parameter_reading = MFG_mfg_sn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_manufacturer_sn_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_manufacturer_sn_write(void)
{
	mfg_dll.parameter_size = EEPROM_MANUFACTURER_SN_SIZE;
	mfg_dll.execute_parameter_writing = MFG_mfg_sn_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_ed_model_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_ed_model_read(void)
{
	mfg_dll.parameter_size = EEPROM_ED_MODEL_SIZE;
	mfg_dll.execute_parameter_reading = MFG_ed_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_ed_model_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_ed_model_write(void)
{
	mfg_dll.parameter_size = EEPROM_ED_MODEL_SIZE;
	mfg_dll.execute_parameter_writing = MFG_ed_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_manufacturer_sn_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_version_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_HW_VERSION_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_hw_ver_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_version_number_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_version_number_write(void)
{
	mfg_dll.parameter_size = EEPROM_HW_VERSION_NUMBER_SIZE;
	mfg_dll.execute_parameter_writing = MFG_hw_ver_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_revision_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_revision_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_HW_REVISION_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_hw_rev_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_revision_number_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_revision_number_write(void)
{
	mfg_dll.parameter_size = EEPROM_HW_REVISION_NUMBER_SIZE;
	mfg_dll.execute_parameter_writing = MFG_hw_rev_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_fw_version_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_fw_version_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_FW_VERSION_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_fw_ver_nbr_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_fw_revision_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_fw_revision_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_FW_REVISION_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_fw_rev_nbr_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_mfg_serial_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_mfg_serial_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_SERIAL_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_mfg_serial_nbr_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_mfg_serial_number_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_mfg_serial_number_write(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_SERIAL_NUMBER_SIZE;
	mfg_dll.execute_parameter_writing = MFG_mfg_serial_nbr_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_fw_part_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_fw_part_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_FW_PART_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_fw_pn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_part_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_part_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_HW_PART_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_hw_pn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_hw_part_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_hw_part_number_write(void)
{
	mfg_dll.parameter_size = EEPROM_HW_PART_NUMBER_SIZE;
	mfg_dll.execute_parameter_writing = MFG_hw_pn_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_board_assembly_sn_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_board_assembly_sn_read(void)
{
	mfg_dll.parameter_size = EEPROM_BOARD_ASSEMBLY_SN_SIZE;
	mfg_dll.execute_parameter_reading = MFG_board_assy_sn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_board_assembly_sn_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_board_assembly_sn_write(void)
{
	mfg_dll.parameter_size = EEPROM_BOARD_ASSEMBLY_SN_SIZE;
	mfg_dll.execute_parameter_writing = MFG_board_assy_sn_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_mfg_module_serial_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_mfg_module_serial_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_mfg_module_sn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_mfg_module_serial_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_mfg_module_serial_number_write(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE;
	mfg_dll.execute_parameter_writing = MFG_mfg_module_sn_write;
	MFG_execute_write();
}

/**
 ******************************************************************************
 * \fn void MFG_mfg_module_serial_number_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_bootloader_part_number_read(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE;
	mfg_dll.execute_parameter_reading = MFG_bootloader_pn_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_device_pac_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_device_id_read(void)
{
	mfg_dll.parameter_size = EEPROM_SGFX_DEVICE_ID_SIZE;
	mfg_dll.execute_parameter_reading = MFG_dev_id_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_device_pac_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_device_pac_read(void)
{
	mfg_dll.parameter_size = EEPROM_SGFX_DEVICE_PAC_SIZE;
	mfg_dll.execute_parameter_reading = MFG_dev_pac_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_sigfox_index_read(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_sigfox_index_read(void)
{
	mfg_dll.parameter_size = EEPROM_SGFX_INDEX_SIZE;
	mfg_dll.execute_parameter_reading = MFG_sgfx_index_read;
	MFG_execute_read();
}

/**
 ******************************************************************************
 * \fn void MFG_sigfox_index_write(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_sigfox_index_write(void)
{
	mfg_dll.parameter_size = EEPROM_SGFX_INDEX_SIZE;
	mfg_dll.execute_parameter_writing = MFG_sgfx_index_write;
	MFG_execute_write();
}

/* Procedures */
/**
 ******************************************************************************
 * \fn void MFG_mfg_2_mode_procedure(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_mfg_2_mode_procedure(void)
{
	mfg_dll.parameter_size = EEPROM_MFG_2_MODE_PARAMETERS_SIZE;
	mfg_dll.execure_procedure = MFG_mfg_2_mode;
	MFG_execute_procedure();
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_sn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_MANUFACTURER_SN_ADDR, parameter_buffer, EEPROM_MANUFACTURER_SN_SIZE );
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_sn_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_sn_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_MANUFACTURER_SN_ADDR, parameter_buffer, EEPROM_MANUFACTURER_SN_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_ed_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_ed_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_ED_MODEL_ADDR, parameter_buffer, EEPROM_ED_MODEL_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_ed_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_ed_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_ED_MODEL_ADDR, parameter_buffer, EEPROM_ED_MODEL_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_hw_ver_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_ver_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_HW_VERSION_NUMBER_ADDR, parameter_buffer, EEPROM_HW_VERSION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_ver_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_HW_VERSION_NUMBER_ADDR, parameter_buffer, EEPROM_HW_VERSION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_hw_rev_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_rev_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_HW_REVISION_NUMBER_ADDR, parameter_buffer, EEPROM_HW_REVISION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_hw_rev_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_rev_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_HW_REVISION_NUMBER_ADDR, parameter_buffer, EEPROM_HW_REVISION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_fw_ver_nbr_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_fw_ver_nbr_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_FW_VERSION_NUMBER_ADDR, parameter_buffer, EEPROM_FW_VERSION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_fw_rev_nbr_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_fw_rev_nbr_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_FW_REVISION_NUMBER_ADDR, parameter_buffer, EEPROM_FW_REVISION_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_serial_nbr_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_serial_nbr_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_MFG_SERIAL_NUMBER_ADDR, parameter_buffer, EEPROM_MFG_SERIAL_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_serial_nbr_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_serial_nbr_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_MFG_SERIAL_NUMBER_ADDR, parameter_buffer, EEPROM_MFG_SERIAL_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_fw_pn_read (uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_fw_pn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_FW_PART_NUMBER_ADDR, parameter_buffer, EEPROM_FW_PART_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_hw_pn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_pn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_HW_PART_NUMBER_ADDR, parameter_buffer, EEPROM_HW_PART_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_hw_pn_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_hw_pn_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_HW_PART_NUMBER_ADDR, parameter_buffer, EEPROM_HW_PART_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_board_assy_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_board_assy_sn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_BOARD_ASSEMBLY_SN_ADDR, parameter_buffer, EEPROM_BOARD_ASSEMBLY_SN_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_board_assy_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_board_assy_sn_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_BOARD_ASSEMBLY_SN_ADDR, parameter_buffer, EEPROM_BOARD_ASSEMBLY_SN_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_module_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_module_sn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_MFG_MODULE_SERIAL_NUMBER_ADDR, parameter_buffer, EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_module_sn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_module_sn_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_MFG_MODULE_SERIAL_NUMBER_ADDR, parameter_buffer, EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_bootloader_pn_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_bootloader_pn_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_BOOTLOADER_PART_NUMBER_ADDR, parameter_buffer, EEPROM_BOOTLOADER_PART_NUMBER_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_dev_pac_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_dev_pac_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_SGFX_DEVICE_PAC_ADDR, parameter_buffer, EEPROM_SGFX_DEVICE_PAC_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_dev_id_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_dev_id_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_SGFX_DEVICE_ID_ADDR, parameter_buffer, EEPROM_SGFX_DEVICE_ID_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_sgfx_index_read(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_sgfx_index_read(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_reader(EEPROM_SGFX_INDEX_ADDR, parameter_buffer, EEPROM_SGFX_INDEX_SIZE );
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_sgfx_index_write(uint8_t *parameter_buffer)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_sgfx_index_write(uint8_t *parameter_buffer)
{
	bool is_ok = false;
	
	EEPROM_writer(EEPROM_SGFX_INDEX_ADDR, parameter_buffer, EEPROM_SGFX_INDEX_SIZE );
	
	/* Refresh New Index to nic_apl*/
	EEPROM_reader(EEPROM_SGFX_INDEX_ADDR, &nic_apl.index, EEPROM_SGFX_INDEX_SIZE);
	
	is_ok = true;
	
	return is_ok;
}

/**
 ******************************************************************************
 * \fn bool MFG_mfg_2_mode(uint8_t *parameter_buffer, uint8_t* proc_response, uint8_t* proc_response_size)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool MFG_mfg_2_mode(uint8_t *parameter_buffer, uint8_t* proc_response, uint8_t* proc_response_size)
{
	uint8_t k = 0;
	bool is_ok = false;
	proc_response[k++] = MFG_OK;
	
	if ((device.system_state == DEVICE_RUNNING) || (device.system_state == DEVICE_MFG_MODE) )
	{
		if(*parameter_buffer == 0x01)					//MFG_ENABLE
		{
			device.system_state = DEVICE_MFG_MODE;
			TimerStart(&MFG_rtos_exit_mgf_mode_task);
			is_ok = true;
		}
		else if (*parameter_buffer == 0x00)		//MFG_DISABLE
		{
			device.system_state = DEVICE_RUNNING;
			is_ok = true;
		}
		else
		{
			proc_response[k++] = MFG_PROC_INVALID_PARAMETER;
		}
		if (is_ok == true)
		{
			proc_response[k++] = MFG_PROC_COMPLETED;
			proc_response[k++] = EEPROM_MFG_2_MODE_ID;
			proc_response[k++] = 0xC0;
			proc_response[k++] = device.system_state;
		}
	}
	else
	{
		proc_response[k++] = MFG_PROC_CONFLICT_WITH_SET_UP;
	}
	
	*proc_response_size = k;
	
	is_ok = true;
	return is_ok;
}

/**
 ******************************************************************************
 * \fn void MFG_exit_mfg_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_exit_mfg_mode(void)
{
	device.system_state = DEVICE_RUNNING;
}

/**
 ******************************************************************************
 * \fn void MFG_rx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_rx_abort(void)
{
	mfg_dll.interrupt_context = true;
	
	mfg_dll.ongoing_transmission = false;
	mfg_dll.uart = USART2;
	mfg_dll.rx_buffer_bytes = 0;
	mfg_dll.rx_state = MFG_RX_IDLE;
	mfg_dll.rx_timer = MFG_RX_TIMER;
	mfg_dll.rx_abort = MFG_rx_abort;
	mfg_dll.rx_data = (uint8_t*)mfg_rx_data;
	
	mfg_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void MFG_tx_abort(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_tx_abort(void)
{
	mfg_dll.interrupt_context = true;
	
	mfg_dll.ongoing_transmission = false;
	mfg_dll.tx_state = MFG_TX_IDLE;
	mfg_dll.tx_timer = MFG_TX_TIMER;
	mfg_dll.tx_abort = MFG_tx_abort;
	mfg_dll.tx_data = (uint8_t*)mfg_tx_data;
	TIMER_reset_ms_timer(mfg_dll.tx_timer);
	
	mfg_dll.interrupt_context = false;
}

/**
 ******************************************************************************
 * \fn void MFG_tx_retry(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_tx_retry(void)
{
	if ( mfg_dll.tx_nbr_retries < 0)
	{
		mfg_dll.tx_nbr_retries++;
	}
	else
	{
	}
}

/**
 ******************************************************************************
 * \fn void MFG_echo_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_echo_task(void)
{
	if((mfg_dll.rx_eop) && (!mfg_dll.rx_error) )
	{
		MFG_send_frame(&mfg_dll.rx_buffer[0], mfg_dll.rx_buffer_bytes);
		mfg_dll.interrupt_context = true;
		mfg_dll.rx_eop = false;
	}
}

/**
 ******************************************************************************
 * \fn void MFG_wait_response_task(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_wait_response_task(void)
{
	if((mfg_dll.rx_eop) && (!mfg_dll.rx_error) )
	{
		mfg_dll.interrupt_context = false;
		mfg_dll.rx_eop = false;
		//SCH_PauseTask(mfg_dll.uart_task);
	}
}

/**
 ******************************************************************************
 * \fn void MFG_send_frame(uint8_t* buffer, uint8_t size)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_send_frame(uint8_t* buffer, uint8_t size)
{
	bool tx_ok;
	memcpy(&mfg_dll.tx_buffer[0], buffer, size);
	mfg_dll.tx_data_bytes = size;
	MFG_tx_packet(&mfg_dll);
	tx_ok = mfg_dll.tx_eop;
}

/**
 ******************************************************************************
 * \fn void MFG_tx_packet(void)
 * \brief MFG Uart Rx Interrupt handler
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void MFG_tx_packet(MFG_DLL_TYPE *mfg)
{
	if( mfg->tx_state == MFG_TX_IDLE )
	{
		mfg->interrupt_context = true; 
		mfg->ongoing_transmission = true;
		mfg->tx_eop = false;
		mfg->tx_buffer_bytes = 0;
		mfg->tx_data_bytes_remaining = (uint16_t)(mfg->tx_data_bytes);
		mfg->tx_state = MFG_TX_STP;
		LL_USART_EnableIT_TXE(USART2);
		mfg->interrupt_context = false;  
		
	}
}

/**
******************************************************************************
* \fn void MFG_IRQHandler(void)
* \brief MFG Uart Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
**/
void MFG_IRQHandler(void)
{
	
}

/**
******************************************************************************
* \fn void void MFG_rx_int_handler(MFG_DLL_TYPE * mfg)
* \brief MFG UART Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
**/
void MFG_rx_int_handler(MFG_DLL_TYPE * mfg)
{
	volatile uint8_t data;
	
  mfg->interrupt_context = true;
	mfg->ongoing_transmission = true;
	
	TimerStop( &MFG_rtos_rx_abort_task);
  data = mfg->uart->RDR;
	
	switch( mfg->rx_state )
	{	
		case MFG_RX_IDLE:
			if(data == MFG_STP)
			{
				mfg->rx_eop = false;
				mfg->rx_buffer_bytes = 0;
				mfg->rx_buffer[mfg->rx_buffer_bytes] = data;
				mfg->rx_buffer_bytes++;
				mfg->rx_data_length = 0;
				mfg->rx_error = false;
				mfg->rx_state = MFG_RX_STP;
			}
			else
			{
				mfg->rx_abort();
			}
			
		break;
		
		case MFG_RX_STP:
			mfg->rx_buffer[mfg->rx_buffer_bytes] = data;
			mfg->rx_buffer_bytes++;
			mfg->rx_data_bytes_remaining = data;
			mfg->rx_state = MFG_RX_DATA;
		break;
		
		case MFG_RX_DATA:
			mfg->rx_buffer[mfg->rx_buffer_bytes] = data;
			mfg->rx_buffer_bytes++;
			mfg->rx_data_bytes_remaining --;
			if(mfg->rx_data_bytes_remaining == 1)
			{
				mfg->rx_state = MFG_RX_CS;
			}
		break;
			
		case MFG_RX_CS:
			mfg->rx_eop = true;
			if (CRC_2s_complement(&mfg_dll.rx_buffer[0], mfg->rx_buffer_bytes) == data)
			{
				mfg->rx_buffer[mfg->rx_buffer_bytes] = data;
				mfg->rx_buffer_bytes++;
				mfg->rx_data_bytes_remaining --;
				
				mfg->rx_error = false;
				mfg->ongoing_transmission = false;
				mfg->rx_state = MFG_RX_IDLE;
				mfg->uart_task = MFG_CALLBACK_TASK;
				mfg->trigger_task = true;
				if(mfg->trigger_task == true)
				{
					SCH_SetTask( (mfg->uart_task) );
				}
			}
			else
			{
				mfg->rx_error = true;
				mfg->uart_task = MFG_CALLBACK_TASK;
				mfg->trigger_task = true;
				if(mfg->trigger_task == true)
				{
					SCH_SetTask( (mfg->uart_task) );
				}
				mfg->rx_abort();
			}
		break;
		
		default:
			mfg->rx_state = MFG_RX_IDLE;
			mfg->ongoing_transmission = false;
		break;
	}
	
	if ( mfg->ongoing_transmission == true )
	{
		TimerStart(&MFG_rtos_rx_abort_task);
	}
	
	mfg->interrupt_context = false;
}

/**
******************************************************************************
* \fn void MFG_tx_int_handler(MFG_DLL_TYPE * mfg)
* \brief GWH0079 Protocol Uart Rx Interrupt handler
*
* \param void
* \retval void
******************************************************************************
*/
void MFG_tx_int_handler(MFG_DLL_TYPE * mfg)
{
  mfg->interrupt_context = true;
	mfg->ongoing_transmission = true;
	
	TimerStop(&MFG_rtos_tx_abort_task);
	
	switch( mfg->tx_state )
	{
		case MFG_TX_STP:
			
			mfg->tx_eop = false;
			mfg->uart->TDR = mfg->tx_buffer[mfg->tx_buffer_bytes];
			mfg->tx_buffer_bytes++;
			mfg->tx_data_bytes_remaining--;
			mfg->tx_state = MFG_TX_DATA;	
		break;
		
		case MFG_TX_DATA:
			mfg->uart->TDR = mfg->tx_buffer[mfg->tx_buffer_bytes];
			mfg->tx_buffer_bytes++;
			mfg->tx_data_bytes_remaining--;
			mfg->tx_state = MFG_TX_DATA;
			
			if(mfg->tx_data_bytes_remaining == 0)
			{
				mfg->tx_eop = true;
				mfg->ongoing_transmission = false;
				mfg->tx_state = MFG_TX_IDLE;
				mfg->uart->ISR = HAL_UART_STATE_READY;	//UART disable TXRDY interrupt
				mfg->wait_for_ack = false; 							//DEBUG_EC
				LL_USART_DisableIT_TXE(USART2);
				TimerStop(&MFG_rtos_tx_abort_task);
			}
			
		break;
		
		default:
			mfg->ongoing_transmission = false;
			mfg->tx_state = MFG_TX_IDLE;
			LL_USART_DisableIT_TXE(USART2);
		break;
		
		if ( mfg->ongoing_transmission == true )
		{
			TimerStart(&MFG_rtos_tx_abort_task);
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
 */
 