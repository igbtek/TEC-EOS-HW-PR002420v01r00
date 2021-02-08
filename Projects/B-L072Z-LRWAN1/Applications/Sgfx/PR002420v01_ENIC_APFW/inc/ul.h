/**
 ******************************************************************************
 * \file ul.h
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

#ifndef  UL_H
#define  UL_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "gwh0079.h"
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
#include "ul.h"
#include "dl.h"
#include "util.h"
#include "system.h"
#include "ucp.h"
#include "debug.h"
#include "command.h"
#include "stm32l0xx_ll_rcc.h"
#include "cm.h"
#include "nic.h"
#include "led.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

/*--------------------------------------------------------------------------------------------------*/
/*		INDEX			|	INTRUCTION	| 						DATA    				|		CHECKSUM 	|													*/
/*		1BYTE			|	1BYTE     	| 						9 BYTES 				|		1 BYTE	  |													*/
/*--------------------------------------------------------------------------------------------------*/

#define UL_PACKET_SIZE						12 	/* Size of uplink array  */
#define UL_PACKET_SIZE_WO_CHKSUM  11	/* Size of uplink array without checksum*/
#define UL_DATA_SIZE	 						 9	/* Size of uplink data*/

#define UL_INDEX 			 	0x00 	/* member of array where is the index*/
#define UL_INSTRUCTION 	0x01 	/* member of array where is the instruction */
#define UL_DATA				 	0x02 	/* member of array where is start data  */
#define UL_CRC				 	0x0B 	/* member of array where is the checksum*/


static const uint8_t COMAND_RELAY_OK 		[4] =	{0x00, 0x00, 0x4F, 0x4B};
static const uint8_t COMAND_RELAY_NO_OK [4] =	{0x45, 0x52, 0x52, 0x4F};
static const uint8_t ERR_COMUNICATION	 	[9] = {0x45, 0x52, 0x52, 0x4F, 0x52, 0x20, 0x4D, 0x45, 0x44};

static const uint8_t ERR_NUM_METEROR	 	[3] = {0x4E, 0x4F, 0x52};
static const uint8_t ERR_MESSAGE			 	[9] = {0x45, 0x52, 0x52, 0x4F, 0x52, 0x20, 0x4D, 0x53, 0x4A};
static const uint8_t NULL_COMUNICATION 	[9] = {0x4E, 0x55, 0x4C, 0x4C, 0x20, 0x43, 0x4F, 0x4D, 0x55};

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
 * UCP Communications state
\*===========================================================================*/

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern bool UL_check_data(void);
extern void UL_send_to_sfx(void);
extern void UL_pending_execution(uint8_t instruction);

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
