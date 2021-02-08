/**
 ******************************************************************************
 * \file dl.h
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

#ifndef  DL_H
#define  DL_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
#include "gwh0079.h"
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
#include "cm.h"
#include "ul.h"
#include "nic.h"
#include "ct.h"
#include "util.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/

#define DL_PACKET_SIZE 		8
#define DL_DATA_SIZE	 		6	

/* BYTES DENINITION OF DOWNLINK MESSAGE  */

#define DL_INDEX 			 0x00 	/* member of array where is the index*/
#define DL_INSTRUCTION 0x01		/* member of array where is the instruction */
#define DL_DATA				 0x02		/* member of array where is start data  */
#define DL_CHKSUM			 0x07		/* member of array where is the checksum*/



/*===========================================================================*\
 * NULA COMUNICACIÓN: COMMAND FOR INTRUCTION 0XD0
\*===========================================================================*/
#define ERR_METER_ID_SIZE 3

static const uint8_t ERR_METER_ID		 [ERR_METER_ID_SIZE]={0x4E,0x4F,0x52};


/*===========================================================================*\
 * ERROR DE MENSAJE: COMMAND FOR INTRUCTION 0X03
\*===========================================================================*/
#define ERR_MSJ_SIZE 9

static const uint8_t ERR_MSJ		 [ERR_MSJ_SIZE]={0x45,0x52,0x52,0x4F,0x52,0x20,0x4D,0x53,0x4A};

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

/*===========================================================================*\
 * GWH0079 SiNaMed struct
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
extern void DL_task(void);
extern void DL_err_num_meter(void);
extern void DL_err_mensaje(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
 #endif
