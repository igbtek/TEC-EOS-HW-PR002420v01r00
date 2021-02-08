/**
 ******************************************************************************
 * \file eeprom.h
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

#ifndef  EEPROM_H
#define  EEPROM_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */

#include <stdbool.h>
#include <stdint.h>
#include "eeprom_cfg.h"
#include "stm32l0xx_hal_flash.h"
#include "b-l072z-lrwan1.h"
#include "gwh0079.h"
#include "debug.h"
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
#include "timer.h"
#include "ucp.h"
#include "bsp.h"
#include "timeServer.h"
#include "hw_gpio.h"
#include "stm32l0xx_ll_rcc.h"
#include "ul.h"
#include "ucp.h"
#include "nic.h"
#include "util.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

 
/*===========================================================================*\
 * External 512KB EEPROM
\*===========================================================================*/
//#define EXT_EEPROM_START_ADDRESS			0x00000
//#define EXT_EEPROM_END_ADDRESS				0x7FFFF

#define USE_INTERNAL_EEPROM

#ifdef USE_INTERNAL_EEPROM

#define EEPROM_START_ADDRESS										0x08080200	//0x08080000 - 0x080801FF = 512 Bytes for Sigfox config settings
#define EEPROM_SGFX_START_PARAM_ADDRESS					0x08080300
#define EEPROM_END_ADDRESS											0x080817FF	//0x08080200 - 0x080817FF = 5632 Bytes for user

#else 

	#define USE_EXTERNAL_EEPROM
	#define EEPROM_START_ADDRESS									0x00000
	#define EEPROM_END_ADDRESS										0x7FFFF
	#define EEPROM_SGFX_START_PARAM_ADDRESS				0x08000300

#endif
/*
 ******************************************************************************
 *								Parameter Sizes (Using C1219 Tables as reference)
 ******************************************************************************
*/

/***************** EEPROM Parameter IDs *****************/
	/*ST-1 Parameter IDs*/
#define	EEPROM_MANUFACTURER_SN_ID  								0x01
#define	EEPROM_ED_MODEL_ID												0x02
#define	EEPROM_HW_VERSION_NUMBER_ID								0x03
#define	EEPROM_HW_REVISION_NUMBER_ID							0x04
#define	EEPROM_FW_VERSION_NUMBER_ID								0x05
#define	EEPROM_FW_REVISION_NUMBER_ID							0x06
#define	EEPROM_MFG_SERIAL_NUMBER_ID								0x07

/*MT-1 Parameter IDs*/
#define	EEPROM_FW_PART_NUMBER_ID									0x08
#define	EEPROM_HW_PART_NUMBER_ID									0x09
//#define	EEPROM_ED_IDENTITY_ID										0x10
#define	EEPROM_BOARD_ASSEMBLY_SN_ID								0x0A
#define	EEPROM_MFG_MODULE_SERIAL_NUMBER_ID				0x0B
#define	EEPROM_BOOTLOADER_PART_NUMBER_ID					0x0C

/*Procedure IDs*/
#define	EEPROM_MFG_2_MODE_ID 											0x40		/* Enable/Disable Manufacturer Mode */
#define	EEPROM_MASS_ERASE_MEMORY_ID								0x41		/* Erase All EEPROM Memory Parameters*/
#define	EEPROM_TEST_METER_COMMUNICATION_ID				0x42		/* Test Meter communication sending command */
#define	EEPROM_TEST_LED_ID												0x43		/* Test LED_RGB */
#define	EEPROM_TEST_SGFX_COMMUNICATION						0x44		/* Test SGFX Communication*/
#define	EEPROM_TEST_EEPROM_MEMORY_ID							0x45		/* Test Write/Read on EEPROM Memory*/
#define	EEPROM_METER_FORWARD_COMMAND							0x46		/* Tes Forward command to meter waiting for its response*/

/*Sigfox user parameters IDs*/
#define	EEPROM_SGFX_ID_METER_ID										0x80
#define	EEPROM_SGFX_ANS_TIME_FM_ID								0x81
#define	EEPROM_SGFX_REP_TIME_ID										0x82
#define	EEPROM_SGFX_INDEX_ID											0x83
#define	EEPROM_SGFX_RESET_COUNTER_ID							0x84

