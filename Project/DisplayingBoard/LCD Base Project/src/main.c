/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
//#include "background16bpp.h"

#include "interfaces/cc2500.h"
//#include "modules/wireless_transmission_sm.h"
#include "modules/protocol_go_back_1.h"

#include <stdio.h>
#include <string.h>
#include "system_config.h"
#include "my_types.h"

#define message_size 10
#define num_messages 14

static char messages[num_messages][message_size];
static uint8_t has_msg[num_messages];
static uint8_t test[100];

void itoa(int n, char* s)
{
	int i, sign;

	if ((sign = n) < 0)  /* record sign */
	 n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
	 s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0)
	 s[i++] = '-';
	s[i] = '\0';

	for (int j = 0; j < i / 2; j++) {
		char temp = s[j];
		s[j] = s[i - 1 - j];
		s[i - 1 - j] = temp;
	}
}

void print_messages(void) {
	for (uint8_t i = 0; i < num_messages; ++i) {
		if (has_msg[i] == TRUE) { 
			LCD_DisplayStringLine(LINE(i), messages[i]);
		}
	}
}

void clear_message(uint8_t msg_index) {
	if (msg_index > num_messages -1) return;
	has_msg[msg_index] = FALSE;
}

void set_message(uint8_t msg_index, char* new_message) {
	if (msg_index > num_messages -1) return;
	strcpy(messages[msg_index], new_message);
	has_msg[msg_index] = TRUE;
}

void set_int_message(uint8_t msg_index, int n) {
	static char holder[10];
	itoa(n, holder);
	set_message(msg_index, holder);
}

void print_bufferr(uint8_t* buffer, uint8_t num) {
	for (uint8_t i = 0; i < num; i++) {
		static char temp[10];
		itoa(buffer[i], temp);
		set_message(i, temp);		
	}
}

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
static uint8_t temp, temp2;
static wireless_received_packet received_test;

void example_1b(void const *argument) {
	wireless_transmission_init();
	protocol_go_back_1_init(GO_BACK_ONE_MODE_RECEIVER);
	
	static uint8_t count = 0;
	
	while (1) {
		memset(test, 0, 12);
		received_test.buffer = test;
		wireless_transmission_periodic(&temp);
//		uint8_t state = wireless_transmission_get_state();
//		if (state == WIRELESS_TRANSMISSION_STATE_IDLE) {
//			wireless_transmission_get_received_packet(&received_test);
//			test[12] = received_test.status;
//			test[13] = received_test.len;
//			
//			if (test[12] == 0x01) {
//				set_message(10, "Good");
//			} else {
//				set_message(10, "Bad");
//			}
//			
//			wireless_transmission_receive_packet();
//		} else {
//			wireless_transmission_get_received_packet(&received_test);
//		}
//		print_bufferr(test, 8);
		
		protocol_go_back_1_periodic(&temp2);

		test[0] = protocol_go_back_1_get_state();
		if (test[0] == GO_BACK_ONE_RECEIVER_STATE_IDLE) {
			protocol_go_back_1_receive();
		}
		test[1] = CC2500_get_state();
		protocol_go_back_1_get_received_data(test + 2);
		print_bufferr(test, 14);
		
		
		osDelay(100);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_points(uint16_t scale_x, uint16_t scale_y, uint16_t* xs, uint16_t* ys, uint16_t length) {

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_1c(void const *argument){
	while(1){
		/* Clear the LCD */ 

		LCD_Clear(LCD_COLOR_WHITE);

		print_messages();

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
