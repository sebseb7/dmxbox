/**
 ******************************************************************************
 * @file    stm324xg_discovery_lcd.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    30-September-2011
 * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
 *          and AM240320D5TOQW01H (LCD_ILI9325) Liquid Crystal Display Modules
 *          of STM324xG-EVAL evaluation board(MB786) RevB.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************  
 */ 
/**
 ******************************************************************************
 * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
 * @file    stm32f4_discovery_lcd.c
 * @author  CMP Team
 * @version V1.0.0
 * @date    28-December-2012
 * @brief   LCD LOW_LEVEL Drive 
 *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB, STM32F4DIS-CAM
 *          and STM32F4DIS-LCD modules.     
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery_lcd.h"

/** @addtogroup Utilities
 * @{
 */ 

/** @addtogroup STM32F4_DISCOVERY
 * @{
 */

/** @defgroup stm32f4_discovery_LCD 
 * @brief This file includes the LCD driver for (LCDSSD2119)
 * @{
 */ 

/** @defgroup stm32f4_discovery_LCD_Private_TypesDef
 * @{
 */

/** @defgroup stm32f4_discovery_LCD_Private define
 * @{
 */
#define LCD_RST_PIN                  (GPIO_Pin_3)
#define LCD_RST_PORT                 (GPIOD)

#define LCD_PWM_PIN                  (GPIO_Pin_13)
#define LCD_PWM_PORT                 (GPIOD)

/* Note: LCD /CS is NE1 - Bank 1 of NOR/SRAM Bank 1~4 */
#define  LCD_BASE_Data               ((u32)(0x60000000|0x00100000))
#define  LCD_BASE_Addr               ((u32)(0x60000000|0x00000000))
#define  LCD_CMD                     (*(vu16 *)LCD_BASE_Addr)
#define  LCD_Data                    (*(vu16 *)LCD_BASE_Data)

/**
 * @}
 */ 

/** @defgroup stm32f4_discovery_LCD_Private_Macros
 * @{
 */
#define ABS(X)  ((X) > 0 ? (X) : -(X))     

/**
 * @brief  LCD Default FSMC Init
 * @param  None
 * @retval None
 */
void LCD_DeInit(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	/*!< LCD Display Off */
	LCD_DisplayOff();

	/* BANK 3 (of NOR/SRAM Bank 1~4) is disabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);

	/*!< LCD_SPI DeInit */
	FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM3);

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
		GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_MCO);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
		GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
		GPIO_Pin_15;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_MCO);

	/* SRAM Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
		GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
		GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource0, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource1, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource2, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource3, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource4, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource5, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource13, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource14, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource15, GPIO_AF_MCO);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
		GPIO_Pin_4 | GPIO_Pin_5;

	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource1, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource2, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource3, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource4, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource5, GPIO_AF_MCO);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13, GPIO_AF_MCO);

	/* NOE and NWE configuration */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5, GPIO_AF_MCO);

	/* NE3 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 

	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_MCO);

	/* NBL0, NBL1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0, GPIO_AF_MCO);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1, GPIO_AF_MCO);
}

/**
 * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
 * @param  None
 * @retval None
 */
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOB, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
			RCC_AHB1Periph_GPIOF, ENABLE);

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines,  NOE and NWE configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
		GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
		GPIO_Pin_4 |GPIO_Pin_5;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
		GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
		GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	/* SRAM Address lines configuration LCD-DC */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);	   

	/* NE3 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);

	/* LCD RST configuration */
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

	/* LCD pwm configuration */
	GPIO_InitStructure.GPIO_Pin = LCD_PWM_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LCD_PWM_PORT, LCD_PWM_PIN);
}

/**
 * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
 * @param  None
 * @retval None
 */
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;

	/* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	p.FSMC_AddressSetupTime = 2;
	p.FSMC_AddressHoldTime = 0;
	p.FSMC_DataSetupTime = 11;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	/* Color LCD configuration ------------------------------------
	   LCD configured as follow:
	   - Data/Address MUX = Disable
	   - Memory Type = SRAM
	   - Data Width = 16bit
	   - Write Operation = Enable
	   - Extended Mode = Enable
	   - Asynchronous Wait = Disable */

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

	/* Enable FSMC NOR/SRAM Bank1 */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/**
 * @brief  LCD Init.
 * @retval None
 */
