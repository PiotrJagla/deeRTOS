CC=arm-none-eabi-gcc 

INC=-Iexternal/CMSIS/CMSIS/Core/Include -Iexternal/CMSIS/Device/ST/STM32F3/Include -I./inc \
	-Iexternal/stm32_graphics_display_drivers/Drivers \
	-Iexternal/stm32_graphics_display_drivers/Drivers/lcd/ili9341 \
	-Iexternal/stm32_graphics_display_drivers/Drivers/Fonts \
	-Iexternal/stm32_graphics_display_drivers/Drivers/stm32f3xx 


#if RTOS is used add -DISRTOS flag
CFLAGS=-mcpu=cortex-m4 -mthumb --specs=nano.specs -DSTM32F302Rx -O0 -DISRTOS
LINKER_FILE=linker_script.ld
LD_FLAGS=-T $(LINKER_FILE) 

SRCDIR=./src
OBJDIR=./obj
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

PROGRAMMER=openocd
PROGRAMMER_FLAGS=-f interface/stlink.cfg -f target/stm32f3x.cfg

debug:
	openocd $(PROGRAMMER_FLAGS)

enterGdb:
	arm-none-eabi-gdb ./programBin.elf -x init.gdb


flash: programBin.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program programBin.elf verify reset exit"

all: programBin.elf

programBin.elf:$(OBJ)
	$(CC)  $(CFLAGS) $(INC) $(LD_FLAGS) $^ -o programBin.elf

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) $^ -g -c -o $@


.PHONY: clean
clean:
	rm -f $(OBJ) *.elf
