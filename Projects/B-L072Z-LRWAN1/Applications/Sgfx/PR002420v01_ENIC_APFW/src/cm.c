/**
 ******************************************************************************
 * \file cm.c
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

#include "cm.h"

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
CM_TYPE cm;
FRAME_FACTORY_TYPE ff;


/*timers */
TimerEvent_t CM_TIMEOUT_TIMER;
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
 * \fn void CM_execute_instruction(CM_TYPE *cmd)
 * \brief void
 *
 * \param CM_TYPE *cmd
 * \retval bool
 ******************************************************************************
 */
void CM_execute_instruction(void)
{
	/*Reset retry timer*/
		TimerReset(&CM_TIMEOUT_TIMER);

	if(cm.IsOn_going_execution == false)
	{
		CM_frame_factory();
	}
	/*Set module in ongoing transmition in true*/
		cm.IsOn_going_execution = true;

	switch(cm.cm_executed)
		{
			case SEND_FRAME_1:
			/*Set number of bytes expected to this frame*/
			cm.cm_rx_bytes_expeccted =cm.bf.bytes_exp_frame_1;
			/*Send frame into frame_1 array */
			ucp_dll.trigger_task = true;
			ucp_dll.uart_task = CM_DATA_RX;
			ucp_dll.uart_fprt = CM_rx_data_analize;
			UCP_send_frame((uint8_t*)&cm.bf.frame_1,cm.bf.size_frame_1);
			/* Start timer  */
			TimerStart(&CM_TIMEOUT_TIMER);
			
			break;
			
			case SEND_FRAME_2:
			/*Set number of bytes expected to this frame*/
			cm.cm_rx_bytes_expeccted =cm.bf.bytes_exp_frame_2;
			/*Send frame into frame_1 array */
			ucp_dll.trigger_task = true;
			ucp_dll.uart_task = CM_DATA_RX;
			ucp_dll.uart_fprt = CM_rx_data_analize;
			UCP_send_frame ((uint8_t*)&cm.bf.frame_2,cm.bf.size_frame_2);
			/* Start timer  */
			TimerStart(&CM_TIMEOUT_TIMER);		
			break;
			
			case SEND_FRAME_3:
			/*Set number of bytes expected to this frame*/
			cm.cm_rx_bytes_expeccted =cm.bf.bytes_exp_frame_3;
			/*Send frame into frame_1 array */
			ucp_dll.trigger_task = true;
			ucp_dll.uart_task = CM_DATA_RX;
			ucp_dll.uart_fprt = CM_rx_data_analize;
			UCP_send_frame ((uint8_t*)&cm.bf.frame_3,cm.bf.size_frame_3);
			/* Start timer  */
			TimerStart(&CM_TIMEOUT_TIMER);	
			break;
			
			default:
			break;
		}
}
	/**
 ******************************************************************************
 * \fn void CM_frame_factory(void)
 * \brief Execute a retry after   
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_frame_factory(void)
{

	switch(nic_apl.technology)
	{
	case MED_1_PH_GHW00_09:
	break;
	
	case MED_2_PH_GHW00_09:		
	break;
	
	case MED_3_PH_GHW00_09:		
	break;
	
	case MED_1_PH_GHW00_34:
		CM_frame_factory_for_1ph_ghw0034();
	break;
	
	case MED_2_PH_GHW00_34:

	break;
	
	default:
	break;
	}
}
   /**
 ******************************************************************************
 * \fn void CM_frame_factory_for_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_frame_factory_for_1ph_ghw0034(void)
{
		/*Set the frame paratmeters to do the Instruction*/
		switch(nic_apl.instruction)
		{
		case INST_10_ID:
			CM_comands_to_meter_inst_10_1ph_ghw0034();
		break;
				
		case INST_20_ID:	
			CM_comands_to_meter_inst_20_1ph_ghw0034();
		break;
				
		case INST_30_ID:	/*--- valores instantaneos fase A---*/
			CM_comands_to_meter_inst_30_1ph_ghw0034();
		break;

		case INST_40_ID:	
			CM_comands_to_meter_inst_40_1ph_ghw0034();			
		break;

		case INST_50_ID:		/*--- Apertura de Relevador ---*/
			CM_comands_to_meter_inst_50_1ph_ghw0034();
		break;

		case INST_60_ID:		/*--- Cierre de Relevador ---*/
				
			CM_comands_to_meter_inst_60_1ph_ghw0034();
		break;
				
		default:
		break;	
		}
		
		/*Verify the number of comands set up IsMulti_comand Flag*/
		if(cm.bf.nbr_cmds != 1)
		{
			cm.IsMulti_comand = true;
		}
		else
		{
			cm.IsMulti_comand = false;
		}
}

   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_10_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_10_1ph_ghw0034(void)
{
		/*Copy frame of intstructions to array bytes*/
		memcpy(&cm.bf.frame_1,MEDIDOR,MEDIDOR_SIZE);
		/*Legth of every frame data to send to meter*/
		cm.bf.size_frame_1 = MEDIDOR_SIZE;
		/*Set up the expexted bytes to received from meter*/
		cm.bf.bytes_exp_frame_1 = EXP_RESP_MEDIDOR;
		/*Number of commands to acompish the instrction*/
		cm.bf.nbr_cmds = MEDIDOR_EXE_CMDS;
}
   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_20_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_20_1ph_ghw0034(void)
{
		/*Copy frame of intstructions to array bytes*/
		memcpy(&cm.bf.frame_1,ENERGIA_R,ENERGIA_R_SIZE);
		memcpy(&cm.bf.frame_2,ENERGIA,ENERGIA_SIZE);
		/*Legth of every frame data to send to meter*/
		cm.bf.size_frame_1 = ENERGIA_R_SIZE;
		cm.bf.size_frame_2 = ENERGIA_SIZE;
		/*Set up the expexted bytes to received from meter*/
		cm.bf.bytes_exp_frame_1 = EXP_RESP_ENERGIA_R;
		cm.bf.bytes_exp_frame_2 = EXP_RESP_ENERGIA;
		/*Number of commands to acompish the instrction*/
		cm.bf.nbr_cmds = LECTURA_ACUMULADA_EXE_CMDS;
}


   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_30_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_30_1ph_ghw0034(void)
{
		/*Copy frame of intstructions to array bytes*/
		memcpy(&cm.bf.frame_1,VOLTAJE_A,VOLTAJE_A_SIZE);	
		memcpy(&cm.bf.frame_2,CORRIENTE_A,CORRIENTE_A_SIZE);
		memcpy(&cm.bf.frame_3,POTENCIA_A, POTENCIA_A_SIZE);
		/*Legth of every frame data to send to meter*/
		cm.bf.size_frame_1 = VOLTAJE_A_SIZE;
		cm.bf.size_frame_2 = CORRIENTE_A_SIZE;
		cm.bf.size_frame_3 = POTENCIA_A_SIZE;
		/*Set up the expexted bytes to received from meter*/
		cm.bf.bytes_exp_frame_1 = EXP_RESP_VOLTAJE_A;
		cm.bf.bytes_exp_frame_2 = EXP_RESP_CORRIENTE_A ;
		cm.bf.bytes_exp_frame_3 = EXP_RESP_POTENCIA_A;
		/*Number of commands to acompish the instrction*/
		cm.bf.nbr_cmds = INSTANTANEOS_PH_A_EXE_CMDS;
}

   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_40_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_40_1ph_ghw0034(void)
{
	/* write parameter of comands to send to meter*/
}
   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_50_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_50_1ph_ghw0034(void)
{
	/* write parameter of comands to send to meter*/
}

   /**
 ******************************************************************************
 * \fn void CM_comands_to_meter_inst_60_1ph_ghw0034(void)
 * \brief This fuction copy the frame necesary from cm.h to do the instrcction 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_comands_to_meter_inst_60_1ph_ghw0034(void)
{
	/* write parameter of comands to send to meter*/
}
  
