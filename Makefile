# Project
TARGET = Motor_Control
MCU    = cortex-m4

# Folders
SRC_DIR     = src
INC_DIR     = inc
STARTUP_DIR = startup
CMSIS_DIR   = CMSIS/Include
CMSIS_DEVICE_DIR = CMSIS/Device/ST/STM32F4xx/Include

# Linker
LD_SCRIPT = stm32f4xx_linker.ld

# Tools
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size
GDB     = gdb-multiarch

# Compile flags
CFLAGS  = -mcpu=$(MCU) -mthumb -Wall -g -O0 -ffunction-sections -fdata-sections \
           -I$(INC_DIR) -I$(CMSIS_DIR) -I$(CMSIS_DEVICE_DIR) -nostdlib \
           -DSTM32F411xE 

LDFLAGS = -mcpu=$(MCU) -mthumb -T $(LD_SCRIPT) -Wl,--gc-sections

# Source files
SRC  = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(STARTUP_DIR)/*.c)
OBJS = $(SRC:.c=.o)

# Targets
all: $(TARGET).elf $(TARGET).bin size

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Convert ELF to BIN
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Show size
size: $(TARGET).elf
	$(SIZE) $<

# Flash (erase + program)
flash: $(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# Debug using multiarch gdb
debug: $(TARGET).elf
	# Start OpenOCD first in another terminal or background
	$(GDB) $(TARGET).elf \
		-ex "set architecture arm" \
		-ex "target extended-remote localhost:3333" \
		-ex "monitor reset halt" \
		-ex "layout split" \
		-ex "load"

# Clean
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin