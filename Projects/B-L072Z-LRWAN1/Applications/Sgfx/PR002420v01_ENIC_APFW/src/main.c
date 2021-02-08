/******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    30-April-2018
 * @brief   this is the main!
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other 
 *    contributors to this software may be used to endorse or promote products 
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this 
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under 
 *    this license is void and will automatically terminate your rights under 
 *    this license. 
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
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
#include "gwh0079.h"
#include "dl.h"
#include "rep.h"
#include "system.h"
#include "cm.h"
#include "ct.h"
#include "eeprom.h"
#include "mfg.h"
#include "ucp.h"
#include "iwdg.h"
#include "ul.h"
#include "system.h"
#include "timeServer.h"
#include "Legacy/stm32_hal_legacy.h"
#include "stm32l0xx_hal_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* #define STDBY_ON */
#define USER_BUTTON_ALT_PIN                         GPIO_PIN_0
#define USER_BUTTON_ALT_GPIO_PORT                   GPIOA
#define PAC_LEN 8
#define ID_LEN 4
/* Default Configuration zone*/
/* Can be chaged according to application zone*/
#define APPLI_RC   ST_RC2
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
sfx_u8 error = 0;
uint8_t err_id;

/* Private functions ---------------------------------------------------------*/
/*!
 * @brief Open the sigfox library
 * @param The Region configuration
 * @retval None
 */
static sfx_error_t st_sigfox_open( st_sfx_rc_t SgfxRc );
//static uint16_t eeprom_resets_counter(void);

/*!
 * @brief send data to back end sigfox server
 * @param None
 * @retval None
 */
static void send_data( void );

/*!
 * @brief to post interrupt to backgroud.
 * @Note managed by scheduler
 * @param None
 * @retval None
 */
#ifndef STDBY_ON 
//static void send_data_request( void );
#endif

/*!
 * @brief Initialize the user btton to request sending data
 * @param None
 * @retval None
 */
#ifndef STDBY_ON 
/* when STDBY_ON the reset button is used instead of the push button */
//static void user_button_init( void );
#endif

/**
 ******************************************************************************
 * \fn int main( void )
 * \brief Main Function
 *
 * \param void
 * \retval 
 ******************************************************************************
 */
int main( void )
{
	uint16_t reset_counter;
	sfx_error_t error;
	
	st_sfx_rc_t SgfxRc = APPLI_RC;

	SYSTEM_initialize();
	
	/* Counter of reset board */
	SYS_pfail_counter();
	
	/* OPen Sifox Lib */
	error=st_sigfox_open( SgfxRc);

	/* use private key*/
	HW_EEPROM_WRITE( E2pData.SgfxKey, CREDENTIALS_KEY_PRIVATE);
	LPM_SetOffMode(LPM_APPLI_Id, LPM_Disable);

	/*Task of Reporter Programed */
	SCH_RegTask( REP_MGR, REP_main_managger );

	/*Task to scheduler the function of the board*/
	SCH_RegTask( NIC_MGR, NIC_apl_mgr );

	/*Task to analize data received from meter*/
	SCH_RegTask( CM_DATA_RX, CM_rx_data_analize);

	/*Task to send data to sigfox*/
	SCH_RegTask( UL_DATA_SFX, UL_send_to_sfx);

	/*Task to receive data from sigfox*/
	SCH_RegTask( DL_DATA_SFX, DL_task);
	
	/* main loop*/
	while(1)
	{
		SCH_Run();
	}
}
/**
 ******************************************************************************
 * \fn void SCH_Idle( void )
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void SCH_Idle( void )
{
  BACKUP_PRIMASK();

  DISABLE_IRQ( );

  //LPM_EnterLowPower( );		//DEBUG_EC Disable EnterLowPower Mode

  RESTORE_PRIMASK( );
}
sfx_error_t st_sigfox_open( st_sfx_rc_t SgfxRc )
{
  sfx_error_t error = SFX_ERR_NONE;

  /*record RCZ*/
  switch(SgfxRc.id)
  {
    case RC1_ID:
    {      
      error = SIGFOX_API_open(&SgfxRc.param);

      break;
    }
    case RC2_ID:
    {

      sfx_u32 config_words[3] = {RC2_SET_STD_CONFIG_SM_WORD_0, RC2_SET_STD_CONFIG_SM_WORD_1, RC2_SET_STD_CONFIG_SM_WORD_2 };

      error = SIGFOX_API_open(&SgfxRc.param );

      if ( error == SFX_ERR_NONE )
      {
        error = SIGFOX_API_set_std_config(  config_words, RC2_SET_STD_TIMER_ENABLE);
      }

      break;
    }
    case RC3C_ID:
    {
      sfx_u32 config_words[3] = {0x00000003,0x00001388,0x00000000};

      error = SIGFOX_API_open(&SgfxRc.param );

      if ( error == SFX_ERR_NONE )
      {
        error = SIGFOX_API_set_std_config( config_words, NA);
      }

      break;
    }
    case RC4_ID:
    {
      sfx_u32 config_words[3] = {RC4_SET_STD_CONFIG_SM_WORD_0, RC4_SET_STD_CONFIG_SM_WORD_1, RC4_SET_STD_CONFIG_SM_WORD_2 };

      error = SIGFOX_API_open(&SgfxRc.param );

      if ( error == SFX_ERR_NONE )
      {
        error = SIGFOX_API_set_std_config( config_words, RC4_SET_STD_TIMER_ENABLE);
      }

      break;
    }
    default:
    {
      error = SFX_ERR_API_OPEN;
      break;
    }
  }
  return error;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
