leapyeartest.c:
	A program written for GNU/Linux that shows the differences between a full
	and partial leap year implementation.

	Result: A partial leap year implementation uses less code and CPU cycles
	and may be preferred if the product is only going to be in use only up to
	the year 2100.

	$ avr-size -C **before**.elf
	AVR Memory Usage
	----------------
	Device: Unknown

	Program:     462 bytes
	(.text + .data + .bootloader)

	Data:          6 bytes
	(.data + .bss + .noinit)


	$ avr-size -C **after**.elf
	AVR Memory Usage
	----------------
	Device: Unknown

	Program:     420 bytes
	(.text + .data + .bootloader)

	Data:          6 bytes
	(.data + .bss + .noinit)