#define	EEPROM_SGFX_DEVICE_ID_ID									0x90
#define	EEPROM_SGFX_DEVICE_PAC_ID									0x91

#define	EEPROM_SIGFOX_PARAM_1_ID									0x92
#define	EEPROM_SIGFOX_PARAM_2_ID									0x93
#define	EEPROM_SIGFOX_PARAM_3_ID									0x94

/*ST-1 Sizes*/
#define EEPROM_MANUFACTURER_SN_SIZE								(uint8_t)4
#define EEPROM_ED_MODEL_SIZE											(uint8_t)8
#define EEPROM_HW_VERSION_NUMBER_SIZE							(uint8_t)1			
#define EEPROM_HW_REVISION_NUMBER_SIZE						(uint8_t)1
#define EEPROM_FW_VERSION_NUMBER_SIZE							(uint8_t)1
#define EEPROM_FW_REVISION_NUMBER_SIZE						(uint8_t)1
#define EEPROM_MFG_SERIAL_NUMBER_SIZE							(uint8_t)16
/*MT-1 Sizes*/
#define EEPROM_FW_PART_NUMBER_SIZE								(uint8_t)8
#define EEPROM_HW_PART_NUMBER_SIZE								(uint8_t)8
#define EEPROM_ED_IDENTITY_SIZE										(uint8_t)1
#define EEPROM_BOARD_ASSEMBLY_SN_SIZE							(uint8_t)16
#define EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE			(uint8_t)16
#define EEPROM_BOOTLOADER_PART_NUMBER_SIZE				(uint8_t)10
#define EEPROM_SGFX_DEVICE_PAC_SIZE								(uint8_t)8
#define EEPROM_SGFX_DEVICE_ID_SIZE								(uint8_t)4

#define EEPROM_MFG_2_MODE_PARAMETERS_SIZE					(uint8_t)1

/*Sigfox user parameters Sizes*/
#define EEPROM_SGFX_ID_METER_SIZE									(uint8_t)6
#define EEPROM_SGFX_ANS_TIME_FM_SIZE							(uint8_t)2
#define EEPROM_SGFX_REP_TIME_SIZE                 (uint8_t)1
#define EEPROM_SGFX_INDEX_SIZE                    (uint8_t)1
#define EEPROM_SGFX_RESET_COUNTER_SIZE						(uint8_t)3
#define EEPROM_SGFX_RESET_FLAG_SIZE								(uint8_t)1

#define EEPROM_SIGFOX_PARAM_1_SIZE								(uint8_t)1
#define EEPROM_SIGFOX_PARAM_2_SIZE								(uint8_t)2
#define EEPROM_SIGFOX_PARAM_3_SIZE								(uint8_t)3

/*
 ******************************************************************************
 *								Parameter Addresses (Using C1219 Tables as reference)
 ******************************************************************************
*/
/*ST-1 Addresses*/									
#define EEPROM_MANUFACTURER_SN_ADDR													EEPROM_START_ADDRESS
#define EEPROM_ED_MODEL_ADDR																EEPROM_MANUFACTURER_SN_ADDR         		+ EEPROM_MANUFACTURER_SN_SIZE		
#define EEPROM_HW_VERSION_NUMBER_ADDR												EEPROM_ED_MODEL_ADDR                		+ EEPROM_ED_MODEL_SIZE					
#define EEPROM_HW_REVISION_NUMBER_ADDR											EEPROM_HW_VERSION_NUMBER_ADDR       		+ EEPROM_HW_VERSION_NUMBER_SIZE	
#define EEPROM_FW_VERSION_NUMBER_ADDR												EEPROM_HW_REVISION_NUMBER_ADDR      		+ EEPROM_HW_REVISION_NUMBER_SIZE
#define EEPROM_FW_REVISION_NUMBER_ADDR											EEPROM_FW_VERSION_NUMBER_ADDR       		+ EEPROM_FW_VERSION_NUMBER_SIZE	
#define EEPROM_MFG_SERIAL_NUMBER_ADDR												EEPROM_FW_REVISION_NUMBER_ADDR					+ EEPROM_FW_REVISION_NUMBER_SIZE
/*MT-1 Addresses*/
#define EEPROM_FW_PART_NUMBER_ADDR													EEPROM_MFG_SERIAL_NUMBER_ADDR 					+ EEPROM_MFG_SERIAL_NUMBER_SIZE
#define EEPROM_HW_PART_NUMBER_ADDR													EEPROM_FW_PART_NUMBER_ADDR							+ EEPROM_FW_PART_NUMBER_SIZE						
#define EEPROM_ED_IDENTITY_ADDR															EEPROM_HW_PART_NUMBER_ADDR              + EEPROM_HW_PART_NUMBER_SIZE						
#define EEPROM_BOARD_ASSEMBLY_SN_ADDR												EEPROM_ED_IDENTITY_ADDR                 + EEPROM_ED_IDENTITY_SIZE								
#define EEPROM_MFG_MODULE_SERIAL_NUMBER_ADDR								EEPROM_BOARD_ASSEMBLY_SN_ADDR           + EEPROM_BOARD_ASSEMBLY_SN_SIZE					
#define EEPROM_BOOTLOADER_PART_NUMBER_ADDR									EEPROM_MFG_MODULE_SERIAL_NUMBER_ADDR    + EEPROM_MFG_MODULE_SERIAL_NUMBER_SIZE
#define EEPROM_SGFX_DEVICE_ID_ADDR													EEPROM_BOOTLOADER_PART_NUMBER_ADDR			+ EEPROM_BOOTLOADER_PART_NUMBER_SIZE
#define EEPROM_SGFX_DEVICE_PAC_ADDR													EEPROM_SGFX_DEVICE_ID_ADDR							+ EEPROM_SGFX_DEVICE_ID_SIZE

/*Sigfox user parameters addresses*/
#define EEPROM_SGFX_ID_METER_ADDR														EEPROM_SGFX_START_PARAM_ADDRESS
#define EEPROM_SGFX_ANS_TIME_FM_ADDR												EEPROM_SGFX_ID_METER_ADDR								+ EEPROM_SGFX_ID_METER_SIZE
#define EEPROM_SGFX_REP_TIME_ADDR     											EEPROM_SGFX_ANS_TIME_FM_ADDR            + EEPROM_SGFX_ANS_TIME_FM_SIZE
#define EEPROM_SGFX_INDEX_ADDR        											EEPROM_SGFX_REP_TIME_ADDR               + EEPROM_SGFX_REP_TIME_SIZE   
#define EEPROM_SGFX_RESET_COUNTER_ADDR											EEPROM_SGFX_INDEX_ADDR                  + EEPROM_SGFX_INDEX_SIZE
#define EEPROM_SGFX_RESET_FLAG_ADDR												  EEPROM_SGFX_RESET_COUNTER_ADDR 					+ EEPROM_SGFX_RESET_COUNTER_SIZE

#define EEPROM_SIGFOX_PARAM_1_ADDR													EEPROM_BOOTLOADER_REVISION_NUMBER_ADDR 	+ EEPROM_BOOTLOADER_REVISION_NUMBER_SIZE
#define EEPROM_SIGFOX_PARAM_2_ADDR													EEPROM_SIGFOX_PARAM_1_ADDR							+ EEPROM_SIGFOX_PARAM_1_SIZE
#define EEPROM_SIGFOX_PARAM_3_ADDR													EEPROM_SIGFOX_PARAM_2_ADDR							+ EEPROM_SIGFOX_PARAM_2_SIZE

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
extern uint16_t eeprom_resets_counter(bool inc);

extern void EEPROM_initialize(void);
extern void EEPROM_io_initialize(void);
extern void EEPROM_io_configure(void);
extern void EEPROM_twi_recovery(void);
extern void EEPROM_twi_enable_pio_mode(void);
extern void EEPROM_twi_enable_peripheral_mode(void);
extern void EEPROM_twi_reconfig_speed(void);
extern bool EEPROM_writer(uint32_t address, uint8_t* data_pointer, unsigned int data_length);
extern bool EEPROM_reader(uint32_t address, uint8_t* data_pointer, unsigned int data_length);

extern void EEPROM_erase_all(void);
extern void EEPROM_check_parameters(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif
