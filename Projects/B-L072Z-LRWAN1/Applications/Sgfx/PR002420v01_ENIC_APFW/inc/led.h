/**
 ******************************************************************************
 * \file led.h
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2020 by Tecnologías EOS S.A. de C.V. All rights reserved.</b><br><br>
 * Tecnologías EOS Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup File content title
 * @section DESCRIPTION DESCRIPTION:
 *
 * File content description.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F_P06-DP_14
 *   - Revision: 08.
 *   - Date: 31/AUG/2020.
 *   - Author: Ernesto Cocoletzi.
 *   - Aprove: Mariano Centeno Camarena.
 ******************************************************************************
*/

#ifndef  LED_H
#define  LED_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
 #include "stm32l072xx.h"
 #include "utilities.h"
 #include "timer.h"
 #include "timeServer.h"
 
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
*/



#define LED_ON_PERIOD_FAIL				TIMER_ms_to_tic(1000)
#define LED_ON_PERIOD							TIMER_ms_to_tic(100)
#define LED_OFF_PERIOD						TIMER_ms_to_tic(900)

#define LED_RTOS_blink_off_time			925
#define LED_RTOS_blink_on_time			75
																							
																							
/*===========================================================================*\
 * Brief description for each block of constant definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
*/

/*===========================================================================*\
 * Brief description for each block of Macro definitions
\*===========================================================================*/

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
*/

/*===========================================================================*\
 * Meter State Types
\*===========================================================================*/
typedef enum {
	
	DEVICE_INITIALIZING = 0,
	DEVICE_RUNNING,
	DEVICE_BUSY_HOST,
	DEVICE_BUSY_SGFX,
	DEVICE_MFG_MODE,
	DEVICE_NBR_STATES,
	
} DEVICE_SYSTEM_STATE_TYPE;

typedef struct{
	
	DEVICE_SYSTEM_STATE_TYPE system_state;
	
}DEVICE_TYPE;

extern DEVICE_TYPE device;

typedef enum 
{
  LED1 = 0,
  LED_GREEN = LED1,
  LED2 = 1,
  LED_RED1 =LED2,
  LED3 = 2,
  LED_BLUE =LED3,
  LED4 = 3,
  LED_RED2 =LED4,
	LED_ALL = 5
} Led_TypeDef;

typedef struct led_state{
	
	bool turn_on;
	
}LED_STATE;

extern LED_STATE led;

#define LEDn                               4

#define LED1_PIN                           GPIO_PIN_5
#define LED1_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED2_PIN                           GPIO_PIN_5
#define LED2_GPIO_PORT                     GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LED3_PIN                           GPIO_PIN_6
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED4_PIN                           GPIO_PIN_7
#define LED4_GPIO_PORT                     GPIOB
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case LED1: \
                                                    LED1_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED2: \
                                                    LED2_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED3: \
                                                    LED3_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  case LED4: \
                                                    LED4_GPIO_CLK_ENABLE();   \
                                                    break;\
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)   do { \
                                                switch( __INDEX__ ) \
                                                {\
                                                  case LED1: \
                                                    LED1_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED2: \
                                                    LED2_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED3: \
                                                    LED3_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  case LED4: \
                                                    LED4_GPIO_CLK_DISABLE();   \
                                                    break;\
                                                  default:\
                                                    break;\
                                                }\
                                              } while(0)

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */
extern TimerEvent_t LED_blink_task;
/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
*/

extern void LED_initialize(void);
extern void LED_io_initialize(void);
extern void LED_io_configure(void);
extern void LED_turn_on(Led_TypeDef led_color);
extern void LED_turn_off(Led_TypeDef led_color);
extern void LED_start_diagnosis_timer(void);
extern void LED_test_rgb(void);
extern void LED_diagnosis_led_task(void);
//extern void LED_turn_off_all_delay_diagnosis_led_task(void);
extern void LED_turn_off_all_delay_diagnosis_task(void);

/*
 ******************************************************************************
 * End of file
 ******************************************************************************
 */
#endif
