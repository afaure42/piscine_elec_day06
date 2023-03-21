#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

#define LED_D5_R (1 << PD5)
#define LED_D5_G (1 << PD6)
#define LED_D5_B (1 << PD3)

volatile uint8_t g_pos;
volatile uint8_t g_prescaler;

void wheel(uint8_t pos);

// ISR(TIMER2_COMPA_vect)
// {
// 	g_prescaler++;
// 	if (g_prescaler == 255)
// 	{
// 		g_pos++;
// 		wheel(g_pos);
// 	}
// }

void init_rgb()
{
	//set mode PWM with FF as top ( mode 3)
	//output mode is set at bottom and clear on match so higher match means brighter led
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1); 

	//set basic clock without prescaler
	TCCR0B = (1 << CS00);

	OCR0A = 0;
	OCR0B = 0;

	//same setting for timer 2
	TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2B1);
	TCCR2B = (1 << CS20);

	OCR2B = 0;
	// OCR2A = 0xFF;
	// TIMSK2 = 1 << OCIE2A;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

int main()
{
	// setting all three led pins to output
	DDRD = LED_D5_B | LED_D5_G | LED_D5_R;
	init_rgb();

	uint8_t pos = 0;
	sei();
	for (;;)
	{
		wheel(pos);
		_delay_ms(10);
		pos++;
	}
}
