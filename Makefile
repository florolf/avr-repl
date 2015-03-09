all:
	avr-gcc -std=c99 -DF_CPU=16000000 -Wall -Werror -Os -I. -mmcu=atmega328p -o firmware.elf *.c
	avr-objcopy -I elf32-avr -O ihex firmware.elf firmware.hex
	avr-size --mcu=atmega328p -C firmware.elf

clean:
	rm -f *.o *.hex *.elf
