CC=avr-gcc

CFLAGS= -DF_CPU=16000000L -mmcu=atmega328p -O0
INC=-I./inc/

SRCDIR=./src
OBJDIR=./obj
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

flash: blink.hex
	sudo avrdude -F -v -c stk500v1 -p atmega328p -U flash:w:blink.hex -P /dev/ttyACM0 -b 115200 

blink.hex: blink.elf
	avr-objcopy -R .eeprom -O ihex blink.elf blink.hex

blink.elf: $(OBJ)
	$(CC) $(CFLAGS) $^ -g -o blink.elf

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INC) $(CFLAGS) $^ -c -g -o $@

clean:
	rm $(OBJ) ./blink.hex
