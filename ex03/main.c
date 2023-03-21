#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"

#define LED_D5_R (1 << PD5)
#define LED_D5_G (1 << PD6)
#define LED_D5_B (1 << PD3)

#define HEX_CHARS "0123456789ABCDEF"

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
	OCR2A = 0xFF;
	TIMSK2 = 1 << OCIE2A;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	uart_printstr("SETTING RGB AT ");
	uart_putnbr(r);
	uart_printstr(" ");
	uart_putnbr(g);
	uart_printstr(" ");
	uart_putnbr(b);
	uart_printstr("\r\n");
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

uint8_t is_inside(char c, char * str)
{
	while(*str)
	{
		if (c == *str)
			return 1;
		str++;
	}
	return 0;
}

uint8_t is_rgb_format(char * buffer)
{
	uint8_t i = 0;

	if (buffer[0] != '#')
		return 0;
	while (i < 6 && buffer[i + 1] != '\0')
	{
		if (is_inside(buffer[i + 1], HEX_CHARS) == 0)
			return 0;
		i++;
	}
	if (i == 6)
		return (1);
	return (0);
}

uint8_t str_find(char *str, char c)
{
	for(uint8_t i = 0; str[i]; i++)
	{
		if (str[i] == c)
			return i;
	}
	return 0;
}

uint32_t atoi_base(char *str, char *base, uint8_t base_length)
{
	uint32_t ret = 0;

	size_t i = 0;
	while(str[i])
	{
		ret *= base_length;
		ret += str_find(base, str[i]);
		i++;
	}
	return ret;
}

void str_to_rgb(char * buffer)
{
	uint32_t rgb = atoi_base(buffer + 1, HEX_CHARS, 16);

	set_rgb((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

int main()
{
	// setting all three led pins to output
	DDRD = LED_D5_B | LED_D5_G | LED_D5_R;
	init_rgb();
	uart_init();

	char buffer[255];
	uint8_t index = 0;

	uart_printstr("Enter RGB value in hex #RRGGBB format\r\n");
	for (;;)
	{
		uart_receive_word(buffer, &index, 0);
		if (is_rgb_format(buffer) == 0)
			uart_printstr("WRONG FORMAT, #RRGGBB\r\n");
		else
			str_to_rgb(buffer);
	}
}
