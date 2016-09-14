## leapyeartest.c
A program written for GNU/Linux that shows the differences between a full
and partial leap year implementation.

Note that this code is *untested*.

### Results
Usual leap year checking.
```
$ avr-size -C before.elf
AVR Memory Usage
----------------
Device: Unknown

Program:     462 bytes
(.text + .data + .bootloader)

Data:          6 bytes
(.data + .bss + .noinit)
```

Partial leap year checking.
```
$ avr-size -C after.elf
AVR Memory Usage
----------------
Device: Unknown

Program:     420 bytes
(.text + .data + .bootloader)

Data:          6 bytes
(.data + .bss + .noinit)
```

### Conclusion
A partial leap year implementation produces a smaller binary and uses
less CPU cycles and so may be preferred if the product is only going to be in
use only up to the year 2100 and if the microcontroller has a very limited
amount of programmable memory.
