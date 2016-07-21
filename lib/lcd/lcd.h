#ifndef _LCD_H_
#define _LCD_H_

/*
 *	LCD interface header file
 */

/* 	Defining CHECKBUSY will check if the LCD is busy. The RW bit of the 
 * 	LCD must connected to a port of the processor for the check busy
 * 	process to work.
 * 
 * 	If CHECKBUSY is not defined it will instead use a delay loop.
 * 	The RW bit of the LCD does not need to connected in this case.
 */

//#define	_XTAL_FREQ	40000000

//#define CHECKBUSY	1

#ifdef CHECKBUSY
	#define	 LCD_WAIT   lcd_check_busy()
#else
	#define  LCD_WAIT   _delay(50000)	 /* delay para hitech pic18PRO */
#endif

#define MESSAGE_LINE		0x0

#define LCD_RS	LD1
#define LCD_EN	LD2
#define LCD_RW	LD3 
#define LCD_DATA	LATD
#define LCD_DATA_PORT	PORTD 
#define LCD_RS_TRIS	TRISD1
#define LCD_EN_TRIS	TRISD2
#define LCD_RW_TRIS	TRISD3 
#define LCD_DATA_TRIS	TRISD


#define OUTPUT_PIN      0x0	
#define INPUT_PIN       0x1	
#define OUTPUT_DATA     0x0	
#define INPUT_DATA      0xF0	

#define LCD_STROBE()	LCD_EN = 1; NOP(); NOP(); LCD_EN = 0;

#define LCD_STROBE_READ(value)	LCD_EN = 1; \
				NOP(); NOP(); \
				value=LCD_DATA_PORT; \
				LCD_EN = 0; 

#define	lcd_cursor(x)			lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()				lcd_cmd(0x1)
#define lcd_putch(x)			lcd_data(x)
#define lcd_goto(x)				lcd_cmd(0x80+(x))
#define lcd_cursor_right()		lcd_cmd(0x14)
#define lcd_cursor_left()		lcd_cmd(0x10)
#define lcd_display_shift()		lcd_cmd(0x1C)
#define lcd_home()				lcd_cmd(0x2)

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_init(void);
extern void ScrollMessage(unsigned char ,const char * s);

#endif

