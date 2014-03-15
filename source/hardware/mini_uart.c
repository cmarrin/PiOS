#include "hardware/mini_uart.h"
#include "hardware/gpio.h"

static gpio_reg *gGpio;
static MiniUart *gUart;
unsigned int mini_uart_initialize(void)
{
	gGpio = (gpio_reg*)GPIO_BASE;
	gUart = (MiniUart*)MINI_UART_BASE;

	// Setup the uart
	gUart->enables.raw = 1; // Can't set individual bits
	gUart->mu_ier.raw = 0;
	gUart->mu_cntl.raw = 0;
	gUart->mu_lcr.bits.data_size = 3; // 8-bit.. !? Set second bit to 1 as well? REserved? huh?
	gUart->mu_mcr.raw = 0;
	gUart->mu_ier.raw = 0;

	// Note: Can't set individual bits on iir - just doesn't work. TODO: Investigate?
	gUart->mu_iir.raw = 0xC6; // fifo_clear = 2, fifo_enables = 2 
	
	gUart->baud_rate.bits.baud_rate = 270; // ((250,000,000 / 115200) / 8) + 1;

	// Enable Uart on the GPIO pins
	gGpio->gpfsel1.bits.fsel14 = 2; // Pin 14 - Alt 5 (TXD1)
	gGpio->gpfsel1.bits.fsel15 = 2; // Pin 15 - Alt 5 (RXD1)

	unsigned volatile int i;
	for(i = 0; i < 150000; i++) { }

	gGpio->gppud.raw = 0;

	// Enable pull down/up clock on pin 14 and 15
	// TODO: Comment why this is necessary 
	gGpio->gppudclk0.bits.pin14 = 1;
	gGpio->gppudclk0.bits.pin15 = 1;

	for(i = 0; i < 150000; i++) { }

	// Disable pull down/up clocks
	gGpio->gppudclk0.raw = 0;
	
	// We have to set receiver_enabled and transmitter_enabled in
	// one operation here - not sure why. But it breaks if we dont :-(
	gUart->mu_cntl.raw = 3;

	return 0;
}

unsigned int mini_uart_read_char(unsigned int block)
{
	if(block)
	{
		while(gUart->mu_lsr.bits.data_ready == 0) { /* Do nothing */ }
	}
	
	return gUart->mu_io.bits.data;
}

void mini_uart_send_string(char* s)
{
	while(*s != '\0')
	{
		mini_uart_send_char(*s);

		s++;
	}
}

void mini_uart_send_char(unsigned int c)
{
	while(1)
	{
		if(gUart->mu_lsr.raw & 0x20) // Transmitter empty
			break;
	}

	if(c == '\n')
	{
		gUart->mu_io.raw = '\n'; // LF

		mini_uart_send_char('\r'); // Carriage return
	}
	else
	{
		gUart->mu_io.raw = c;
	}
}