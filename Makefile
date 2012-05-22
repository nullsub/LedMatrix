###############################################################################
# Makefile for the project LED_Cube16x4
###############################################################################

## General Flags
PROJECT = 16x16_Matrix
MCU = atmega8
TARGET = Matrix16x16
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS += $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=8000000UL -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=build/matrix16x16.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

INCLUDES= \
	$(END)

CFILES= \
	main.c \
	apps.c \
	pong.c \
	conway.c \
	go.c \
	led_matrix.c \
	uart.c \
	shift_register.c \
	$(END)

## Build
all: build/$(TARGET).elf 

OFILES := $(addprefix build/, $(addsuffix .o,$(basename $(CFILES))))

## Compile

##Link
build/$(TARGET).elf: $(OFILES)
	$(CC) $(LDFLAGS) $(LIBDIRS) $(LIBS) $(OFILES) -o build/$(TARGET).elf  
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS) build/$(TARGET).elf build/$(TARGET).hex
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex build/$(TARGET).elf build/$(TARGET).eep
	avr-objdump -d -S build/$(TARGET).elf >  build/$(TARGET).lss
	@avr-size build/$(TARGET).elf

build/%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

build/%.o : %.S 
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

build/%.o : %.asm 
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

flash: all
	avrdude -c avrisp2 -p m8 -P usb -F -v -V -B10 -U flash:w:build/Matrix16x16.hex:a	

## Clean target
clean:
	rm -rf build/*

# Create output files directory
$(shell mkdir -p build)

