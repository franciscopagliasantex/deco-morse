#include	<htc.h>
#include 	"lcd.h"

#ifdef CHECKBUSY

unsigned char 
lcd_read_cmd_nowait(void)
{
	unsigned char c, readc;

	LCD_DATA_TRIS	 = INPUT_DATA;

	LCD_RW = 1; // Read LCD
	NOP(); // short propagation delay
	NOP(); // short propagation delay


	LCD_STROBE_READ(readc); // Read high nibble
	c = ( readc & 0xF0 ); 
	LCD_STROBE_READ(readc); // Read low nibble
	
    c |= ( (readc >> 4) & 0x0F ); // Or in 4 more bits to low nibble
	
	
	LCD_RW = 0; // Return to default mode of writing LCD
	LCD_DATA_TRIS	 = OUTPUT_DATA; // Return to default mode of writing LCD

	return(c);
}

void
lcd_check_busy(void) // Return when the LCD is no longer busy, or we've waiting long enough!
{
	// To avoid hanging forever in event there's a bad or 
	// missing LCD on hardware.  Will just run SLOW, but still run.
	unsigned int retry; 
	unsigned char c;

	for (retry=1000; retry-- > 0; ) {
		c = lcd_read_cmd_nowait();
		if (0==(c&0x80)) break; // Check busy bit.  If zero, no longer busy
	}
}

#endif

/* send a command to the LCD */
void
lcd_cmd(unsigned char c)
{
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA = ((LCD_DATA & 0x0F)|(c & 0xF0 )); //sale nibble superior
	LCD_STROBE();
    LCD_DATA = ((LCD_DATA & 0x0F)|((c<<4) & 0xF0 )); //sale nibble inferior
	LCD_STROBE();
}

/* send data to the LCD */
void
lcd_data(unsigned char c)
{
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA = 0;
	LCD_RS = 1;
	
    LCD_DATA = ((LCD_DATA & 0x0F)|(c & 0xF0 )); //sale nibble superior
	LCD_STROBE();
    LCD_DATA = ((LCD_DATA & 0x0F)|((c<<4) & 0xF0 )); //sale nibble inferior
	LCD_STROBE();
	LCD_RS = 0;
}

/* write a string of chars to the LCD */

void
lcd_puts(const char * s)
{
	while(*s){
		lcd_data(*s++);
	}
}

/* initialize the LCD */
void
lcd_init(void)
{
	char init_value;

	init_value = 0x30;

	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;
	LCD_RS_TRIS	 = OUTPUT_PIN;
	LCD_EN_TRIS	 = OUTPUT_PIN;
	LCD_RW_TRIS	 = OUTPUT_PIN;
	LCD_DATA_TRIS	 = OUTPUT_DATA;
	_delay(150000);	/* delay 150000 ciclos de intrucion, a 40MHz aprox. = 15 mSeg */
	LCD_DATA	 = init_value;
	LCD_STROBE();
	_delay(50000);	/* delay 50000 ciclos de intrucion, a 40MHz aprox. = 5 mSeg */

	LCD_DATA	 = init_value;
	LCD_STROBE();
	_delay(10000);	/* delay 10000 ciclos de intrucion, a 40MHz aprox. = 1 mSeg */

	LCD_DATA	 = init_value;
	LCD_STROBE();
	

	LCD_WAIT; //may check LCD busy flag, or just delay a little, depending on lcd.h
	LCD_DATA = 0x20; // Set 4-bit mode
	LCD_STROBE();

	lcd_cmd(0x28); // Function Set

	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
	LCD_WAIT;
}


void ScrollMessage(unsigned char row,const char Message[])
{
 char j, TempS[30];
 unsigned int  MHead=0,Done=0,count;
 if(row >1) row=1;
 row=row*40;
  while(Done==0)
  {
      for(count=0;count<20;count++)
       {
 	  TempS[count]=Message[MHead+count];
 	  if(Message[MHead+count+1]==0){ Done=1;}
 	  }
   MHead++;
   lcd_goto(row);
   lcd_puts(TempS);
   for (j=0; j<26; ++j)		// bucle de retardo desplazamiento scrolling
	 {
		_delay(150000);	
	 }  	 
  }
}
