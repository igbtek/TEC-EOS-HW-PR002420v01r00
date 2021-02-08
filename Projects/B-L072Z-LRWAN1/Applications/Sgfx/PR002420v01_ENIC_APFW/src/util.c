/******************************************************************************
 * @file    util.h
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    30-April-2018
 * @brief   manages virtual com port
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

#include "util.h"

uint16_t UTIL_get_buffer_size(char* buffer){
	volatile uint16_t buff_size;
	buff_size = (unsigned int)sizeof(buffer);
	return buff_size;
}

/**
 ******************************************************************************
 * \file    crc.c
 * \brief   CRC function library
 * \author  Ernesto Cocoletzi
 * \version 04
 * \date    2019-05-28
 *
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2019 by EOS TECH S.A. de C.V. All rights reserved.</b><br><br>
 * EOS TECH Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup Main Function Library
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all standard code file functions for crc.c using stm32f0xx core.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 *   - - SIMCEGAB - Sistema de Medicion de Energia Electrica Centralizado en Gabinete.
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *	 - CFE: Especificacion CFE G0100-05. Marzo 2010.
 *
 *   - Applicable Standards:
 *     - Elster: Communication Protocol for Option Boards. Rev B.
 *     - Elster: Third party communication board design specification. Rev D.
 *     - Elster: Communications Interface: Third-Party Accessible Tables and Functions. Rev A.
 *     - Elster: MF-262 Table. Rev 03 [2012-10-01].
 *		 - ANSI: C1218-2006
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F_P06-DP_13
 *   - Revision: 03
 *   - Date: 2019-05-01
 *   - Author: Mariano Centeno Camarena.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
 
  #include "util.h"

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

/*===========================================================================*\
 * Brief description for each structure and typedefs
\*===========================================================================*/

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
 * \fn uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 16-bit CRC of the specified data.
 *
 *                                              16   12   5
 *        This is the CCITT CRC 16 polynomial X  + X  + X  + 1.
 *        This works out to be 0x1021, but the way the algorithm works
 *        lets us use 0x8408 (the reverse of the bit pattern).  The high
 *        bit is always assumed to be set, thus we only use 16 bits to
 *        represent the 17 bit value.
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc12 The CRC of the data
 ******************************************************************************
 */
uint16_t CRC_crc16(const uint8_t *pData, uint16_t num_bytes)
{  
  static volatile uint16_t crc; 
  static volatile uint8_t i, byte, temp;
   
   crc = 0xffff;    // Reset crc
   do 
   { 
      for (i = 0, byte = *pData++; i < 8; i++, byte >>= 1) 
      { 
          temp = (uint8_t) ((uint8_t) crc & (uint8_t) 0x01); 
          crc >>= 1; 
          if (temp ^ (byte & 0x01)) 
          crc ^= 0x8408;
      }
   } 
   while (--num_bytes); 
  crc = ~((crc << 8) | (crc >> 8));
  return crc;
}
 
/**
 ******************************************************************************
 * \fn uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes)
 * \brief Computes the 2's complement CRC of the specified data.
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval crc The CRC of the data
 ******************************************************************************
 */
uint8_t CRC_2s_complement(const uint8_t *pData, uint16_t num_bytes)
{  
    static volatile uint8_t crc; 
    static volatile uint16_t i;
	
    crc = 0;    // Reset crc

    for (i = 0; i < num_bytes; i++) 
    { 
        crc += *pData++;
    }

	crc = 0xFF - crc + 1;
	return crc;
}

/**
 ******************************************************************************
 * \fn void ascii_to_hex(uint32_t *Dst,const uint8_t *pData, uint16_t num_bytes)
 * \brief Convert ans string of 6 bytes in array of uitn32_t 
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval hex_val return the value of convertion. 
 ******************************************************************************
 */

void ascii_to_hex(uint32_t *pDst,const uint8_t *pData, uint16_t num_bytes)
{  
	uint32_t coef [6] ={100000,10000,1000,100,10,1};
  volatile uint8_t i;
	
    *pDst = 0;
    for (i = 0; i < num_bytes; i++) 
    { 
        *pDst += ((*pData++)-(0x30))*(coef[i]);
    }

}

/**
 ******************************************************************************
 * \fn void ascii_to_hex10(uint32_t *Dst,const uint8_t *pData, uint16_t num_bytes)
 * \brief Convert ans string of 6 bytes in array of uitn32_t 
 *
 * \param pData Pointer to data
 * \param num_bytes The length of the data
 * \retval hex_val return the value of convertion. 
 ******************************************************************************
 */

void ascii_to_hex10(uint32_t *pDst,const uint8_t *pData, uint16_t num_bytes)
{  
	uint32_t coef [10] ={1000000000,100000000,10000000,1000000,100000,10000,1000,100,10,1};
  volatile uint8_t i;
	
    *pDst = 0;
    for (i = 0; i < num_bytes; i++) 
    { 
        *pDst += ((*pData++)-(0x30))*(coef[i]);
    }
}

/** 
 ******************************************************************************
 * \fn bool UTIL_array_compare(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length)
 * \brief 
 *
 * \param	
 *			
 * \retval 	
 ******************************************************************************
 */
bool UTIL_compare_array(uint8_t *array_1_ptr, uint8_t *array_2_ptr, uint8_t array_length)
{
	bool compare_ok = false;
	
	for (uint8_t i = 0; i < array_length; i++)
	{
		if ( *array_1_ptr == *array_2_ptr)
		{
			array_1_ptr++;
			array_2_ptr++;
			if ( i == (array_length - 1) )
			{
				compare_ok = true;
			}
		}
		else
		{
			i = array_length;
		}
	}
	
	return compare_ok;
}

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
