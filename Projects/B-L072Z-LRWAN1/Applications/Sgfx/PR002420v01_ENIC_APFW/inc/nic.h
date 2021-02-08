/**
 ******************************************************************************
 * \file nic.h
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

#ifndef  NIC_H
#define  NIC_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
#include "ul.h"
#include "dl.h"
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
#include "rep.h"
#include "cm.h"
#include "ucp.h"
#include "timeServer.h"
#include "system.h"
#include "eeprom.h"
#include "ct.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/
typedef void (*void_fptr) (void);               // void void Function pointer typedef

/* TABLA 4. PRIMER DIGITO DE INDICE. DIVICION IMPLEMNTADA */
typedef enum index_div_type
{
	VALLE_DE_MEXICO_NORTE =0,
	VALLE_DE_MEXICO_CENTRO,
	VALLE_DE_MEXICO_SUR,
	BAJA_CALIFORNIA,
	NOROESTE,
	NORTE,
	GOLFO_NORTE,
	CENTRO_OCCIDENTE,
	CENTRO_SUR,
	ORIENTE,
	SURESTE,
	BAJIO,
	GOLFO_CENTRO,
	CENTRO_ORIENTE,
	PENINSULAR,
	JALISCO,
	
}INDEX_DIV_TYPE;

/*	TABLA 5. PRIMER DIGITO DE INDICE. TECNOLOGÍA IMPLEMNTADA */
typedef enum index_tech_type
{
	MED_1_PH_GHW00_09 =0,
	MED_2_PH_GHW00_09,
	MED_3_PH_GHW00_09,
	MED_1_PH_GHW00_34,
	MED_2_PH_GHW00_34,
	
}INDEX_TECH_TYPE;

/*===========================================================================*\
 * C12.19 tables programmed
\*===========================================================================*/


/*	APENDIX C INSTRUCCIONES DESDE SINAMED TO SCALING BOARD*/
#define IDLE_INST		0x00		/* DUMMY INSTRUCTION */
#define INST_10_ID	0x10		/*INSTRUCCIÓN DE IDENTIFICADOR MEDIDOR*/
#define	INST_20_ID	0x20		/*INSTRUCCIÓN LECTURA GENERAL DEL MEDIDOR*/
#define	INST_30_ID	0x30		/*INSTRUCCIÓN INSTANTANEOS FASE A*/
#define	INST_40_ID	0x40		/*INSTRUCCIÓN INSTANTANEOS FASE C*/
#define	INST_50_ID	0x50		/*INSTRUCCIÓN APERTURA DE RELEVADOR*/
#define	INST_60_ID	0x60		/*INSTRUCCIÓN CIERRE DE RELVADOR*/
#define	INST_01_ID	0x01		/*INSTRUCCIÓN PROGRAMACIÓN TIEMPO DE RESPUESTA*/
#define	INST_02_ID	0x02		/*INSTRUCCIÓN PROGRAMACIÓN DE FRECUENCIA DE TIEMPO DE REPORTEO*/

#define	INST_04_ID	0x04		/*INSTRUCCIÓN REINICIO*/
#define	INST_05_ID	0x05		/*INSTRUCCIÓN INTERRUPCIONES*/
#define	INST_06_ID	0x06		/*INSTRUCCIÓN PROGRAMACIÓN*/
#define	INST_70_ID	0x70		/*INSTRUCCIÓN DEMANDAS*/
#define	INST_80_ID	0x80		/*INSTRUCCIÓN DEMANDAS MENSUALES*/
#define	INST_90_ID	0x90		/*INSTRUCCIÓN ESTADO*/
#define	INST_A0_ID	0xA0		/*INSTRUCCIÓN MODALIDAD*/
#define	INST_E0_ID	0xE0		/*INSTRUCCIÓN PREPAGO*/
#define	INST_F0_ID	0xF0		/*INSTRUCCIÓN POSPAGO*/
#define	INST_11_ID	0x11		/*INSTRUCCIÓN NORMAL*/
#define	INST_21_ID	0x21		/*INSTRUCCIÓN BIDIRECCIONAL*/
#define	INST_31_ID	0x31		/*INSTRUCCIÓN ENERGÍAS BIDIRECCIONAL*/
#define	INST_41_ID	0x41		/*INSTRUCCIÓN DEMANDAS BIDIRECCIONAL*/
#define	INST_51_ID	0x51		/*INSTRUCCIÓN INSTANTANEOS FASE B*/
#define	INST_61_ID	0x61		/*INSTRUCCIÓN CONSUMO MENSUAL*/
#define	INST_71_ID	0x71		/*INSTRUCCIÓN UNIDIRECCIONAL*/
#define	INST_81_ID	0x81		/*INSTRUCCIÓN FRECUANCIAS*/

