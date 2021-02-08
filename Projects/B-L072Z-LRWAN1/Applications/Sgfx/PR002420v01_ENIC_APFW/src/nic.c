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

#include "nic.h"

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

/*	Structure for nic aplication*/
NIC_APL_TYPE nic_apl;
/*	Structure for buffer of DL and UL*/
NIC_DLL_TYPE nic_dll;
/* timer to refresh data*/
TimerEvent_t timer_to_refresh_data;

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
 * \fn void NIC_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_initialize(void)	
{
	/* Clear UL buffer */
	memset(&nic_dll.ul_data, 0, UL_DATA_SIZE);
	/* Clear DL buffer */
	memset(&nic_dll.dl_data, 0, DL_DATA_SIZE);
	/* Clear comands executed successfully */
	nic_apl.IsInst_exe_success = false;
	/* Init timers*/
	SYS_config_timers();
	/* Load index from eeprom */
	EEPROM_reader(EEPROM_SGFX_INDEX_ADDR, &nic_apl.index, EEPROM_SGFX_INDEX_SIZE);
	/* Set acodding to index the tecnology and divition of location*/
	NIC_verify_index();
	/* Clean instruction */
	nic_apl.instruction = IDLE_INST;
	/* Load meter ID from eeprom */
	EEPROM_reader(EEPROM_SGFX_ID_METER_ADDR, &nic_apl.id_meter[0], EEPROM_SGFX_ID_METER_SIZE);
}


 /**
 ******************************************************************************
 * \fn void NIC_verify_index(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_verify_index(void)
{
	/*Set default values*/
	if((nic_apl.index==0x00)||(nic_apl.index==0xFF))
	{
		nic_apl.index =0x03;
		EEPROM_writer(EEPROM_SGFX_INDEX_ADDR, &nic_apl.index, EEPROM_SGFX_INDEX_SIZE);
	}
	
	nic_apl.divition = (nic_apl.index >> 4);
	nic_apl.technology = ((0x0F)&(nic_apl.index));
}
/**
 ******************************************************************************
 * \fn void NIC_initialize_timer_refresh_data(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_initialize_timer_refresh_data(void)
{
	 uint32_t period_to_refresh_data =0;

		/* Init rep timer instances */	 
	 TimerInit(&timer_to_refresh_data, NIC_refresh_params);
		/* Every minute reconfig nic_apl params */
	 period_to_refresh_data =60000;
		/* Set timer period */
	 TimerSetValue(&timer_to_refresh_data, period_to_refresh_data);
		/* Start counter */
	 TimerStart(&timer_to_refresh_data);
}
/**
 ******************************************************************************
 * \fn void NIC_initialize(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_check_parameters(void)
{
	uint8_t temp_buffer[8];
	uint8_t temp_param = 0;
	
	/*Check Firmware Part Number*/
	EEPROM_reader(EEPROM_SGFX_INDEX_ADDR, &temp_buffer[0], EEPROM_SGFX_INDEX_SIZE);
	if(memcmp(&temp_buffer[0], "PR002420", EEPROM_FW_PART_NUMBER_SIZE) != 0)
	{
		EEPROM_writer(EEPROM_FW_PART_NUMBER_ADDR, (uint8_t*)"PR002420",EEPROM_FW_PART_NUMBER_SIZE);
	}
	/*Check Firmware Version*/
	EEPROM_reader(EEPROM_FW_VERSION_NUMBER_ADDR, &temp_buffer[0], EEPROM_FW_VERSION_NUMBER_SIZE);
	temp_param = temp_buffer[0];
	if(temp_param != 0x01)
	{
		temp_param = 0x01;
		EEPROM_writer(EEPROM_FW_VERSION_NUMBER_ADDR, &temp_param, EEPROM_FW_VERSION_NUMBER_SIZE);
	}
	/*Check Firmware Revision*/
	EEPROM_reader(EEPROM_FW_REVISION_NUMBER_ADDR, &temp_buffer[0], EEPROM_FW_REVISION_NUMBER_SIZE);
	temp_param = temp_buffer[0];
	if(temp_param != 0xF5)
	{
		temp_param = 0xF5;
		EEPROM_writer(EEPROM_FW_REVISION_NUMBER_ADDR, &temp_param, EEPROM_FW_REVISION_NUMBER_SIZE);
	}
}
 /**
 ******************************************************************************
 * \fn void NIC_refresh_params(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_refresh_params(void)	
{
	/*Check if any of vital fucntions are active  */
	if((rep.IsRep_executing!=true)&&(cm.IsOn_going_execution!=true)&&(nic_apl.IsEnable_dl_msj!=true))
		{
		/* Clear UL buffer */
		memset(&nic_dll.ul_data, 0, UL_DATA_SIZE);
		/* Clear DL buffer */
		memset(&nic_dll.dl_data, 0, DL_DATA_SIZE);
		/* Load index from eeprom */
		EEPROM_reader(EEPROM_SGFX_INDEX_ADDR, &nic_apl.index, EEPROM_SGFX_INDEX_SIZE);
		/* Clean instruction */
		nic_apl.instruction = IDLE_INST;
		/* Load meter ID from eeprom */
		EEPROM_reader(EEPROM_SGFX_ID_METER_ADDR, &nic_apl.id_meter[0], EEPROM_SGFX_ID_METER_SIZE);
		}
		/* Reset timer */
		TimerReset(&timer_to_refresh_data);
		/* Init timer */
		TimerStart(&timer_to_refresh_data);
}
  /**
 ******************************************************************************
 * \fn void NIC_apl_sch(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void NIC_apl_mgr(void)
{	
	cm.cm_executed =0;

	switch (nic_apl.instruction)
	{
		case IDLE_INST: 	/* IDLE INSTRUCTION */
		break;

		case INST_10_ID: 	/* Instrucción 0x10 ID de medidor */
			NIC_INST_10_excecute();
		break;

		case INST_20_ID:	/* Instrucción 0x20 Energias Acumuladas */
			NIC_INST_20_excecute();
		break;

		case INST_30_ID:	/* Instrucción 0x30 Valores instantaneos */
			NIC_INST_30_excecute();
		break;
		
		case INST_40_ID:
		break;

		case INST_50_ID:	/*Instrucción 0x50 Abrir relevador */
			NIC_INST_50_excecute();
		break;

		case INST_60_ID:	/*Instrucción 0x60 Cerrar relevador */
			NIC_INST_60_excecute();
		break;
		
		case INST_70_ID:
			vcom_Send("7");
		break;
		
		case INST_80_ID:
			vcom_Send("8");
		break;
		
		case INST_90_ID:
			vcom_Send("9");
		break;
				
		case INST_01_ID: 	/* Programación de tiempo de respuesta del medidor a comandos enviados por scaling board*/
			NIC_INST_01_excecute();
		break;

		case INST_02_ID:	/* Programacion de frecuencia de reporteo */
			NIC_INST_02_excecute();
		break;
		
		case INST_03_ID:	/* Error de mensaje */
		NIC_INST_03_excecute();
		break;
		
		case INST_04_ID:	/* Reinicio de la tarjeta de escalamiento */
			NIC_INST_04_excecute();
		break;

		case INST_05_ID:	/* Interrupcions de energía */
			NIC_INST_05_excecute();
		break;

		case INST_06_ID:	/* Valores programados */
			NIC_INST_06_excecute();
		break;

		case INST_D0_ID:	/* Error nula comunicacion */
			NIC_INST_D0_excecute();
		break;
		
		case INST_C0_ID:  /* Numero de medidor erroneo */
			NIC_INST_C0_excecute();
		break;

		case INST_B0_ID:  /* Error de producción */
			NIC_INST_B0_excecute();
		break;
		
		default:
			DL_err_mensaje(); /*Error si la instruccion no corresponde con ninguna anterior*/
		break;	
		
			}
		}
 /**
 ******************************************************************************
 * \fn void NIC_INST_10_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_10_excecute(void)
	{
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_METER;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	/**
 ******************************************************************************
 * \fn void NIC_INST_20_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
 	void NIC_INST_20_excecute(void)
	{
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_METER;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	/**
 ******************************************************************************
 * \fn void NIC_INST_30_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_30_excecute(void)
	{
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_METER;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}	
	/**
 ******************************************************************************
 * \fn void NIC_INST_5_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_50_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_METER;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
		/**
 ******************************************************************************
 * \fn void NIC_INST_60_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_60_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_METER;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}

	/*
 ******************************************************************************
 * \fn void NIC_INST_01_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_01_excecute(void)
	{
		/*Disable downlink message */
		nic_apl.IsEnable_dl_msj = false; 
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_SINAMED;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	
/*
 ******************************************************************************
 * \fn void NIC_INST_02_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
	void NIC_INST_02_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_SINAMED;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}

/*
 ******************************************************************************
 * \fn void NIC_INST_04_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
	void NIC_INST_04_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_SINAMED;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	
/*
 ******************************************************************************
 * \fn void NIC_INST_05_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
	void NIC_INST_05_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_SINAMED;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
/*
 ******************************************************************************
 * \fn void NIC_INST_06_excecute(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
*/
	void NIC_INST_06_excecute(void)
	{
		/* Disable downlink message*/
		nic_apl.IsEnable_dl_msj = false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_TO_SINAMED;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	/**
 ******************************************************************************
 * \fn void NIC_INST_D0_excecute(void)
 * \brief This error it's enable when meter do not any answer.
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_D0_excecute(void)
	{
		/* Disable downlink message */
		nic_apl.IsEnable_dl_msj =false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_ERR_MSJ;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	/**
 ******************************************************************************
 * \fn void NIC_INST_C0_excecute(void)
 * \brief This error it's enable when meter id it is diferent between sinamed and physical meter
	
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_C0_excecute(void)
	{
		/* Disable downlink message */
		nic_apl.IsEnable_dl_msj =false;
		/*Direction of instruction*/
		nic_apl.dir_instruction = SEND_ERR_MSJ;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	/**
 ******************************************************************************
 * \fn void NIC_INST_B0_excecute(void)
 * \brief This error it's enable when meter answer something different that appendix A
	
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_B0_excecute(void)
	{
		/* Disable downlink message */
		nic_apl.IsEnable_dl_msj =false;
		/* set up direction of instruction*/
		nic_apl.dir_instruction = SEND_ERR_MSJ;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
	
	/**
 ******************************************************************************
 * \fn void NIC_INST_03_excecute(void)
 * \brief This error it's enable when meter answer something different that appendix A
	
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_INST_03_excecute(void)
	{
		/* Disable downlink message */
		nic_apl.IsEnable_dl_msj =false;
		/* set up direction of instruction*/
		nic_apl.dir_instruction = SEND_ERR_MSJ;
		/*execute the instruction*/
		NIC_run_instruction(&nic_apl);
	}
 /**
 ******************************************************************************
 * \fn void NIC_execute_instruction(NIC_APL_TYPE *nic)
 * \brief 
	
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
	void NIC_run_instruction(NIC_APL_TYPE *nic)
	{
		if(nic->dir_instruction == SEND_TO_METER)
		{
			/*Check commands needed to complete the instruction */
			if(nic_apl.IsInst_exe_success == true)
			{
				/* Reset comand execute succesfully flag */
				nic_apl.IsInst_exe_success = false;
				/* Enable UPLINK Task */
				SCH_SetTask(UL_DATA_SFX);
			}
			else
			{
				/* Keep execute comands*/
				CM_execute_instruction();
			}
		}
		else if(nic->dir_instruction == SEND_TO_SINAMED)
		{
				/* Check commands needed to complete the instruction */
			if(nic_apl.IsInst_exe_success == true)
			{
				/* Reset comand execute succesfully flag */
				nic_apl.IsInst_exe_success = false;
				
				/* Enable UPLINK Task */
				SCH_SetTask(UL_DATA_SFX);
			}
			else
			{
				CT_execute_instruction();
			}
			
		}
		else if(nic->dir_instruction == SEND_ERR_MSJ)
		{
			/* Enable UPLINK Task*/
			SCH_SetTask(UL_DATA_SFX);
		}
	}
	/**
 ******************************************************************************
 * \fn void NIC_execute_instruction(NIC_APL_TYPE *nic)
 * \brief 
	
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
 /**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
