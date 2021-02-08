/**
 ******************************************************************************
 * \file cm.h
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

#ifndef  CM_H
#define  CM_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
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

#define MS_TIME_RETRY 3000

#define SEND_FRAME_1  0
#define SEND_FRAME_2  1
#define SEND_FRAME_3  2

#define RECEIVE_RESPOSE_OF_FRAME_1  1
#define RECEIVE_RESPOSE_OF_FRAME_2  2
#define RECEIVE_RESPOSE_OF_FRAME_3  3

/*	APENDIX A COMADOS DE MEDIDORES  */
/*===========================================================================*\
 * MEDIDOR: COMMAND FOR INTRUCTION 0X10 1PH GWH0034
\*===========================================================================*/
#define MEDIDOR_EXE_CMDS		 		 (uint8_t)1
/*size of frame to send */
#define MEDIDOR_SIZE 						 (uint8_t)9

static const uint8_t MEDIDOR		[MEDIDOR_SIZE] = {0x4D, 0x45, 0x44, 0x49, 0x44, 0x4F, 0x52, 0x20, 0x0D};

#define EXP_RESP_MEDIDOR				 (uint8_t)6

/*===========================================================================*\
 * LECTURA ACUMULADA: COMMAND FOR INTRUCTION 0X20
\*===========================================================================*/
#define LECTURA_ACUMULADA_EXE_CMDS (uint8_t)2
/*Size of frame to send */
#define ENERGIA_SIZE 				     	 (uint8_t)8
#define ENERGIA_R_SIZE 			       (uint8_t)10

static const uint8_t ENERGIA			 [ENERGIA_SIZE] = {0x45, 0x4E, 0x45, 0x52, 0x47, 0x49, 0x41, 0x0D};
static const uint8_t ENERGIA_R		 [ENERGIA_R_SIZE]= {0x45, 0x4E, 0x45, 0x52, 0x47, 0x49, 0x41, 0x20, 0x52, 0x0D};

#define EXP_RESP_ENERGIA				 	 (uint8_t)12
#define EXP_RESP_ENERGIA_R				 (uint8_t)12


/*===========================================================================*\
 * INSTANTANEOS FASE A: COMMAND FOR INTRUCTION 0X30
\*===========================================================================*/
#define INSTANTANEOS_PH_A_EXE_CMDS (uint8_t)3

/*Length of frame to send */
#define VOLTAJE_A_SIZE 				   	 (uint8_t)10
#define CORRIENTE_A_SIZE 			   	 (uint8_t)12
#define POTENCIA_A_SIZE 			   	 (uint8_t)11

static const uint8_t VOLTAJE_A		 [VOLTAJE_A_SIZE]= {0x56, 0x4F, 0x4C, 0x54, 0x41, 0x4A, 0x45, 0x20, 0x41, 0x0D};
static const uint8_t CORRIENTE_A	 [CORRIENTE_A_SIZE]= {0x43, 0x4F, 0x52, 0x52, 0x49, 0x45, 0x4E, 0x54, 0x45, 0x20, 0x41, 0x0D};
static const uint8_t POTENCIA_A		 [POTENCIA_A_SIZE]= {0x50, 0x4F, 0x54, 0x45, 0x4E, 0x43, 0x49, 0x41, 0x20, 0x41, 0x0D};


#define EXP_RESP_VOLTAJE_A 				 (uint8_t)7
#define EXP_RESP_CORRIENTE_A		   (uint8_t)7
#define EXP_RESP_POTENCIA_A			   (uint8_t)7


/*===========================================================================*\
 * INSTANTANEOS FASE C: COMMAND FOR INTRUCTION 0X40
\*===========================================================================*/

static const uint8_t VOLTAJE_C		 [11]= {0x56, 0x4F, 0x4C, 0x41, 0x54, 0x41, 0x4A, 0x45, 0x20, 0x43, 0x0D};
static const uint8_t CORRIENTE_C	 [12]= {0x43, 0x4F, 0x52, 0x52, 0x49, 0x45, 0x4E, 0x54, 0x45, 0x20, 0x43, 0x0D};
static const uint8_t POTENCIA_C		 [11]= {0x50, 0x4F, 0x54, 0x45, 0x4E, 0x43, 0x49, 0x41, 0x20, 0x43, 0x0D};

/*===========================================================================*\
 * ABRIR: COMMAND FOR INTRUCTION 0X50
\*===========================================================================*/
#define ABRIR_EXE_CMDS				 (uint8_t)2
#define ABRIR_SIZE 					   (uint8_t)6

static const uint8_t ABRIR				 [ABRIR_SIZE] = {0x41, 0x42, 0x52, 0x49, 0x52, 0x0D};
static const uint8_t OK												[4] = {0x00, 0x00, 0x4F, 0x4B};
#define EXP_RESP_ABRIR	   		 (uint8_t)8

