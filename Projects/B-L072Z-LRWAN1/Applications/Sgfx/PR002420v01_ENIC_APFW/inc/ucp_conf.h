/**
 ******************************************************************************
 * \file ucp_conf.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologias EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologias EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup UART Communication Protocol Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all exportable functions for Timer Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE GWH00-79. Enero 2020.
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
#ifndef  __UCP_CONF_H
#define  __UCP_CONF_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
 #include "mlm32l0xx_hw_conf.h"


 
  /*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */
 
 //#define VCOM
 //#define USART1_CUSTOM_CONF
 
 /*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

#ifdef VCOM

#else

	#define UARTX                           USART1
	#define UARTX_CLK_ENABLE()              __USART1_CLK_ENABLE();
	#define UARTX_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
	#define UARTX_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

	#define UARTX_FORCE_RESET()             __USART1_FORCE_RESET()
	#define UARTX_RELEASE_RESET()           __USART1_RELEASE_RESET()


	#define UARTX_TX_PIN                  GPIO_PIN_9
	#define UARTX_TX_GPIO_PORT            GPIOA  
	#define UARTX_TX_AF                   GPIO_AF4_USART1
	#define UARTX_RX_PIN                  GPIO_PIN_10
	#define UARTX_RX_GPIO_PORT            GPIOA 
	#define UARTX_RX_AF                   GPIO_AF4_USART1

	/* Definition for USARTx's NVIC */
	#define UARTX_IRQn                      USART1_IRQn
	#define UARTX_IRQHandler                USART1_IRQHandler
	
#endif

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */
 
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

  /**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
 #endif