/*******************************************************************************
****************************RESPONSE FROM METER**********************************
*******************************************************************************/
/******************************************************************************/
   /**
 ******************************************************************************
 * \fn void check_valid_data(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */

void CM_rx_data_analize(void)
{
	/* This validation ensure that only receive data when is execiting an instruction*/
	ucp_dll.trigger_task = false;
	if(cm.IsOn_going_execution == true)
	{
	/* DELETE THE CARRIER RETURN BYTE TO BUFFER COUNTER*/
			ucp_dll.rx_buffer_bytes = ucp_dll.rx_buffer_bytes - 1;
	
	/* CHECK IF THE BYTES RECEIVED  FROM METER ITS  EQUAL TO BYTES EXPECTED*/
		if(ucp_dll.rx_buffer_bytes == cm.cm_rx_bytes_expeccted)	
		{
			/* Stop timer and reset */
			TimerStop(&CM_TIMEOUT_TIMER);
			TimerReset(&CM_TIMEOUT_TIMER);
			/* check if commads to need to complete the instruction it is ok */
			if(cm.IsMulti_comand == true)
			{
				TimerStop(&CM_TIMEOUT_TIMER);
				memcpy(&cm.cm_rx_buffer,&ucp_dll.rx_buffer,ucp_dll.rx_buffer_bytes);
				cm.cm_executed ++;
				CM_build_ul_data();
			}
			else
			{
				memcpy(&cm.cm_rx_buffer,&ucp_dll.rx_buffer,ucp_dll.rx_buffer_bytes);
				cm.cm_executed ++;
				CM_build_ul_data();
			}
		}
		else
		{
			CM_err_communication();
		}
	}
}

 /**
 ******************************************************************************
 * \fn void CM_build_ul_data(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_build_ul_data(void)
{
	/*Accoridng to technology build the frame*/
	switch(nic_apl.technology)
	{
	case MED_1_PH_GHW00_09:
	break;
	
	case MED_2_PH_GHW00_09:		
	break;
	
	case MED_3_PH_GHW00_09:		
	break;
	
	case MED_1_PH_GHW00_34:
		CM_build_ul_data_for_1ph_gwh0034();
	break;
	
	case MED_2_PH_GHW00_34:
	break;
	
	default:
	break;
	}
}
 /**
 ******************************************************************************
 * \fn void CM_build_ul_data_for_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */
void CM_build_ul_data_for_1ph_gwh0034(void)
{
	switch (nic_apl.instruction)
	{
	case INST_10_ID:
		CM_build_ul_frame_10_1ph_gwh0034();
	break;
	
	case INST_20_ID:
		CM_build_ul_frame_20_1ph_gwh0034();
	break;
	
	case INST_30_ID:
		CM_build_ul_frame_30_1ph_gwh0034();
	break;
	
	case INST_40_ID:
	
	break;
	
	case INST_50_ID:
		CM_build_ul_frame_50_1ph_gwh0034();
	break;
	
	case INST_60_ID:
		CM_build_ul_frame_60_1ph_gwh0034();
	break;
	
	default:
	
	break;	
	}
}



 /**
 ******************************************************************************
 * \fn void CM_build_ul_frame_10_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */

void CM_build_ul_frame_10_1ph_gwh0034(void)
{
	uint8_t command_test[EXP_RESP_MEDIDOR]= {NULL};
	
	/* Set cm_ulbuffer with "0xFF"*/
	memset(&nic_dll.ul_data,0xff,sizeof(nic_dll.ul_data));
	/* Copy the id of meter into nic managger*/
	memcpy(&nic_apl.id_meter[0] ,&ucp_dll.rx_buffer,sizeof(nic_apl.id_meter));
	/* Copy meter ID in eeprom memory*/
	EEPROM_writer(EEPROM_SGFX_ID_METER_ADDR, &nic_apl.id_meter[0], EEPROM_SGFX_ID_METER_SIZE);
	/* Load index from eeprom */
	EEPROM_reader(EEPROM_SGFX_ID_METER_ADDR, &command_test[0], EEPROM_SGFX_ID_METER_SIZE);
	/* Copy data received into cm.cm_ul_buffer to send to ul module*/
	memcpy(&nic_dll.ul_data[3] ,&ucp_dll.rx_buffer,sizeof(nic_dll.ul_data));
	/* Check if all comands has been executed*/
	CM_check_executed_comands();
}
 /**
 ******************************************************************************
 * \fn void CM_build_ul_frame_20_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */
	void CM_build_ul_frame_20_1ph_gwh0034(void)
	{	
		uint8_t buffer_rx[EXP_RESP_ENERGIA]= {NULL};
		uint8_t buffer_to_hex[10]={NULL};
		uint32_t hex_val=0;
		uint8_t i;
		
		nic_dll.ul_data[0]=	0xFF;

		memcpy(&buffer_rx,&ucp_dll.rx_buffer,EXP_RESP_ENERGIA);
		
		switch(cm.cm_executed)
		{
		case RECEIVE_RESPOSE_OF_FRAME_1:
		
		for(i=0;i<12;i++)
		{
			buffer_to_hex[i]=buffer_rx[i+2];
		}
		
		ascii_to_hex10(&hex_val, &buffer_to_hex[0], 10);

		nic_dll.ul_data[1]=	hex_val >> 24;
		nic_dll.ul_data[2]=	hex_val >> 16;
		nic_dll.ul_data[3]=	hex_val >> 8;
		nic_dll.ul_data[4]=	hex_val;
		
		break;
		
		case RECEIVE_RESPOSE_OF_FRAME_2:
			
		for(i=0;i<12;i++)
		{
			buffer_to_hex[i]=buffer_rx[i+2];
		}
		
		ascii_to_hex10(&hex_val, &buffer_to_hex[0], 10);
		
		nic_dll.ul_data[5]=	hex_val >> 24;
		nic_dll.ul_data[6]=	hex_val >> 16;
		nic_dll.ul_data[7]=	hex_val >> 8;
		nic_dll.ul_data[8]=	hex_val;
		
		break;
		
		default:
		break;
		}
		/*check if all comands has been executed*/
		CM_check_executed_comands();
	}
 /**
 ******************************************************************************
 * \fn void CM_build_ul_frame_30_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */
void CM_build_ul_frame_30_1ph_gwh0034(void)
{
		uint32_t other_val =0;
		uint32_t hex_val;
		uint8_t buffer_rx[7]={NULL}; 
		uint8_t buffer_to_hex[6]={NULL};
		uint8_t buffer[3] ={NULL};
		uint8_t i;
		
		memcpy(&buffer_rx,&ucp_dll.rx_buffer,8);
		
		for(i=0;i<7;i++)
		{
			buffer_to_hex[i]=buffer_rx[i+1];
		}
		ascii_to_hex(&hex_val ,&buffer_to_hex[0], 6);
		
		buffer[0] = hex_val >> 16;
		buffer[1] = hex_val >> 8;
		buffer[2] = hex_val;
		/* Prepare UL*/
		switch(cm.cm_executed)
		{
			case RECEIVE_RESPOSE_OF_FRAME_1:
			nic_dll.ul_data[0]=	buffer[0];
			nic_dll.ul_data[1]=	buffer[1];
			nic_dll.ul_data[2]=	buffer[2];
			break;
			
			case RECEIVE_RESPOSE_OF_FRAME_2:
			nic_dll.ul_data[3]=	buffer[0];
			nic_dll.ul_data[4]=	buffer[1];
			nic_dll.ul_data[5]=	buffer[2];
			break;
			
			case RECEIVE_RESPOSE_OF_FRAME_3:
			nic_dll.ul_data[6]=	buffer[0];
			nic_dll.ul_data[7]=	buffer[1];
			nic_dll.ul_data[8]=	buffer[2];
			break;
			
			default:
			break;
		}
		/*check if all comands has been executed*/
		CM_check_executed_comands();
}
 /**
 ******************************************************************************
 * \fn void CM_build_ul_frame_50_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */
void CM_build_ul_frame_50_1ph_gwh0034(void)
{
 /* write code to build uplink answer */
}
 /**
 ******************************************************************************
 * \fn void CM_build_ul_frame_60_1ph_gwh0034(void)
 * \brief void
 *
 * \param void
 * \retval bool
 ******************************************************************************
 */
void CM_build_ul_frame_60_1ph_gwh0034(void)
{
	 /* write code to build uplink answer */
}
 /**
 ******************************************************************************
 * \fn void CM_check_executed_comands(void)
 * \brief 
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_check_executed_comands(void)
{
		/* Check if the number of comands needed to acomplish the instruction is complete */	
	if(cm.cm_executed == cm.bf.nbr_cmds)
	{
		/* Disable ongoing execution flag when is complete the instruction*/
		cm.IsOn_going_execution = false;
		
		/* Enable Instruction execute succesfully */
		nic_apl.IsInst_exe_success = true;
		
		/* Enable Nic Manager Task */
		SCH_SetTask(NIC_MGR);
	}
	else
	{
		/* Recall to send next frame */
		CM_execute_instruction();
	}	
}


 /**
 ******************************************************************************
 * \fn void CM_null_communication(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_null_communication(void)
{
	memcpy(&nic_dll.ul_data,&NULL_COMM, NULL_COM_SIZE);
	
	/* Error instruction*/
	nic_apl.instruction = INST_D0_ID;
	
	/* Enable Nic Manager Task */
	SCH_SetTask(NIC_MGR);
}
 /**
 ******************************************************************************
 * \fn void CM_err_communication(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_err_communication(void)
{
	memcpy(&nic_dll.ul_data,&ERR_COMM, ERR_COM_SIZE);
	
	nic_apl.instruction = INST_B0_ID;
	
	/* Enable Nic Manager Task */
	SCH_SetTask(NIC_MGR);
}
 /**
 ******************************************************************************
 * \fn void CM_init_cmd_timeout_timer(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void CM_init_cmd_timeout_timer(void)
{
	volatile uint16_t period =0;
	uint8_t temp_val[2]={NULL};
	/* Set the calback function */
	TimerInit(&CM_TIMEOUT_TIMER, CM_null_communication);
	
	EEPROM_reader(EEPROM_SGFX_ANS_TIME_FM_ADDR,&temp_val[0] , EEPROM_SGFX_ANS_TIME_FM_SIZE);
	
	if((temp_val[0]==0xFF)||(temp_val[0]==0x00))
	{
		/*Set default values */
		temp_val[0] = 0x88;
		temp_val[1] = 0x13;
		
		EEPROM_writer(EEPROM_SGFX_ANS_TIME_FM_ADDR,&temp_val[0] , EEPROM_SGFX_ANS_TIME_FM_SIZE);
		EEPROM_reader(EEPROM_SGFX_ANS_TIME_FM_ADDR,&temp_val[0] , EEPROM_SGFX_ANS_TIME_FM_SIZE);
	}
	
	period |= temp_val[0] << 8;
	period |= temp_val[1];

	/* Set the time value to shot the timer */
	TimerSetValue(&CM_TIMEOUT_TIMER, period );
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