/*===========================================================================*\
 * CERRAR: COMMAND FOR INTRUCTION 0X60
\*===========================================================================*/
#define CERRAR_EXE_CMDS				 (uint8_t)2
#define CERRAR_SIZE 					 (uint8_t)7

static const uint8_t CERRAR				 [CERRAR_SIZE] = {0x43, 0x45, 0x52, 0x52, 0x41, 0x52, 0x0D};
#define EXP_RESP_CERRAR	   		 (uint8_t)9

/*===========================================================================*\
 * CERRAR: COMMAND FOR INTRUCTION 0X60
\*===========================================================================*/
static const uint8_t ENERGIA_MENS	 [13]= {0x45, 0x4E, 0x45, 0x52, 0x47, 0x49, 0x41, 0x20, 0x4D, 0x45, 0x4E, 0x53, 0x0D};

/*===========================================================================*\
 * Demandas Ambos sentidos: COMMAND FOR INTRUCTION 0X60
\*===========================================================================*/
static const uint8_t DEMANDA_E		 [10]= {0x44, 0x45, 0x4D, 0x41, 0x4E, 0x44, 0x41, 0x20, 0x45, 0x0D};
static const uint8_t DEMANDA_R		 [10]= {0x44, 0x45, 0x4D, 0x41, 0x4E, 0x44, 0x41, 0x20, 0x52, 0x0D};

/*===========================================================================*\
 * Demandas Ambos sentidos: COMMAND FOR INTRUCTION 0X60
\*===========================================================================*/
static const uint8_t DEMANDA_MENS	 [13]= {0x44, 0x45, 0x4D, 0x41, 0x4E, 0x44, 0x41, 0x20, 0x4D, 0x45, 0x4E, 0x53, 0x0D};
static const uint8_t DEMANDA_MES_A [14]= {0x44, 0x45, 0x4D, 0x41, 0x4E, 0x44, 0x41, 0x20, 0x4D, 0x45, 0x53, 0x20, 0x41, 0x0D};

/*===========================================================================*\
 * Frecuencias: COMMAND FOR INTRUCTION 0X81
\*===========================================================================*/
static const uint8_t FREC_A				 [7] = {0x46, 0x52, 0x45, 0x43, 0x20, 0x41, 0x0D};
static const uint8_t FREC_B				 [7] = {0x46, 0x52, 0x45, 0x43, 0x20, 0x42, 0x0D};
static const uint8_t FREC_C				 [7] = {0x46, 0x52, 0x45, 0x43, 0x20, 0x43, 0x0D};

/*===========================================================================*\
 * MOLADIDAD: COMMAND FOR INTRUCTION 0XA0
\*===========================================================================*/
static const uint8_t MOLADIDAD		 [10]= {0x4D, 0x4F, 0x4C, 0x41, 0x44, 0x49, 0x44, 0x41, 0x44, 0x0D};

/*===========================================================================*\
 * PREPAGO: COMMAND FOR INTRUCTION 0XE0
\*===========================================================================*/
static const uint8_t PREPAGO			 [8] = {0x50, 0x52, 0x45, 0x50, 0x41, 0x47, 0x4F, 0x0D};

/*===========================================================================*\
 * POSPAGO: COMMAND FOR INTRUCTION 0XF0
\*===========================================================================*/
static const uint8_t POSPAGO			 [8] = {0x50, 0x4F, 0x53, 0x50, 0x41, 0x47, 0x4F, 0x0D};

/*===========================================================================*\
 * NORMAL: COMMAND FOR INTRUCTION 0X11
\*===========================================================================*/
static const uint8_t NORMAL				 [7] = {0x4E, 0x4F, 0x52, 0x4D, 0x41, 0x4C, 0x0D};

/*===========================================================================*\
 * BIDIRECCIONAL: COMMAND FOR INTRUCTION 0X21
\*===========================================================================*/
static const uint8_t BIDIRECCIONAL [14]= {0x42, 0x49, 0x44, 0x49, 0x52, 0x45, 0x43, 0x43, 0x49, 0x4F, 0x4E, 0x41, 0x4C, 0x0D};

/*===========================================================================*\
 * UNIDIRECCIONAL: COMMAND FOR INTRUCTION 0X71
\*===========================================================================*/
static const uint8_t UNIDIRECCIONAL[15]= {0x55, 0x4E, 0x49, 0x44, 0x49, 0x52, 0x45, 0x43, 0x43, 0x49, 0x4F, 0x4E, 0x41, 0x4C, 0x0D};


