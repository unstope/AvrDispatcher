
#include "OneWire.h"

#define ONEWIRE_BIT			(ONEWIRE_PIN & _BV(ONEWIRE_PN))
#define ONEWIRE_PULL_LOW()	{ ONEWIRE_DDR |= _BV(ONEWIRE_PN);  }
#define ONEWIRE_LISTEN()	{ ONEWIRE_DDR &= ~_BV(ONEWIRE_PN); }

// Setup of the one-wire pin
void ow_init(void)
{
	ONEWIRE_PORT &= ~_BV(ONEWIRE_PN);
	ONEWIRE_LISTEN()
}

// Returns 0 if the sensor is present, 1 otherwise

uint8_t ow_reset(void)
{
	uint8_t status;
	cli();
	ONEWIRE_PULL_LOW()
	_delay_us(480);
	ONEWIRE_LISTEN()
	_delay_us(60);
	status = ONEWIRE_BIT;
	sei();
	_delay_us(410);
	return status;
}

// Read Bit Function
uint8_t ow_read_bit(void)
{
	uint8_t bit;
	cli();
	ONEWIRE_PULL_LOW()
	_delay_us(3);
	ONEWIRE_LISTEN()
	_delay_us(10);
	bit = ONEWIRE_BIT;
	sei();
	_delay_us(53);
	return bit;
}

// Write bit Function
void ow_write_bit(uint8_t bit)
{
	cli();
	ONEWIRE_PULL_LOW()
	if (bit)
	{
		_delay_us(10);
	}
	else
	{
		_delay_us(55);
	}
	ONEWIRE_LISTEN()
	sei();
	if (bit)
	{
		_delay_us(65);
	}
	else
	{
		_delay_us(5);
	}
}

// Read BYTE Function
uint8_t ow_read(void)
{
	uint8_t byte = 0;
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		byte >>= 1;
		if (ow_read_bit())
		{
			byte |= 0x80;
		}
	}
	return byte;
}

// Write BYTE Function
void ow_write(uint8_t byte)
{
	uint8_t i;
	uint8_t temp;

	for (i = 0; i < 8; i++)
	{
		temp = byte >> i;
		temp &= 1;
		ow_write_bit(temp);
	}
}