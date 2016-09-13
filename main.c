#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/**
 * Implementing a real time clock for the ATMega 48 series.
 * Adapted from the "AVR134: Real Time Clock (RTC) Using the
 * Asynchronous Timer)" document release by Atmel for the ATMega 128.
 */

/**
 * Use a partial implementation to find leap years. This will only work for up
 * to year 2100. If you expect your product to work past the year 2100 or want
 * a full implementation anyway, comment out this define.
 */
#define PARTIAL_LEAP

// TODO: Research fuses


// TODO: ATMega 48PB does not have asynchronous 8-bit Timer, only 16-bit.
// Check if this code still works regardless.


static char not_leap(void);
static void init(void);

typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char date;
	unsigned char month;
	unsigned char year;
}time;

time t;

int main(void)
{
	/* Initialize registers and configure RTC. */
    init();

	while(1)
	{
		/* Enter sleep mode. (Will wake up from timer overflow interrupt) */
		sleep_mode();

		/* Write dummy value to control register */
		TCCR2B = (1 << CS20) | (1 << CS22);

		/* Wait until TC2 is updated */
		while(ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << TCR2AUB)));
	}
}

static void init(void)
{
	/* Wait for external clock crystal to stabilize */
	for (uint8_t i = 0; i < 0x40; i++)
	{
		for (int j = 0; j < 0xFFFF; j++);
	}

	/* Configure all eight pins of port B as outputs */
	DDRB = 0xFF;

	/* Make sure all TC0 interrupts are disabled */
	TIMSK2 &= ~(( 1 << OCIE2B) | (1 << OCIE2A) | (1 << TOIE2));
	/**
	 * Set Timer/counter2 to be asynchronous from the CPU clock with a second
	 * external clock (32.768kHz) driving it.
	 */
	ASSR |= (1 << AS2);

	/* Reset timer */
	TCNT2 = 0;
	/**
	 * Prescale the timer to be clock source/128 to make it exactly 1 second
	 * for every overflow to occur
	 */
	TCCR2B =(1 << CS20)|(1 << CS22);

	/* Wait until TC2 is updated */
	while(ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << TCR2AUB)));

	/* Set 16-bit Timer/Counter2 Overflow Interrupt Enable */
	TIMSK2 |= (1 << TOIE2);
	/* Set the Global Interrupt Enable Bit */
	sei();
	/* Selecting power save mode as the sleep mode to be used */
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	/* Enabling sleep mode */
	sleep_enable();
}

/**
 * Update and calculate time.
 */
ISR(TIMER2_OVF_vect)
{
	if(++t.second == 60)
	{
		t.second = 0;

		if(++t.minute == 60)
		{
			t.minute = 0;

			if(++t.hour == 24)
			{
				t.hour = 0;

				if(++t.date == 32)
				{
					t.month++;
					t.date=1;
				}
				else if(t.date == 31)
				{
					if((t.month == 4) || (t.month == 6) || (t.month == 9) ||
							(t.month == 11))
					{
						t.month++;
						t.date = 1;
					}
				}
				else if(t.date == 30)
				{
					if(t.month == 2)
					{
						t.month++;
						t.date = 1;
					}
				}
				else if (t.date == 29)
				{
					if((t.month == 2) && (not_leap()))
					{
						t.month++;
						t.date = 1;
					}
				}

				if(t.month == 13)
				{
					t.month = 1;
					t.year++;
				}
			}
		}
	}
	PORTB = ~(((t.second & 0x01) | t.minute << 1) | t.hour << 7);
}

/**
 * Check for leap year.
 *
 * \return	0 if leap year.
 */
static char not_leap(void)
{
#ifdef PARTIAL_LEAP
	return (char)(t.year % 4);
#else
	if(!(t.year%100))
	{
		return (char)(t.year % 400);
	}
	else
	{
		return (char)(t.year % 4);
	}
#endif
}