/*===========================================================================*\
 * Energías Ambos Sentidos: COMMAND FOR INTRUCTION 0X71
\*===========================================================================*/
static const uint8_t ENTREGADA		 [10]= {0x45, 0x4E, 0x54, 0x52, 0x45, 0x47, 0x41, 0x44, 0x41, 0x0D};
static const uint8_t RECIBIDA			 [9] = {0x52, 0x45, 0x43, 0x49, 0x42, 0x49, 0x44, 0x41, 0x0D};

/*===========================================================================*\
 * LECTURA ACUMULADA: COMMAND FOR INTRUCTION 0X70
\*===========================================================================*/
static const uint8_t VOLTAJE_B		 [10]= {0x56, 0x4F, 0x4C, 0x54, 0x41, 0x4A, 0x45, 0x20, 0x42, 0x0D};
static const uint8_t CORRIENTE_B	 [12]= {0x43, 0x4F, 0x52, 0x52, 0x49, 0x45, 0x4E, 0x54, 0x45, 0x20, 0x42, 0x0D};
static const uint8_t POTENCIA_B		 [11]= {0x50, 0x4F, 0x54, 0x45, 0x4E, 0x43, 0x49, 0x41, 0x20, 0x42, 0x0D};



static const uint8_t ESTADO				 [8] = {0x45, 0x53, 0x54, 0x41, 0x44, 0x4F, 0x20, 0x0D};

/*===========================================================================*\
 * NULA COMUNICACIÓN: INTRUCTION COMAND 0XD0
\*===========================================================================*/

#define NULL_COM_SIZE 				 	 (uint8_t)9
static const uint8_t NULL_COMM	 [NULL_COM_SIZE]={0x4E,0x55,0x4C,0x4C,0x20,0x43,0x4F,0x4D,0x55};

/*===========================================================================*\
 * ERROR DE COMUNICACIÓN: INTRUCTION COMAND 0XB0
\*===========================================================================*/

#define ERR_COM_SIZE 				 	 	 (uint8_t)9
static const uint8_t ERR_COMM		 [ERR_COM_SIZE]={0x45,0x52,0x52,0x4F,0x52,0x20,0x4D,0x45,0x44};

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
typedef struct fm_typedef{
	
	uint8_t frame_1[15];	
	uint8_t frame_2[15];
	uint8_t frame_3[15];
	uint8_t frame_4[15];
	uint8_t size_frame_1;
	uint8_t size_frame_2;
	uint8_t size_frame_3;
	uint8_t size_frame_4;
	uint8_t bytes_exp_frame_1;
	uint8_t bytes_exp_frame_2;
	uint8_t bytes_exp_frame_3;
	uint8_t bytes_exp_frame_4;
	uint8_t nbr_cmds;
	
}FRAME_FACTORY_TYPE;

typedef struct cm_typedef{
FRAME_FACTORY_TYPE bf;						/* Struture that contains the frames tha will be utilized*/
uint8_t cm_rx_buffer[16];					/* Bufer reived from meter */
uint8_t cm_rx_bytes_expeccted;		/* Bytes expected from meter response */
uint8_t cm_retry;									/* Count of retries comands to meter */
uint8_t cm_executed;							/* Count of comansd executed succesfully */

bool IsOn_going_execution;				/* Flag that show a comand is on going extecution */
bool IsMulti_comand;							/* Flag that show a instruction has more than one comand*/
	
}CM_TYPE;

typedef enum err_status
{
NO_ERR =0,
NO_ANSWER =1,

}ERR_STATUS;


extern CM_TYPE cm;
extern FRAME_FACTORY_TYPE ff;
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
extern void CM_init_cmd_timeout_timer(void);

extern void CM_execute_instruction(void);

extern void CM_frame_factory(void);
extern void CM_frame_factory_for_1ph_ghw0034(void);

extern void CM_comands_to_meter_inst_10_1ph_ghw0034(void);
extern void CM_comands_to_meter_inst_20_1ph_ghw0034(void);
extern void CM_comands_to_meter_inst_30_1ph_ghw0034(void);
extern void CM_comands_to_meter_inst_40_1ph_ghw0034(void);
extern void CM_comands_to_meter_inst_50_1ph_ghw0034(void);
extern void CM_comands_to_meter_inst_60_1ph_ghw0034(void);


extern void CM_rx_data_analize(void);
extern void CM_build_ul_data(void);

extern void CM_build_ul_data_for_1ph_gwh0034(void);
extern void CM_build_ul_frame_10_1ph_gwh0034(void);
extern void CM_build_ul_frame_20_1ph_gwh0034(void);
extern void CM_build_ul_frame_30_1ph_gwh0034(void);
extern void CM_build_ul_frame_50_1ph_gwh0034(void);
extern void CM_build_ul_frame_60_1ph_gwh0034(void);

extern void CM_check_executed_comands(void);

extern void CM_null_communication(void);
extern void CM_err_communication(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
