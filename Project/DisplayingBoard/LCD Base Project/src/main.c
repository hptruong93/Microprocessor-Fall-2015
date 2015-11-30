/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header

#include <stdio.h>
#include <string.h>

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
//#include "background16bpp.h"

#include "interfaces/cc2500.h"
#include "modules/coordinate_db.h"
#include "modules/lcd_writer_sm.h"
#include "modules/commands.h"
//#include "modules/wireless_transmission_sm.h"
#include "modules/protocol_go_back_1.h"

#include "system_config.h"
#include "my_types.h"

static char long_message[40];
static uint8_t test[100];

static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_1a(void const *argument){
	while(1){
//		/* Clear the LCD */ 
//	LCD_Clear(LCD_COLOR_WHITE);
//	
//	  //The files source and header files implement drawing characters (drawing strings)
//	  //using different font sizes, see the file font.h for the four sizes
//	LCD_SetFont(&Font8x8);
//	  //The number of string lines avaialble is dependant on the font height:
//	  //A font height of 8 will result in 320 / 8 = 40 lines
//	LCD_DisplayStringLine(LINE(1), (uint8_t*)"      Welcome to uP lab     ");
//	LCD_DisplayStringLine(LINE(2), (uint8_t*)"          Good Luck         ");
//	  
//	  //The stm32f429i_discovery_lcd.h file offers functions which allows to draw various shapes
//	  //in either border or filled with colour. You can draw circles, rectangles, triangles, lines,
//	  //ellipses, and polygons. You can draw strings or characters, change background/foreground 
//	  //colours.
//	
//	LCD_DrawLine(0, 32, 240, LCD_DIR_HORIZONTAL);
//	LCD_DrawLine(0, 34, 240, LCD_DIR_HORIZONTAL);
//	LCD_SetTextColor(LCD_COLOR_BLUE2); 
//	LCD_DrawFullCircle(120, 160, 100);
//	LCD_SetTextColor(LCD_COLOR_CYAN); 
//	LCD_DrawFullCircle(120, 160, 90);
//	LCD_SetTextColor(LCD_COLOR_YELLOW); 
//	LCD_DrawFullCircle(120, 160, 80);
//	LCD_SetTextColor(LCD_COLOR_RED); 
//	LCD_DrawFullCircle(120, 160, 70);
//	LCD_SetTextColor(LCD_COLOR_BLUE); 
//	LCD_DrawFullCircle(120, 160, 60);
//	LCD_SetTextColor(LCD_COLOR_GREEN); 
//	LCD_DrawFullCircle(120, 160, 50);
//	LCD_SetTextColor(LCD_COLOR_BLACK); 
//	LCD_DrawFullCircle(120, 160, 40);
//	LCD_SetTextColor(LCD_COLOR_WHITE);
//	LCD_DrawRect(90,130,60,60);
//	LCD_SetTextColor(LCD_COLOR_MAGENTA);
//	LCD_FillTriangle(90, 120, 150, 130, 180, 130);
//	LCD_SetFont(&Font12x12);
//	LCD_DisplayStringLine(LINE(15), (uint8_t*)"      Success!    ");

	osDelay(250);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8_t temp;
static uint8_t is_drawing;
static uint8_t xs[255];
static uint8_t ys[255];

void draw_points(uint16_t scale_x, uint16_t scale_y, uint8_t* xs, uint8_t* ys, uint16_t length) {

	float prev_pixels_x = (float)xs[0] / scale_x;
	float prev_pixels_y = (float)ys[0] / scale_y;
	float pixels_x;
	float pixels_y;

	for (int i = 1; i < length; ++i) {
		pixels_x = (float)xs[i] / scale_x;
		pixels_y = (float)ys[i] / scale_y;

		LCD_DrawUniLine(prev_pixels_x, prev_pixels_y, pixels_x, pixels_y);

		prev_pixels_x = pixels_x;
		prev_pixels_y = pixels_y;
	}
}

void draw_from_db(void) {
	LCD_Clear(LCD_COLOR_WHITE);
	uint8_t len = coordinate_db_get_len();
	if (len == 0) {
		return;
	}

	static coordinate rr;
	uint8_t count = 0;

	for (uint8_t i = 0; i < len; i++) {
		coordinate_db_get_entry(i, &rr);
		xs[i] = rr.x;
		ys[i] = rr.y;
		count = 1;
	}

	draw_points(1, 1, xs, ys, len);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_1b(void const *argument) {
	is_drawing = FALSE;
	lcd_writer_clear();
	//coordinate_db_init();
	protocol_go_back_1_init(GO_BACK_ONE_MODE_RECEIVER);
	
	static uint8_t result = 0;
	
	while (1) {
		memset(test, 0, 12);
		protocol_go_back_1_periodic(&temp);

		test[0] = protocol_go_back_1_get_state();
		test[1] = CC2500_get_state();
		
		uint8_t len = protocol_go_back_1_get_received_data(test + 2);
		lcd_writer_clear();
		lcd_writer_print_buffer(test, 14);
		sprintf(long_message, "%d", 4);
		lcd_write_message(long_message);
		sprintf(long_message, "drawing = %d", is_drawing);
		lcd_write_message(long_message);
		sprintf(long_message, "len is %d", coordinate_db_get_len());
		lcd_write_message(long_message);
		sprintf(long_message, "result is %d", result);
		lcd_write_message(long_message);
		
		
		if (test[0] == GO_BACK_ONE_RECEIVER_STATE_IDLE) {
			if (len == COMMAND_CLEAR_LEN) {
				if (memcmp(test + 2, CLEAR_COMMAND, len) == 0) {
					is_drawing = FALSE;
					coordinate_db_clear();
					protocol_go_back_1_init(GO_BACK_ONE_MODE_RECEIVER);
					continue;
				}
			}

			if (len == COMMAND_PLOT_LEN) {
				if (memcmp(test + 2, PLOT_COMMAND, len) == 0) {
					is_drawing = TRUE;
					draw_from_db();
					break;
				}
			}

			if (is_drawing == FALSE) {
				if (len > 0) {
					result = coordinate_db_insert_entry(test + 2, len);
				}
			}

			protocol_go_back_1_receive();
		}
		
		osDelay(20);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_1c(void const *argument){
	while(1){
		if (is_drawing == FALSE) {
			LCD_Clear(LCD_COLOR_WHITE);
			lcd_writer_display();
		} else {
			break;
		}

		osDelay(250);
	}
}

osThreadDef(example_1a, osPriorityNormal, 1, 0);
osThreadDef(example_1b, osPriorityNormal, 1, 0);
osThreadDef(example_1c, osPriorityNormal, 1, 0);

// ID for theads
osThreadId example_1a_thread;
osThreadId example_1b_thread;
osThreadId example_1c_thread;

/*
 * main: initialize and start the system
 */
int main (void) {
	CC2500_LowLevel_Init();
	CC2500_Reset();

	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	// initialize peripherals here
	/* LCD initiatization */
	LCD_Init();
  
	/* LCD Layer initiatization */
	LCD_LayerInit();

	/* Enable the LTDC controler */
	LTDC_Cmd(ENABLE);

	/* Set LCD foreground layer as the current layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);

	LCD_SetFont(&Font16x24);
//	LCD_Clear(LCD_COLOR_WHITE);
//	static uint8_t sending_coordinates[3][11] = {
//		{0x00, 10, 10, 20, 20, 30, 20, 30, 30, 40, 30},
//		{0x00, 40, 40, 50, 40, 50, 50, 60, 50, 60, 60},
//		{0x00, 75, 75, 100, 120, 120, 140, 150, 150, 160, 170},
//	};
//	coordinate_db_insert_entry(sending_coordinates[0] + 1, 10);
//	uint8_t x = coordinate_db_get_len();
//	coordinate_db_insert_entry(sending_coordinates[1] + 1, 10);
//	x = coordinate_db_get_len();
//	coordinate_db_insert_entry(sending_coordinates[2] + 1, 10);
//	x = coordinate_db_get_len();
//	draw_from_db();
//	return 1;
	
	
	/*******************************************************
			 Uncomment the example you want to see
	example_1a: Simple shape draw, fill and text display
	example_1b: bitmap image display
	example_1c: Simple animation
	********************************************************/

	//example_1a_thread = osThreadCreate(osThread(example_1a), NULL);
	example_1b_thread = osThreadCreate(osThread(example_1b), NULL);
	example_1c_thread = osThreadCreate(osThread(example_1c), NULL);

	osKernelStart ();                         // start thread execution 
}