void STM32f4_Discovery_LCD_Init(void)
{ 

	unsigned long ulCount;

	/* Configure the LCD Control pins */
	LCD_CtrlLinesConfig();

	/* Configure the FSMC Parallel interface */
	LCD_FSMCConfig();

	delay_ms(5); 

	/* Reset LCD */
	GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);	
	delay_ms(10);	
	GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);

	/*
	   SSD2119Init(void)
	   */
	/* Enter sleep mode (if we are not already there).*/
	LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0001);

	/* Set initial power parameters. */
	LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x00B2);
	LCD_WriteReg(SSD2119_VCOM_OTP_1_REG, 0x0006);

	/* Start the oscillator.*/
	LCD_WriteReg(SSD2119_OSC_START_REG, 0x0001);

	/* Set pixel format and basic display orientation (scanning direction).*/
	LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG, 0x30EF);
	LCD_WriteReg(SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600);

	/* Exit sleep mode.*/
	LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0000);
	delay_ms(5);

	/* Configure pixel color format and MCU interface parameters.*/
	//#define ENTRY_MODE_DEFAULT 0x6830
	//LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);
	//LCD_WriteReg(SSD2119_ENTRY_MODE_REG, 0x4870);
	LCD_WriteReg(SSD2119_ENTRY_MODE_REG, 0x4870);

	/* Set analog parameters */
	LCD_WriteReg(SSD2119_SLEEP_MODE_2_REG, 0x0999);
	LCD_WriteReg(SSD2119_ANALOG_SET_REG, 0x3800);

	/* Enable the display */
	LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0033);

	/* Set VCIX2 voltage to 6.1V.*/
	LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0005);

	/* Configure gamma correction.*/
	LCD_WriteReg(SSD2119_GAMMA_CTRL_1_REG, 0x0000);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_2_REG, 0x0303);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_3_REG, 0x0407);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_4_REG, 0x0301);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_5_REG, 0x0301);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_6_REG, 0x0403);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_7_REG, 0x0707);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_8_REG, 0x0400);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_9_REG, 0x0a00);
	LCD_WriteReg(SSD2119_GAMMA_CTRL_10_REG, 0x1000);

	/* Configure Vlcd63 and VCOMl */
	LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x000A);
	LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

	/* Set the display size and ensure that the GRAM window is set to allow
	   access to the full display buffer.*/
	LCD_WriteReg(SSD2119_V_RAM_POS_REG, (LCD_PIXEL_HEIGHT-1) << 8);
	LCD_WriteReg(SSD2119_H_RAM_START_REG, 0x0000);
	LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);

	LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
	LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);

	/* clear the lcd  */
	LCD_WriteReg(SSD2119_RAM_DATA_REG, 0x0000);
	for(ulCount = 0; ulCount < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); ulCount++)
	{
		LCD_Data = 0;
	}
}


/**
 * @brief  Sets the cursor position.
 * @param  Xpos: specifies the X position.
 * @param  Ypos: specifies the Y position. 
 * @retval None
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	/* Set the X address of the display cursor.*/
	LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);

	/* Set the Y address of the display cursor.*/
	LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}

/**
 * @brief  Writes to the selected LCD register.
 * @param  LCD_Reg: address of the selected register.
 * @param  LCD_RegValue: value to write to the selected register.
 * @retval None
 */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	LCD_CMD = LCD_Reg;
	/* Write 16-bit Reg */
	LCD_Data = LCD_RegValue;
}

/**
 * @brief  Reads the selected LCD Register.
 * @param  LCD_Reg: address of the selected register.
 * @retval LCD Register Value.
 */
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD_CMD = LCD_Reg;
	/* Read 16-bit Reg */
	return (LCD_Data);
}

/**
 * @brief  Prepare to write to the LCD RAM.
 * @param  None
 * @retval None
 */
void LCD_WriteRAM_Prepare(void)
{
	LCD_CMD = SSD2119_RAM_DATA_REG;
}

/**
 * @brief  Writes to the LCD RAM.
 * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
 * @retval None
 */
void LCD_WriteRAM(uint16_t RGB_Code)
{
	/* Write 16-bit GRAM Reg */
	LCD_Data = RGB_Code;
}

/**
 * @brief  Reads the LCD RAM.
 * @param  None
 * @retval LCD RAM Value.
 */
uint16_t LCD_ReadRAM(void)
{
	/* Write 16-bit Index (then Read Reg) */
	//  LCD_CMD = SSD2119_RAM_DATA_REG; /* Select GRAM Reg */
	/* Read 16-bit Reg */
	return LCD_Data;
}

/**
 * @brief  Test LCD Display
 * @retval None
 */
void LCD_RGB_Test(void)
{
	uint32_t index;

	LCD_SetCursor(0x00, 0x00); 
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

	/* R */
	for(index = 0; index < (LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH)/3; index++)
	{
		LCD_Data = 0xFf00;
		LCD_Data = 0x0000;
	}

	/* G */
	for(;index < 2*(LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH)/3; index++)
	{
		LCD_Data = 0x00ff;
		LCD_Data = 0x0000;
	}

	/* B */
	for(; index < LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH; index++)
	{
		LCD_Data = 0x0000;
		LCD_Data = 0x00ff;
	}
}

/**
 * @brief  Clears the hole LCD.
 * @param  Color: the color of the background.
 * @retval None
 */
void LCD_Clear(uint16_t Color)
{
	uint32_t index = 0;

	LCD_SetCursor(0x00, 0x00); 
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for(index = 0; index < LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH; index++)
	{
		LCD_Data = Color;
	}  
}

