#include "stm32f4_discovery_lcd.h"
#include "stmpe811qtr.h"
#include "LCD_Touch_Calibration.h"


int main(void)
{

	STM32f4_Discovery_LCD_Init();
	LCD_SetCursor(0x00, 0x00); 
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */



	while(1)
	{

		for(y = 0; y < LCD_PIXEL_HEIGHT; y++) 
		{

			for(x = 0; x < LCD_PIXEL_WIDTH; x++) 
			{

				LCD_WriteRAM((red<<8)+green);
				LCD_WriteRAM(blue);
			}
		}
	}	

	

}
