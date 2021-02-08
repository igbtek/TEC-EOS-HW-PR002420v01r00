/**
 ******************************************************************************
 * \file eeprom.c
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

#include "eeprom.h"
#include "eeprom_cfg.h"
#include "stm32l0xx_hal_gpio.h"
#include "hw.h"
//#include "TimeServer.h"
//#include "twi.h"

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
 * \brief I2C Library Initialization
 *
 * I/O port, Usart and variables initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_initialize(void)
{  
	EEPROM_io_initialize();
	EEPROM_io_configure();
	EEPROM_twi_reconfig_speed();
	
	EEPROM_check_parameters();
	
}

/**
 ******************************************************************************
 * \fn  void EEPROM_io_initialize(void)
 * \brief I2C EEPROM I/O Port Registers Initialization
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_io_initialize(void)
{
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);		//DEBUG_EC_EEPROM_NEW
	//ioport_set_pin_level( EEPROM_WC_PIN, true);		//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn void EEPROM_io_configure(void)
 * \brief I2C I/O Port Registers Configuration
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_io_configure(void)
{	
	//twi_options_t opt;
		
	EEPROM_twi_recovery();
		
	//ioport_init();
	
	// Set WC_PIN as output
	//ioport_set_pin_dir(EEPROM_WC_PIN, IOPORT_DIR_OUTPUT);		//DEBUG_EC_EEPROM
	
	//disable PIO control of pins to enable peripheral control
	EEPROM_twi_enable_peripheral_mode();
	
	/* Configure PMC. */
	//pmc_enable_periph_clk( ID_TWI0 );																//DEBUG_EC_EEPROM
	//PDC_TWI0->PERIPH_PTCR = PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN;	//DEBUG_EC_EEPROM
	
	/* Initialize TWI master. */
	/*opt.master_clk = EEPROM_TWI_MASTER_CLK;
	opt.speed = EEPROM_TWI_STANDARD_MODE_SPEED;
	opt.smbus = 0;
	twi_master_init(EEPROM_TWI, &opt);*/		//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn void EEPROM_twi_recovery(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_twi_recovery(void)
{
	EEPROM_twi_enable_pio_mode();
	
	/*ioport_set_pin_dir(EEPROM_SCL_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(EEPROM_SDA_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(EEPROM_WC_PIN, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(EEPROM_SDA_PIN, true);
	ioport_set_pin_dir(EEPROM_SDA_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(EEPROM_SCL_PIN, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(EEPROM_SDA_PIN, true);
	ioport_set_pin_level(EEPROM_SCL_PIN, true);		
	delay_us(10);
	
	ioport_set_pin_level(EEPROM_SDA_PIN, false);
	delay_us(10);

	ioport_set_pin_level(EEPROM_SCL_PIN, false);
	delay_us(10);*/																							//DEBUG_EC_EEPROM
	
	/*__NOP();
	__NOP();
	
	ioport_set_pin_dir(EEPROM_SDA_PIN, IOPORT_DIR_INPUT);
	
	while(ioport_get_pin_level(EEPROM_SDA_PIN) == false)
	{
		ioport_toggle_pin_level( EEPROM_SCL_PIN);
		delay_us(10);
	}
	
	ioport_set_pin_dir(EEPROM_SDA_PIN, IOPORT_DIR_INPUT);*/		//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn void EEPROM_twi_enable_peripheral_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_twi_enable_peripheral_mode(void)
{
	/*REG_PIOA_PDR |= PIO_PDR_P24;
	REG_PIOA_PDR |= PIO_PDR_P25;
	
	REG_PIOA_PER &= ~PIO_PER_P24;
	REG_PIOA_PER &= ~PIO_PER_P25;*/			//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn void EEPROM_twi_enable_pio_mode(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_twi_enable_pio_mode(void)
{
	/*REG_PIOA_PDR &= ~PIO_PDR_P24;
	REG_PIOA_PDR &= ~PIO_PDR_P25;
	
	REG_PIOA_PER |= PIO_PER_P24;
	REG_PIOA_PER |= PIO_PER_P25;*/		//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn void EEPROM_twi_reconfig_speed(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_twi_reconfig_speed(void)
{
	uint8_t hardware_version = 0;
	/*EEPROM_reader( ST_1_HW_VERSION_NUMBER_ADDR, &hardware_version, 1 );
	
	if ( (hardware_version > 14) && (hardware_version != 0xFF) )
	{
		twi_set_speed(EEPROM_TWI, EEPROM_TWI_FAST_MODE_SPEED, EEPROM_TWI_MASTER_CLK);
	}*/			//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn bool EEPROM_writer(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EEPROM_writer(uint32_t address, uint8_t* data_pointer, unsigned int data_length)
{
	#ifdef USE_INTERNAL_EEPROM
	
		bool is_ok = false;
		unsigned int k = 0;
		
		HAL_DATA_EEPROMEx_Unlock();
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | FLASH_FLAG_FWWERR | FLASH_FLAG_NOTZEROERR);
		
		while (k < data_length){
			
			HAL_DATA_EEPROMEx_Program(FLASH_TYPEPROGRAMDATA_BYTE, (address + k), (uint32_t)*data_pointer);
			k ++;
			data_pointer ++;
			HAL_Delay(1);
		};
		
		HAL_DATA_EEPROMEx_Lock();
		
		is_ok = true;
		return is_ok;
	
	#endif
//	bool write_ok;
//	volatile uint8_t tries;
//	volatile unsigned int i;
//	volatile uint8_t* data = data_pointer;
//	volatile uint8_t temp_data[256];
//	volatile unsigned int wr_bytes;
//	
//	/* --------------------------------------------------------------------- */
//	/* Determine size of first block write                                   */
//	/* --------------------------------------------------------------------- */
//	wr_bytes = 256 - (address & 0x000000FF);			/* get maximum size for first write */
//	
//	if ( data_length < wr_bytes )                       /* sync to page boudary */
//	{
//		wr_bytes = data_length;                         /* yes, then just write the request amount */
//	}
//	
//	/* --------------------------------------------------------------------- */
//	/* Write blocks */
//	/* --------------------------------------------------------------------- */
//	do
//	{		
//		write_ok = true;
//		
//		for ( tries = 0; tries < 5; tries++ )
//		{
// 			// Enable WR pin
// 	 		/*ioport_set_pin_level( EEPROM_WC_PIN, false);
// 	 		ioport_set_pin_dir(EEPROM_WC_PIN, IOPORT_DIR_OUTPUT);*/			//DEBUG_EC_EEPROM
//						
//			// Configure the TWI data
//			/*
//			eeprom_twi_package.chip = EEPROM_TWI_CHIP_ADDR(address);
//			eeprom_twi_package.addr[0] = (uint8_t)(address >> 8);
//			eeprom_twi_package.addr[1] = (uint8_t)(address);
//			eeprom_twi_package.addr_length = 2;
//			eeprom_twi_package.length = wr_bytes; // Data size
//			eeprom_twi_package.buffer = (void*)data_pointer;
//			
//			if (twi_master_write(TWI0, &eeprom_twi_package) != STATUS_OK)
//			{
//				EEPROM_io_configure();
//			}
//			delay_ms(10);
//		
//			// Disable WR pin
//	 		ioport_set_pin_level( EEPROM_WC_PIN, true);
//	 		ioport_set_pin_dir(EEPROM_WC_PIN, IOPORT_DIR_OUTPUT);
//	 				
//			// Configure the TWI data
//			eeprom_twi_package.chip = EEPROM_TWI_CHIP_ADDR(address);
//			eeprom_twi_package.addr[0] = (uint8_t)(address >> 8);
//			eeprom_twi_package.addr[1] = (uint8_t)(address);
//			eeprom_twi_package.addr_length = 2;
//			eeprom_twi_package.length = wr_bytes; // Data size
//			eeprom_twi_package.buffer = (void*) &temp_data;
//			
//			if ( twi_master_read(TWI0, &eeprom_twi_package) != STATUS_OK )		//DEBUG_EC_EEPROM
//			{
//				EEPROM_io_configure();
//			}
//			
//			for ( i = 0; i < wr_bytes; i++)
//			{
//				if ( data[i] != temp_data[i] )
//				{
//					write_ok = false;
//					break;
//				}
//			}

//			if ( write_ok == true )
//			{
//				break;
//			}*/		//DEBUG_EC_EEPROM
//		}
//		
//		//address  += wr_bytes;                                       /* bump the pointers by amount written */	//DEBUG_EC_EEPROM
//		//data_pointer  += wr_bytes;																																						//DEBUG_EC_EEPROM
//		//data = data_pointer;																																									//DEBUG_EC_EEPROM
//		//data_length -= wr_bytes;                                    /* number of bytes left */								//DEBUG_EC_EEPROM
//		/*
//		if ( 256 > data_length )
//		{
//			wr_bytes = data_length;
//		} 
//		else
//		{
//			wr_bytes = 256;
//		}
//		*/		//DEBUG_EC_EEPROM
//	}
//	while ( (data_length > 0) && (write_ok == true) );
//	
//	return write_ok;
}

/**
 ******************************************************************************
 * \fn bool EEPROM_reader(uint32_t address, void* data_pointer, unsigned int data_length)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
bool EEPROM_reader(uint32_t address, uint8_t* data_pointer, unsigned int data_length)
{
	#ifdef USE_INTERNAL_EEPROM
		bool read_ok = false;
		uint8_t k = 0;
		
		for(k = 0; k< (uint8_t)data_length; k++)
		{
			data_pointer[k] = *((uint16_t *)address);
			address++;
		}
		
		read_ok = true;
		return read_ok;
	#endif
//	bool read_ok = false;
//	uint8_t nbr_retries = 0;
//	
//	while ( (nbr_retries < 20) && (read_ok == false) )
//	{
//		// Configure the TWI data
//		/*
//		eeprom_twi_package.chip = EEPROM_TWI_CHIP_ADDR(address);
//		eeprom_twi_package.addr[0] = (uint8_t)(address >> 8);
//		eeprom_twi_package.addr[1] = (uint8_t)(address);
//		eeprom_twi_package.addr_length = 2;
//		eeprom_twi_package.length = data_length; // Data size
//		eeprom_twi_package.buffer = (void*) data_pointer;
//		*/			//DEBUG_EC_EEPROM
//		/*if ( twi_master_read(TWI0, &eeprom_twi_package) == STATUS_OK )
//		{
//			read_ok = true;
//		}
//		else
//		{
//			read_ok = false;
//			EEPROM_io_configure();
//			delay_ms(10);
//		}
//		*/		//DEBUG_EC_EEPROM
//		nbr_retries++;
//	}
//	
//	if ( read_ok == false )
//	{
//		EEPROM_io_configure();
//	}
//	
//	return read_ok;
}

/**
 ******************************************************************************
 * \fn bool EEPROM_erase_all(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_erase_all(void)
{
	uint8_t data[256];
	static uint32_t addr;
	/*
	memset(&data, 0xFF, (unsigned int)sizeof(data));
	
	EEPROM_writer( C1219_TBLS_PROG_ADDR, &data, 100);

	for ( addr = EXT_EEPROM_DATA_START_ADDR; addr <= EXT_EEPROM_DATA_END_ADDR; addr += 256 )
	{
		EEPROM_writer( addr, data, (unsigned int)256 );
		WATCHDOG_APP_vRestart();
		
		OPR_task();
		WATCHDOG_APP_vRestart();
		
		OPF_task();
		WATCHDOG_APP_vRestart();
	}
	*/			//DEBUG_EC_EEPROM
}

/**
 ******************************************************************************
 * \fn bool EEPROM_check_parameters(void)
 * \brief
 *
 * \param void
 * \retval void
 ******************************************************************************
 */
void EEPROM_check_parameters(void)
{
	uint8_t temp_buffer[20];
	uint8_t temp_param = 0;
	uint8_t dev_id[EEPROM_SGFX_DEVICE_ID_SIZE];			//DEBUG_EC
	uint8_t dev_pac[EEPROM_SGFX_DEVICE_PAC_SIZE];		//DEBUG_EC
	
	/*Check Firmware Part Number*/
	EEPROM_reader(EEPROM_FW_PART_NUMBER_ADDR, &temp_buffer[0], EEPROM_FW_PART_NUMBER_SIZE);
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
	/*Check SGFX Device ID*/
	EEPROM_reader(EEPROM_SGFX_DEVICE_ID_ADDR, &temp_buffer[0], EEPROM_SGFX_DEVICE_ID_SIZE);
	SIGFOX_API_get_device_id(dev_id);
	if(memcmp(&temp_buffer[0], &dev_id[0], EEPROM_SGFX_DEVICE_ID_SIZE) != 0)
	{
		EEPROM_writer(EEPROM_SGFX_DEVICE_ID_ADDR, &dev_id[0], EEPROM_SGFX_DEVICE_ID_SIZE);
	}
	/*Check SGFX Device PAC*/
	EEPROM_reader(EEPROM_SGFX_DEVICE_PAC_ADDR, &temp_buffer[0], EEPROM_SGFX_DEVICE_PAC_SIZE);
	SIGFOX_API_get_initial_pac(dev_pac);
	if(memcmp(&temp_buffer[0], &dev_pac[0], EEPROM_SGFX_DEVICE_PAC_SIZE) != 0)
	{
		EEPROM_writer(EEPROM_SGFX_DEVICE_PAC_ADDR, &dev_pac[0], EEPROM_SGFX_DEVICE_PAC_SIZE);
	}
}

 /**
 ******************************************************************************
 * \fn uint16_t eeprom_resets_counter(bool inc)
 * \brief void
 *
 * \param bool
 * \retval uint16_t
 ******************************************************************************
 */
 
uint16_t eeprom_resets_counter(bool inc)
{
	volatile uint32_t data;
	#define TEST_ADDR_2_PTR		*((uint32_t *)	EEPROM_SGFX_RESET_COUNTER_ADDR)
	#define TEST_ADDR_2												EEPROM_SGFX_RESET_COUNTER_ADDR
	
	data = TEST_ADDR_2_PTR;
	
	HAL_DATA_EEPROMEx_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | FLASH_FLAG_FWWERR | FLASH_FLAG_NOTZEROERR);
	
	if (data == 0xFFFF)
	{
		HAL_DATA_EEPROMEx_Program(FLASH_TYPEPROGRAMDATA_WORD, TEST_ADDR_2, 0x0000);
	}
	else
	{
		if(inc ==true)
		{
			data++;
		}
		HAL_DATA_EEPROMEx_Program(FLASH_TYPEPROGRAMDATA_WORD, TEST_ADDR_2, data);
	}
	HAL_DATA_EEPROMEx_Lock();
	
	return data;
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
