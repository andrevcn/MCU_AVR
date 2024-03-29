/*------------------------------------------------*/
/* UART functions                                 */
/*------------------------------------------------*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define	UART_BPS		115200
#define	UART_BUFF		64


typedef struct {
	uint16_t	wi, ri, ct;
	uint8_t buff[UART_BUFF];
} FIFO;
static
volatile FIFO TxFifo, RxFifo;



/* Initialize UART */

void uart_init (uint32_t bps)
{
	UCSR0B = 0;

	PORTE |= _BV(1); DDRE |= _BV(1);	/* Set TXD as output */
	DDRE &= ~_BV(0); PORTE &= ~_BV(0); 	/* Set RXD as input */

	RxFifo.ct = 0; RxFifo.ri = 0; RxFifo.wi = 0;
	TxFifo.ct = 0; TxFifo.ri = 0; TxFifo.wi = 0;

	UBRR0L = F_CPU / bps / 16 - 1;
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0);
}


/* Get a received character */

uint16_t uart_test (void)
{
	return RxFifo.ct;
}


uint8_t uart_getc (void)
{
	uint8_t d, i;


	while (RxFifo.ct == 0) ;

	i = RxFifo.ri;
	d = RxFifo.buff[i];
	cli();
	RxFifo.ct--;
	sei();
	RxFifo.ri = (i + 1) % sizeof RxFifo.buff;

	return d;
}


/* Put a character to transmit */

void uart_putc (uint8_t d)
{
	uint8_t i;


	while (TxFifo.ct >= sizeof TxFifo.buff) ;

	i = TxFifo.wi;
	TxFifo.buff[i] = d;
	cli();
	TxFifo.ct++;
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0) | _BV(UDRIE0);
	sei();
	TxFifo.wi = (i + 1) % sizeof TxFifo.buff;
}

void uart_puts(char *str)
{
	char k;
	while ((k=*str++)) uart_putc(k);
	uart_putc(10);
	uart_putc(13);
}


/* UART RXC interrupt */

ISR(USART0_RX_vect)
{
	uint8_t d, n, i;


	d = UDR0;
	n = RxFifo.ct;
	if (n < sizeof RxFifo.buff) {
		RxFifo.ct = ++n;
		i = RxFifo.wi;
		RxFifo.buff[i] = d;
		RxFifo.wi = (i + 1) % sizeof RxFifo.buff;
	}
}


/* UART UDRE interrupt */

ISR(USART0_UDRE_vect)
{
	uint8_t n, i;


	n = TxFifo.ct;
	if (n) {
		TxFifo.ct = --n;
		i = TxFifo.ri;
		UDR0 = TxFifo.buff[i];
		TxFifo.ri = (i + 1) % sizeof TxFifo.buff;
	}
	if (n == 0) UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0);
}

