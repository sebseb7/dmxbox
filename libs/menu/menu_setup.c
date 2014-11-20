
#include "menu_setup.h"
#include "menu_main.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/button.h"

void menu_setup()
{


	draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

	uint16_t text_width =  get_text_width_16pt("SETUP");

	draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "SETUP", 200,200,255);


	draw_button_icon(11,45,92,1,"Zones",155,0,0,0,255,0);
	draw_button_icon(114,45,92,1,"Actions",155,0,0,0,255,0);
	draw_button_icon(217,45,92,1,"Loops",155,0,0,0,255,0);
	draw_button_icon(11,110,92,1,"Scenes",155,0,0,0,255,0);
	draw_button_icon(114,110,92,1,"Devices",155,0,0,0,255,0);
	draw_button_icon(217,110,92,1,"",55,55,55,0,255,0);
	draw_button_icon(11,175,92,1,"",55,55,55,0,255,0);
	draw_button_icon(114,175,92,1,"",55,55,55,0,255,0);
	draw_button_icon(217,175,92,1,"General",155,0,0,0,255,0);


}

