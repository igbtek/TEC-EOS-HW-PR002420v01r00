/**
  ******************************************************************************
  * @file    b-l072z-lrwan1.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    15-December-2017
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on B-L072Z-LRWAN1 Discovery Kit 
  *            from STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "b-l072z-lrwan1.h"
#include <stdlib.h>
#include "timer.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup B-L072Z-LRWAN1
  * @{
  */   
    
/** @addtogroup B-L072Z-LRWAN1_LOW_LEVEL 
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on B-L072Z-LRWAN1 Discovery Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Private_TypesDefinitions 
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Private_Defines 
  * @{
  */ 

/**
  * @brief 32L082MLM DISCO BSP Driver version number V1.0.0
  */
#define __B_L072Z_LRWAN1_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __B_L072Z_LRWAN1_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __B_L072Z_LRWAN1_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __B_L072Z_LRWAN1_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __B_L072Z_LRWAN1_BSP_VERSION         ((__B_L072Z_LRWAN1_BSP_VERSION_MAIN << 24)\
                                             |(__B_L072Z_LRWAN1_BSP_VERSION_SUB1 << 16)\
                                             |(__B_L072Z_LRWAN1_BSP_VERSION_SUB2 << 8 )\
                                             |(__B_L072Z_LRWAN1_BSP_VERSION_RC))



/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Private_Variables
  * @{
  */ 
//GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
//const uint16_t LED_PIN[LEDn] = {LED1_PIN, LED2_PIN,LED3_PIN, LED4_PIN};
                                
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT }; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY_BUTTON_PIN }; 
const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn };

/**
  * @}
  */ 
  

/** @defgroup B-L072Z-LRWAN1_LOW_LEVEL_Private_Functions
  * @{
  */ 
  /**
  * @}
  */
/**
  * @brief  This method returns the B-L072Z-LRWAN1 BSP Driver revision
  * @param  None
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __B_L072Z_LRWAN1_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *            @arg  LED2
  * @retval None
  */
//void BSP_LED_Init(Led_TypeDef Led)
//{
//  GPIO_InitTypeDef  GPIO_InitStruct;
//  
//  /* Enable the GPIO_LED Clock */
//  LEDx_GPIO_CLK_ENABLE( Led );

//  /* Configure the GPIO_LED pin */
//  GPIO_InitStruct.Pin = LED_PIN[Led];
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  
//  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStruct);
//}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *            @arg  LED2
  * @retval None
  */
//void BSP_LED_On(Led_TypeDef Led)
//{
//  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET); 
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_on_green(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_on_green(void)
//{
//	BSP_LED_On(LED_GREEN);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_on_red1(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_on_red1(void)
//{
//	BSP_LED_On(LED_RED1);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_on_blue(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_on_blue(void)
//{
//	BSP_LED_On(LED_BLUE);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_on_red1(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_on_red2(void)
//{
//	BSP_LED_On(LED_RED2);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_off_green(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_off_green(void)
//{
//	BSP_LED_Off(LED_GREEN);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_off_red1(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_off_red1(void)
//{
//	BSP_LED_Off(LED_RED1);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_off_blue(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_off_blue(void)
//{
//	BSP_LED_Off(LED_BLUE);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_off_red2(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
//void BSP_LED_off_red2(void)
//{
//	BSP_LED_Off(LED_RED2);
//}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_on_led_green(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_on_led_green(void)
{
	//BSP_LED_on_green();
	//TIMER_start_ms_timer(LED_BLINKING_ON_GREEN_TIMER, TIMER_ms_to_tic(100), BSP_LED_blink_off_led_green);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_on_led_red1(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_on_led_red1(void)
{
	//BSP_LED_on_red1();
	//TIMER_start_ms_timer(LED_BLINKING_ON_RED1_TIMER, TIMER_ms_to_tic(200), BSP_LED_blink_off_led_red1);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_on_led_blue(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_on_led_blue(void)
{
	//BSP_LED_on_blue();
	//TIMER_start_ms_timer(LED_BLINKING_ON_BLUE_TIMER, TIMER_ms_to_tic(100), BSP_LED_blink_off_led_blue);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_on_led_red2(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_on_led_red2(void)
{
	//BSP_LED_on_red2();
	//TIMER_start_ms_timer(LED_BLINKING_ON_RED2_TIMER, TIMER_ms_to_tic(100), BSP_LED_blink_off_led_red2);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_off_led_green(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_off_led_green(void)
{
	//BSP_LED_off_green();
	TIMER_start_ms_timer(LED_BLINKING_OFF_GREEN_TIMER, TIMER_ms_to_tic(1000), BSP_LED_blink_on_led_green);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_off_led_red1(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_off_led_red1(void)
{
	//BSP_LED_off_red1();
	TIMER_start_ms_timer(LED_BLINKING_OFF_RED1_TIMER, TIMER_ms_to_tic(1100), BSP_LED_blink_on_led_red1);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_off_led_blue(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_off_led_blue(void)
{
	//BSP_LED_off_blue();
	TIMER_start_ms_timer(LED_BLINKING_OFF_BLUE_TIMER, TIMER_ms_to_tic(1200), BSP_LED_blink_on_led_blue);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_off_led_red2(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_off_led_red2(void)
{
	//BSP_LED_off_red2();
	TIMER_start_ms_timer(LED_BLINKING_OFF_RED2_TIMER, TIMER_ms_to_tic(1300), BSP_LED_blink_on_led_red2);
}

/**
 ******************************************************************************
 * \fn void BSP_LED_blink_stop_led_blue(void)
 * \brief void
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void BSP_LED_blink_stop_led_blue(void)
{
	TIMER_reset_ms_timer(LED_BLINKING_ON_BLUE_TIMER);
	TIMER_reset_ms_timer(LED_BLINKING_OFF_BLUE_TIMER);
	//BSP_LED_off_blue();
}

/**
  * @brief  Turns selected LED Off. 
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *            @arg  LED2
  * @retval None
  */
//void BSP_LED_Off(Led_TypeDef Led)
//{
//  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET); 
//}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *            @arg  LED2
  * @retval None
  */
//void BSP_LED_Toggle(Led_TypeDef Led)
//{
//  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
//}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
  }
  
  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; 
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x03);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */    

/**
  * @}
  */ 
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
