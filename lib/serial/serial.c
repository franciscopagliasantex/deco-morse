#include <htc.h>
#include <stdio.h>
#include "serial.h"


/* Serial initialization */
void 
init_comms(void)
{
	SPBRG = DIVIDER;     	
	TXSTA = (SPEED|NINE_BITS|0x20);
	RCSTA = (NINE_BITS|0x90);
	TRISC6 = OUTPUT;
	TRISC7 = INPUT;
}

void 
putch(unsigned char byte) 
{
	/* output one byte */
	while(!TRMT)	/* set whilst TX in progress */
		continue;
	TXREG = byte;
}

unsigned char 
getch() {
	/* retrieve one byte */
	while(!RCIF)	/* set when register is not empty */
		continue;
	return RCREG;	
}

unsigned char
getche(void)
{
	unsigned char c;
	putch(c = getch());
	return c;
}

unsigned char getch_timeout(void)
{
	unsigned char i;

	for (i=0;i<10;i++)   // ejecuta 10 veces el retardo de 1 mSeg.
	{
		_delay(10000);	/* delay 10000 ciclos de intrucion, a 40MHz aprox. = 1 mSeg */
		
		CLRWDT();
		if (RCIF)
		{
			return RCREG;
		}
		
	}
	return 0;
}