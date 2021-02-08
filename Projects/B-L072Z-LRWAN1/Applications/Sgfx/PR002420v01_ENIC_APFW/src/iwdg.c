/**
 ******************************************************************************
 * \file iwdg.c
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
 #include "stm32l0xx_hal.h"
 #include "iwdg.h"
 #include "stm32l0xx_hal_iwdg.h"
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
 IWDG_HandleTypeDef iwdg_handle;
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
  * \fn void IWDG_initialize(void)
  * \brief Initialize the IWDG
  * 
  * \param void
  * \retval void
  ******************************************************************************
  */
  
  void IWDG_initialize(void)
  {
    IWDG_io_init(); 
    IWDG_io_config();
  }
  
   /**
  ******************************************************************************
  * \fn void IWDG_io_init(void)
  * \brief Initialize the IO IWDG
  * 
  * \param void
  * \retval void
  ******************************************************************************
  */
  void IWDG_io_init(void)
  {
    
  }

  /**
  ******************************************************************************
  * \fn void IWDG_io_config(void)
  * \brief Configure the IO IWDG
  * 
  * \param void
  * \retval void
  ******************************************************************************
  */
  void IWDG_io_config(void)
  {
    iwdg_handle.Instance = IWDG;
    iwdg_handle.Init.Prescaler = IWDG_PRESCALER_128;
    iwdg_handle.Init.Reload = 512;
		iwdg_handle.Init.Window = 500;
    HAL_IWDG_Init(&iwdg_handle);
  }
  
  /**
  ******************************************************************************
  * \fn void IWDG_start(void)
  * \brief Starts the WDG
  * v   
  * \param void
  * \retval void
  ******************************************************************************
  */
  
  void IWDG_start(void)
  {
   HAL_IWDG_Start(&iwdg_handle);
  }
    /**
  ******************************************************************************
  * \fn void IWDG_refresh(void)
  * \brief Refresh the WDG
  * 
  * \param void
  * \retval void
  ******************************************************************************
  */
  void IWDG_refresh(void)
  {
  HAL_IWDG_Refresh(&iwdg_handle);
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
