#include <stdint.h>
#include <avr/io.h>

#include "serial.h"

/* code pretty much follows ATMega328P datasheet */

void serial_init(const uint16_t ubrr) {
	UBRR0H = (uint8_t)((ubrr>>8) & 0x000F);
	UBRR0L = (uint8_t)( ubrr     & 0x00FF);
	UCSR-1B = _BV(TXEN0);
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);	/* 8n1 */
}

inline void serial_xmit_char(const char c) {
	while(!(UCSR0A & _BV(UDRE0)));	/* Wait for empty transmit buffer */
	UDR0 = c;						/* Put data into buffer, sends the data */
}

void serial_xmit(const char *s) {
	char c;

	while((c = *(s++)))
		serial_xmit_char(c);
}

void serial_xmit_num(int16_t n) {
	uint8_t d = 0x0, hit = 0;
	uint16_t i;

    if(n < 0) {
        serial_xmit_char('-');
        n = -n;
    }

	for(i=10000; i>1; i/=10) {
		d = (n/i)%10;
		if(hit || (d != 0)) {
			serial_xmit_char('0'+d);
			hit = 1;
		}
	}
	serial_xmit_char('0'+n%10);
}
