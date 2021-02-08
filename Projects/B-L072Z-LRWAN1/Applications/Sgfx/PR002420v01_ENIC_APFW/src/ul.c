	/**
 ******************************************************************************
 * \file ul.c
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

#include "ul.h"
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


UL_STATES ul_sts;

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
 * \fn UL_send_to_sfx
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
 
void UL_send_to_sfx(void)
{
	uint8_t cksumcalc;
	uint8_t ul_msg[UL_PACKET_SIZE] = {NULL};
  uint8_t dl_msg[DL_PACKET_SIZE] = {NULL};
  uint8_t ul_size = UL_PACKET_SIZE;
	uint8_t bkp_system_state;
	
  uint32_t nbTxRepeatFlag=1;
	
	/*Add Index to buffer */
	ul_msg[UL_INDEX] = nic_apl.index;
	
	/*Add Instruction to buffer */
	ul_msg[UL_INSTRUCTION] =nic_apl.instruction;
	
	/* COPY THE DATA FROM COMAND STRUCTURE*/
	memcpy(&ul_msg[UL_DATA],nic_dll.ul_data,UL_DATA_SIZE);
	
	cksumcalc = CRC_2s_complement(ul_msg,(UL_PACKET_SIZE_WO_CHKSUM));
	ul_msg[UL_CRC] =cksumcalc;

	bkp_system_state = device.system_state;
	device.system_state = DEVICE_BUSY_SGFX;

	SIGFOX_API_send_frame(ul_msg, UL_PACKET_SIZE, dl_msg, nbTxRepeatFlag, nic_apl.IsEnable_dl_msj);
	
	device.system_state = DEVICE_RUNNING;
	bkp_system_state = device.system_state;
	
	/* Check if downlink is enable */
	if(nic_apl.IsEnable_dl_msj == false)
	{
		/*Check if it is an error state*/
		if((ul_msg[UL_INSTRUCTION]==INST_B0_ID)||(ul_msg[UL_INSTRUCTION]==INST_C0_ID)||(ul_msg[UL_INSTRUCTION]==INST_D0_ID)||(ul_msg[UL_INSTRUCTION]==INST_03_ID))
		{
			/*reset and start recurring reporter */
			REP_reconfig_recurring_rep();
		}
		if(rep.IsRep_executing == true)
		{
			/* Increment the counter to next intruction of reporter*/
		rep.current_rep ++;
			/* Callback to reporter managger*/
		SCH_SetTask(REP_MGR);
		}
		if(nic_apl.IsPending_execution == true)
		{
			/* check if after of executing an instruction */
			UL_pending_execution(nic_apl.instruction);
		}
		
	}
	else if(nic_apl.IsEnable_dl_msj == true)
	{
		/* Disable downlink message after that was enabled in recurring reporter*/
		nic_apl.IsEnable_dl_msj = false;
	
		/*agregar logica cuando el buffer venga en ceros*/
		if(dl_msg[DL_INDEX] == nic_apl.index)
		{
		/*copy downlink instruction*/ 
		nic_apl.instruction = dl_msg[DL_INSTRUCTION];
			
		memcpy(&nic_dll.dl_data,&dl_msg[DL_DATA],DL_DATA_SIZE);
			
		SCH_SetTask(DL_DATA_SFX);
		}
		else
		{
			DL_err_mensaje(); /*Error si la instruccion no corresponde con ninguna anterior*/
		}

	}
}
 /**
 ******************************************************************************
 * \fn UL_pending_execution(uint8_t instruction)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void UL_pending_execution(uint8_t instruction)
{
		switch (instruction)
		{				
			case INST_04_ID:
				CT_reset_scaling_board();
			break;

			default:
			break;	
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
