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
#include "interfaces/cc2500_settings.h"

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

static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}

void write_config() {
	uint8_t value, address;
	for (uint8_t i = 0; i < CC2500_CONFIG_COUNT; i++) {
		value = cc2500_config[i][0];
		address = cc2500_config[i][1];
		CC2500_Write(&value, address, 1);
	}
}

void print_buffer(uint8_t* buffer, uint8_t num) {
	for (uint8_t i = 0; i < num; i++) {
		static char temp[10];
		itoa(buffer[i], temp);
		set_message(i, temp);		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_1a(void const *argument){
	while(1){
		/* Clear the LCD */ 
	LCD_Clear(LCD_COLOR_WHITE);
	
	  //The files source and header files implement drawing characters (drawing strings)
	  //using different font sizes, see the file font.h for the four sizes
	LCD_SetFont(&Font8x8);
	  //The number of string lines avaialble is dependant on the font height:
	  //A font height of 8 will result in 320 / 8 = 40 lines
	LCD_DisplayStringLine(LINE(1), (uint8_t*)"      Welcome to uP lab     ");
	LCD_DisplayStringLine(LINE(2), (uint8_t*)"          Good Luck         ");
	  
	  //The stm32f429i_discovery_lcd.h file offers functions which allows to draw various shapes
	  //in either border or filled with colour. You can draw circles, rectangles, triangles, lines,
	  //ellipses, and polygons. You can draw strings or characters, change background/foreground 
	  //colours.
	
	LCD_DrawLine(0, 32, 240, LCD_DIR_HORIZONTAL);
	LCD_DrawLine(0, 34, 240, LCD_DIR_HORIZONTAL);
	LCD_SetTextColor(LCD_COLOR_BLUE2); 
	LCD_DrawFullCircle(120, 160, 100);
	LCD_SetTextColor(LCD_COLOR_CYAN); 
	LCD_DrawFullCircle(120, 160, 90);
	LCD_SetTextColor(LCD_COLOR_YELLOW); 
	LCD_DrawFullCircle(120, 160, 80);
	LCD_SetTextColor(LCD_COLOR_RED); 
	LCD_DrawFullCircle(120, 160, 70);
	LCD_SetTextColor(LCD_COLOR_BLUE); 
	LCD_DrawFullCircle(120, 160, 60);
	LCD_SetTextColor(LCD_COLOR_GREEN); 
	LCD_DrawFullCircle(120, 160, 50);
	LCD_SetTextColor(LCD_COLOR_BLACK); 
	LCD_DrawFullCircle(120, 160, 40);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawRect(90,130,60,60);
	LCD_SetTextColor(LCD_COLOR_MAGENTA);
	LCD_FillTriangle(90, 120, 150, 130, 180, 130);
	LCD_SetFont(&Font12x12);
	LCD_DisplayStringLine(LINE(15), (uint8_t*)"      Success!    ");
		
	osDelay(250);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t tett, tett1, tett2;
void example_1bb(void const *argument) {
	static char testing[5];

	while (1) {
		test[0] = 0;
		test[1] = 0;
		test[2] = 0;
		test[3] = 0;

		CC2500_Read(test + 1, CC2500_MARCSTATE, 1); //MARCSTATE
		
		static uint8_t sent = 0;
		if (sent == 1 && test[1] != 19 && test[1] != 22) {
			CC2500_Read(test, CC2500_STX, 1);
		}

		// if (test[1] == 13) {
		// 	CC2500_Read(test + 2, CC2500_RXBYTES, 1); //Rx bytes
		// 	if (test[2] > 0) {
		// 		CC2500_Read(test + 3, CC2500_RX_FIFO, 1); //Read rx fifo
		// 		set_message(8, "Kkk");
		// 	} else {
		// 		set_message(9, "Nope");
		// 	}
		// }
		if (test[1] != 19 && sent != 1) {
			static uint8_t sending = 0;
			sending = (sending + 1) % 10;
			uint8_t to_send = sending + 'A';
			tett = to_send;
			CC2500_Write(&to_send, CC2500_TX_FIFO, 1);
			sent = 1;
		} else {
			static uint8_t count = 0;
			count = (count + 1) % 100;
			
			itoa(count, testing);
			set_message(7, testing);
			set_message(8, "But why");
		}

		print_buffer(test, 5);
		osDelay(1000);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define STATE(x) ((x & (7 << 4)) >> 4)
#define LEN_MASK(x) (x & 15)
void example_1b(void const *argument) {
	static char testing[5];
	CC2500_Read(test, CC2500_SRX, 1);

	while (1) {
		for (uint8_t i = 0; i < 10; i++) {
			if (i != 3) {
				test[i] = 0;
			}
		}
		clear_message(7);
		clear_message(8);

		CC2500_Read(test + 5, 0x34 | 0xC0, 1); //MARCSTATE
		CC2500_Read(test + 1, CC2500_MARCSTATE, 1);

		if (test[1] == CC2500_STATE_RX_OVERFLOW) {
			CC2500_Read(test, CC2500_SFRX, 1);
			continue;
		} else if (test[1] != CC2500_STATE_RX) {
			CC2500_Read(test, CC2500_SRX, 1);
			CC2500_Read(test + 1, CC2500_MARCSTATE, 1);
			continue;
		}

		if (test[1] == CC2500_STATE_RX) {
			CC2500_Read(test + 2, CC2500_RXBYTES, 1);

			if (test[2] > 0) {
				CC2500_Read(test + 3, CC2500_RX_FIFO, 1); //Read rx fifo
				set_message(9, "kkk");
			} else {
				set_message(8, "Nope");
			}
		} else {
			static uint8_t count = 0;
			count = (count + 1) % 100;
			
			itoa(count, testing);
			set_message(7, testing);
		}

		print_buffer(test, 7);
		osDelay(500);
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
	// printf("Start..");
	
	CC2500_LowLevel_Init();
	// system_init();
	
	CC2500_Read(test, CC2500_SRES, 1);
	CC2500_Read(test + 1, CC2500_PARTNUM, 1);
	
	do {
		test[0] = 0;
		CC2500_Read(test, CC2500_MARCSTATE, 1);
	} while (test[0] != CC2500_STATE_IDLE);

///////////////////////////////////////////////////////////
	write_config();
	// uint8_t value[2] = {0, 0};
	// value[0] = 0;
	// CC2500_Read(value, CC2500_FREQ2_REG_ADDR, 1);
///////////////////////////////////////////////////////////
	CC2500_Read(test + 1, CC2500_PARTNUM, 1);
	
	print_buffer(test, 3);

	CC2500_Read(test, CC2500_SFRX, 1);
	CC2500_Read(test, CC2500_SFTX, 1);
	
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
