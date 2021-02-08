#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32l0xx_hal.h"
#include "string.h"
#include "stdbool.h"
/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*===========================================================================*\
 * Brief description for each Define
\*===========================================================================*/
#define BIT0_P			0
#define BIT1_P			1
#define BIT2_P			2
#define BIT3_P			3
#define BIT4_P			4
#define BIT5_P			5
#define BIT6_P			6
#define BIT7_P			7
#define BIT8_P			8
#define BIT9_P			9
#define BIT10_P			10
#define BIT11_P			11
#define BIT12_P			12
#define BIT13_P			13
#define BIT14_P			14
#define BIT15_P			15

#define BIT0_M			0x0001
#define BIT1_M			0x0002
#define BIT2_M			0x0004
#define BIT3_M			0x0008
#define BIT4_M			0x0010
#define BIT5_M			0x0020
#define BIT6_M			0x0040
#define BIT7_M			0x0080
#define BIT8_M			0x0100
#define BIT9_M			0x0200
#define BIT10_M			0x0400
#define BIT11_M			0x0800
#define BIT12_M			0x1000
#define BIT13_M			0x2000
#define BIT14_M			0x4000
#define BIT15_M			0x8000

#define SHIFT1			1
#define SHIFT2			2
#define SHIFT3			3
#define SHIFT4			4
#define SHIFT5			5
#define SHIFT6			6
#define SHIFT7			7
#define SHIFT8			8

#define HIGH_NIBBLE		0xF0
#define LOW_NIBBLE		0x0F

#define  FALSE			0
#define  TRUE           (!false)

/*===========================================================================*\
 * Exact Width integer types
\*===========================================================================*/
#define UINT8_T_MIN		(0)
#define UINT8_T_MAX		(255)
#define UINT16_T_MIN	        (0)
#define UINT16_T_MAX	        (65535)
#define UINT32_T_MIN	        (0)
#define UINT32_T_MAX	        (4294967295U)

#define INT8_T_MIN		(-128)
#define INT8_T_MAX		(127)
#define INT16_T_MIN		(-32768)
#define INT16_T_MAX		(32767)
#define INT32_T_MIN		(-2147483648)
#define INT32_T_MAX		(2147483647)

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
 * Global Enumerations, Structures and Typedefs                          
 ******************************************************************************
 */
typedef void (*void_fptr) (void);               // void void Function pointer typedef
typedef  void (*pFunction)(void);

typedef struct uint8_t_to_bits                           /* generic type to access uint8_t in BIT size */
{
	uint8_t Bit0 : 1;
	uint8_t Bit1 : 1;
	uint8_t Bit2 : 1;
	uint8_t Bit3 : 1;
	uint8_t Bit4 : 1;
	uint8_t Bit5 : 1;
	uint8_t Bit6 : 1;
	uint8_t Bit7 : 1;
}UINT8_T_TO_BITS;

typedef struct uint16_t_to_bits                           /* generic type to access uint16_t in BIT size */
{
	uint8_t Bit0 : 1;
	uint8_t Bit1 : 1;
	uint8_t Bit2 : 1;
	uint8_t Bit3 : 1;
	uint8_t Bit4 : 1;
	uint8_t Bit5 : 1;
	uint8_t Bit6 : 1;
	uint8_t Bit7 : 1;
	uint8_t Bit8 : 1;
	uint8_t Bit9 : 1;
	uint8_t Bit10 : 1;
	uint8_t Bit11 : 1;
	uint8_t Bit12 : 1;
	uint8_t Bit13 : 1;
	uint8_t Bit14 : 1;
	uint8_t Bit15 : 1;
}UINT16_T_TO_BITS;

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
extern void ascii_to_hex10(uint32_t *pDst,const uint8_t *pData, uint16_t num_bytes);
extern void ascii_to_hex(uint32_t *pDst,const uint8_t *pData, uint16_t num_bytes);
extern bool UTIL_compare_array(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length);

extern uint16_t UTIL_get_buffer_size(char* buffer);
typedef void (*void_fptr) (void);               // void void Function pointer typedef
typedef  void (*pFunction)(void);

extern uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes); 
extern uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes);
/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * ----------------------------------------------------------------------------
 *	- Revision 01 / Mariano Centeno Camarena / 2017_04_13
 *	- Revision 02 / Mariano Centeno Camarena / 2019_05_01
 *  - Change 2:  Description.
 *  - Change 1:  Description.
 ******************************************************************************
 */

#endif /* __VCOM_H__*/
