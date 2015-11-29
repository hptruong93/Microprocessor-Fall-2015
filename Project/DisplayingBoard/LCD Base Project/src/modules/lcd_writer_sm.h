#ifndef LCD_WRITER_H
#define LCD_WRITER_H

#include "stm32f4xx.h"

#define LCD_WRITER_LINE_COUNT 13
#define LCD_WRITER_STR_LEN    20

void lcd_writer_display(void);
void lcd_write_message(char* message);
void lcd_writer_print_buffer(uint8_t* buffer, uint8_t len);
void lcd_writer_clear(void);

#endif
