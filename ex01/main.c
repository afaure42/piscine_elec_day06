#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_D5_R (1 << PD5)
#define LED_D5_G (1 << PD6)
#define LED_D5_B (1 << PD3)



int main()
{
	//setting all three led pins to output
	DDRD = LED_D5_B | LED_D5_G | LED_D5_R;

	for(;;)
	{
		PORTD = LED_D5_R;
		_delay_ms(1000);
		PORTD = LED_D5_G;
		_delay_ms(1000);
		PORTD = LED_D5_B;
		_delay_ms(1000);
		PORTD = LED_D5_R | LED_D5_G; // yellow
		_delay_ms(1000);
		PORTD = LED_D5_G | LED_D5_B; // cyan
		_delay_ms(1000);
		PORTD = LED_D5_R | LED_D5_B; // magenta
		_delay_ms(1000);
		PORTD = LED_D5_R | LED_D5_B | LED_D5_G; // white
		_delay_ms(1000);
		
	}
}
