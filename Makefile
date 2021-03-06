APP ?= dummy
TTYUSB ?= /dev/ttyUSB0

build:
	avr-gcc -std=c99 -DF_CPU=16000000 -Wall -Werror -Os -I. -mmcu=atmega328p -o firmware.elf *.c apps/$(APP).c
	avr-objcopy -I elf32-avr -O ihex firmware.elf firmware.hex
	avr-size --mcu=atmega328p -C firmware.elf

flash: build
	touch tty.lock
	screen -X -S tty quit || true
	avrdude -b 57600 -p atmega328p -c arduino -P $(TTYUSB) -U flash:w:firmware.hex
	rm tty.lock

tty:
	while true; do [ -f tty.lock ] || screen -S tty $(TTYUSB) 9600; sleep 1; done

clean:
	rm -f *.o *.hex *.elf
