/**
 ******************************************************************************
 * \file ct.c
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

#include "ct.h"

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
TimerEvent_t shutdown_timer;
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
 
 /*
 ******************************************************************************
 * \fn void CT_init_config(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
 void CT_init_config(void)
 {

 }
  /*
 ******************************************************************************
 * \fn void CT_execute_instruction(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
 
 void CT_execute_instruction(void)
 {
 
		switch (nic_apl.instruction)
				{
					case INST_01_ID:
						CT_instruction_01();
					break;
					
					case INST_02_ID:
						CT_instruction_02();
					break;
					
					case INST_04_ID:
						CT_instruction_04();
					break;

					case INST_05_ID:
						CT_instruction_05();
					break;
					
					case INST_06_ID:
						CT_instruction_06();
					break;
					
					default:
					break;	
				}
					
 }
   /*
 ******************************************************************************
 * \fn void CT_instruction_01(void)		
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_instruction_01(void)	
{
	nic_apl.IsInst_exe_success =false;

	/*write code to acomplish instruction */

	nic_apl.IsPending_execution = false;
	nic_apl.IsInst_exe_success = true;
	SCH_SetTask(NIC_MGR);
}
  /*
 ******************************************************************************
 * \fn void CT_instruction_02 (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_instruction_02(void)	
{
	nic_apl.IsInst_exe_success =false;

	/*write code to acomplish instruction */

	nic_apl.IsPending_execution = false;
	nic_apl.IsInst_exe_success = true;
	SCH_SetTask(NIC_MGR);
}
 /*
 ******************************************************************************
 * \fn void CT_instruction_04 (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_instruction_04 (void)	
{
	nic_apl.IsInst_exe_success =false;

	/*write code to acomplish instruction */

	nic_apl.IsPending_execution = false;
	nic_apl.IsInst_exe_success = true;
	SCH_SetTask(NIC_MGR);
}
 /*
 ******************************************************************************
 * \fn void CT_instruction_05 (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_instruction_05 (void)	
{
	nic_apl.IsInst_exe_success =false;

	/*write code to acomplish instruction */

	nic_apl.IsPending_execution = false;
	nic_apl.IsInst_exe_success = true;
	SCH_SetTask(NIC_MGR);
} 
/*
 ******************************************************************************
 * \fn void CT_instruction_06 (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_instruction_06 (void)	
{
	nic_apl.IsInst_exe_success =false;

	/*write code to acomplish instruction */

	nic_apl.IsPending_execution = false;
	nic_apl.IsInst_exe_success = true;
	SCH_SetTask(NIC_MGR);
}
/*
 ******************************************************************************
 * \fn void CT_reset_scaling_board (void)	
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CT_reset_scaling_board(void)
{
	/*	Shutdown scaling board */
		NVIC_SystemReset();
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