#define INST_D0_ID  0xD0    /* NULA COMUNICACION */
#define INST_C0_ID	0xC0		/* NUMERO DE MEDIDOR ERRONEO*/
#define INST_B0_ID  0xB0    /* ERROR DE COMUNICACION */
#define	INST_03_ID	0x03		/* ERROR POR ERROR DE MENSAJE*/

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

/*----------------------------------- UPLINK DATA ------------------------*/
/*		INDEX			|	INTRUCTION	| 						DATA    				|		CHECKSUM 	*/
/*		1BYTE			|	1BYTE     	| 						9 BYTES 				|		1 BYTE	  */
/*------------------------------------------------------------------------*/
#define NIC_UL_DATA_SIZE			9

/*----------------------------------- DOWNLINK DATA ------------*/
/*		INDEX			|	INTRUCTION	| 						DATA    					  */
/*		1BYTE			|	1BYTE     	| 						6 BYTES 				 		*/
/*--------------------------------------------------------------*/
#define NIC_DL_DATA_SIZE			6 

#define NIC_RETRIES_NBR 		  3
#define NIC_METER_ID_SIZE  		6 /*Byte size of id meter*/


typedef enum device_dir
{
SEND_TO_METER =0,
SEND_TO_SINAMED,
SEND_ERR_MSJ,

}DEVICE_DIR_TYPE;


typedef struct nic_dll_type{
	uint8_t dl_data[NIC_DL_DATA_SIZE];
	uint8_t ul_data[NIC_UL_DATA_SIZE];

}NIC_DLL_TYPE;



typedef struct nic_apl_type{

	uint8_t index;												/* index byte of scaling board */
	uint8_t divition;											/* divition where is located the meter */
	uint8_t technology;										/* meter technology */
	uint8_t instruction;									/* Instruction to execute */ 

	uint8_t id_meter[NIC_METER_ID_SIZE];	/* meter ID*/
	uint8_t dir_instruction;							/* Dir */													

	bool 		IsInst_exe_success;
	bool		IsEnable_dl_msj;
	bool		IsPending_execution; 
	
	uint8_t ErrorStatus;
	
} NIC_APL_TYPE;
	



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

extern NIC_DLL_TYPE nic_dll;
extern NIC_APL_TYPE nic_apl;

extern void NIC_run_instruction(NIC_APL_TYPE *nic);

extern void NIC_initialize(void);
extern void NIC_verify_index(void);
extern void NIC_check_parameters(void);
extern void NIC_refresh_params(void);
extern void NIC_initialize_timer_refresh_data(void);

extern void NIC_apl_mgr(void);

extern void NIC_INST_10_excecute(void);
extern void NIC_INST_20_excecute(void);
extern void NIC_INST_30_excecute(void);
extern void NIC_INST_50_excecute(void);
extern void NIC_INST_60_excecute(void);

extern void NIC_INST_01_excecute(void);
extern void NIC_INST_02_excecute(void);
extern void NIC_INST_04_excecute(void);
extern void NIC_INST_03_excecute(void);
extern void NIC_INST_05_excecute(void);
extern void NIC_INST_06_excecute(void);

/* Error prototypes definitions */
extern void NIC_INST_D0_excecute(void);
/* Error por numero de medidor incorrecto */
extern void NIC_INST_C0_excecute(void);
/* Error de comunicación */
extern void NIC_INST_B0_excecute(void);
/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
