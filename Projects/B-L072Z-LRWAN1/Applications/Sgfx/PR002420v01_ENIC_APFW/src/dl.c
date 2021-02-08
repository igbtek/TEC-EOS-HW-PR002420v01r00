/**
 ******************************************************************************
 * \file dl.c
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
#include "dl.h"

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

/*
 ******************************************************************************
 * Static Variables and Const Variables With File Level Scope
 ******************************************************************************
 */
 
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
 * \fn void DL_task(void)
 * \brief Function that receive downlink message and proccess the instruction.
 *
 * \param void
 * \retval 
 ******************************************************************************
 */

void DL_task(void)
{
	uint8_t dl_frame[DL_PACKET_SIZE] ={NULL};
	uint8_t dl_data[DL_DATA_SIZE]={NULL};
	volatile uint8_t checksum = 0;
	bool id_meter= false;

	/*VALIDACIÓN PARA ENVIAR LOS DATOS A LA NIC DE ESCALAMIENTO */
	if(nic_apl.instruction ==INST_01_ID || nic_apl.instruction ==INST_02_ID || nic_apl.instruction ==INST_04_ID ||nic_apl.instruction ==INST_05_ID|| nic_apl.instruction ==INST_06_ID)
	{	
		dl_frame[DL_INDEX] =nic_apl.index;
		dl_frame[DL_INSTRUCTION] =nic_apl.instruction;
		memcpy(&dl_frame[DL_DATA],&nic_dll.dl_data,DL_DATA_SIZE);
		
		/* calculate the checksum from downlink frame */
		checksum = CRC_2s_complement(dl_frame,(DL_PACKET_SIZE - 1));
		/* check the checksum into downlink frame and checksum estimated */
		if(checksum == nic_dll.dl_data[5])
		{
			/*call to nic NIC MGR*/
			SCH_SetTask(NIC_MGR);
		}
		else
		{
			DL_err_mensaje();
		}
	}
	else
	{
		/* copy id meter from downlink message*/
			memcpy(&dl_data,&nic_dll.dl_data,NIC_METER_ID_SIZE);
		
		/*VALIDACIÓN SI EL MEDIDOR ID ES EL MISMO QUE SE RECIBE DESDE SINAMED*/
		id_meter = UTIL_compare_array(nic_apl.id_meter,dl_data, NIC_METER_ID_SIZE);
	
		if(id_meter==true)
		{
			/*Execute the instruction*/
			SCH_SetTask(NIC_MGR);
		}
		else
		{
			/* Send Error of meter id error to sigfox */
			DL_err_num_meter();
		
		}
	}
}
/**
 ******************************************************************************
 * \fn void DL_err_num_meter(void)
 * \brief Function that fills the uplink buffer in case of meter id error
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void DL_err_num_meter(void)
{
	/* copy error message*/
	memcpy(&nic_dll.ul_data,&ERR_METER_ID,ERR_METER_ID_SIZE);
	/* copy id of meter*/
	memcpy(&nic_dll.ul_data[3],&nic_apl.id_meter,NIC_METER_ID_SIZE);
	/* copy instruction ID*/
	nic_apl.instruction = INST_C0_ID;
	/* set up direction of instruction*/
	nic_apl.dir_instruction = SEND_ERR_MSJ;
	/* Enable Nic Manager Task */
	SCH_SetTask(NIC_MGR);
}

/**
 ******************************************************************************
 * \fn void DL_err_num_meter(void)
 * \brief Function that fills the uplink buffer in case of meter id error
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
void DL_err_mensaje(void)
{
	/* copy error message*/
	memcpy(&nic_dll.ul_data,&ERR_MSJ,ERR_MSJ_SIZE);
	/* copy instruction ID*/
	nic_apl.instruction = INST_03_ID;
	/* set up direction of instruction*/
	nic_apl.dir_instruction = SEND_ERR_MSJ;
	/* Enable Nic Manager Task */
	SCH_SetTask(NIC_MGR);
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
