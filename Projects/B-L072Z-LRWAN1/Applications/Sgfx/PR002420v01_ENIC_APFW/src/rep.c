/**
 ******************************************************************************
 * \file rep.c
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

#include "rep.h"

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
 
REP_TYPE rep;

TimerEvent_t initial_rep;
TimerEvent_t recurring_rep;


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
 * \fn  void REP_init_first_rep(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
 void REP_init_first_rep(void)
 {
	 uint32_t hex_val =0;
 	 uint8_t ID_SCALING_TARGET [6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
	 
	 volatile uint32_t time_scaling_t = 0;

	 /* init timer */
	 TimerInit(&initial_rep, REP_main_managger );
	 
	 rep.first_reporter |= ID_SCALING_TARGET [3] << 16;
	 rep.first_reporter |= ID_SCALING_TARGET [4] << 8;
	 rep.first_reporter |= ID_SCALING_TARGET [5];

	 rep.first_reporter = (rep.first_reporter) * ((uint32_t)TIME_CONSTANT);

	 TimerSetValue(&initial_rep, rep.first_reporter);

	 /*Set initial configuration to first report*/
	 rep.current_rep = FIRST_INS_REP;
	 /*Set up first instruction*/
	 rep.executing_state = FIRST_REP;
	 /*Start initial reporter*/
	 TimerStart(&initial_rep);
 }
  /**
 ******************************************************************************
 * \fn  void REP_init_recurring_rep(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
 
 void REP_init_recurring_rep(void)
 {
	 uint32_t period_recurring_rep =0;
	 uint8_t temp_param =0;
		/*Init rep timer instances */	 
	 TimerInit(&recurring_rep, REP_recurrent_rep);

	 EEPROM_reader(EEPROM_SGFX_REP_TIME_ADDR,&temp_param, EEPROM_SGFX_REP_TIME_SIZE);
	 
	 if((temp_param==0x00)||(temp_param==0xFF)||(temp_param > 0x0A))
		{
			period_recurring_rep = ((REP_RECURRING_TIME_DEFAULT)*(SET_TIME_MS_1HR));
		}
		else
		{
			period_recurring_rep = ((temp_param)*(SET_TIME_MS_1HR));
		}
			period_recurring_rep =10000; //KH_JUSTFORDEBUG
		/* Set timer period */
	 TimerSetValue(&recurring_rep, period_recurring_rep);
 } 
 /**
 ******************************************************************************
 * \fn  void REP_secuence(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
 void REP_secuence_of_rep(void)
 {
		 switch(rep.current_rep)
		 {
			case FIRST_INS_REP:
				/* instruction for 1 state */
				nic_apl.instruction = INST_10_ID;
				/* disable downlink message */
				nic_apl.IsEnable_dl_msj = false;

			break;
			
			case SECCOND_INS_REP:
				/* instruction for 3 state */
				nic_apl.instruction = INST_30_ID;
				/* disable downlink message */
				nic_apl.IsEnable_dl_msj = false;

			break;
			
			case THIRD_INS_REP:
				/* instruction for 2 state */
				nic_apl.instruction = INST_20_ID;
				/* Check if it is the first report */
			if(rep.executing_state == FIRST_REP)
				{
				/* Disable downlink messages */	
				nic_apl.IsEnable_dl_msj = false;
				}
			else if(rep.executing_state == RECURRING_REP)
				{
				/* Enable downlink messages */	
				nic_apl.IsEnable_dl_msj = true;
					
				}
				/* Enable end of report flag, when thrid instruction will be executed */
				rep.IsEnd_of_rep = true;

			break;
			
			default:
			break;
		 }
 }
 /**
 ******************************************************************************
 * \fn  void REP_first_rep(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
 void REP_main_managger(void)
 {
	 	 
	 switch(rep.executing_state)
	 {
				
		case FIRST_REP: /* case only for first report*/
			
			if(rep.current_rep == END_INS_REP)
			{
				/*Reset recurring counter*/
				REP_reconfig_recurring_rep();
			}
			else
			{
				/* enable reporter on going execution */
				rep.IsRep_executing = true;
				
				REP_secuence_of_rep();
				SCH_SetTask(NIC_MGR);
			}
		break;
		
		case RECURRING_REP: /* case for reccuring report*/
			
			if(rep.current_rep == END_INS_REP)
			{
				/*Reset recurring counter*/
				REP_reconfig_recurring_rep();
			}
			else
			{
				rep.IsRep_executing =true;
				REP_secuence_of_rep();
				SCH_SetTask(NIC_MGR);
			}
		break;
		
		default:
		break;
	 }
 }
 

  /**
 ******************************************************************************
 * \fn  void REP_recurrent_rep(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
void REP_recurrent_rep(void)
{
		rep.IsRep_executing =true;
		rep.executing_state = RECURRING_REP;
		rep.current_rep = SECCOND_INS_REP;
		SCH_SetTask(REP_MGR);
}

  /**
 ******************************************************************************
 * \fn  void REP_end_first_rep(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
void REP_reconfig_recurring_rep(void)
{		
		/* Before to start the recurring reporter reconfigure recurring reporter*/
		REP_init_recurring_rep();
		/* Disable IsRep_executing flag*/
		rep.IsRep_executing =false;
		REP_start_recurring_rep();
}

  /**
 ******************************************************************************
 * \fn  void REP_config_rep_recurring(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
 
void REP_start_recurring_rep(void)
{
	//	REP_stop_timers();
		TimerReset(&recurring_rep);
		TimerStart(&recurring_rep);
	
	
}
  /**
 ******************************************************************************
 * \fn  void REP_stop_timers(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 **/
void REP_stop_timers(void)
{
	TimerStop(&recurring_rep);
	TimerStop(&initial_rep);
}


 /*
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
