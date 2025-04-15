TARGET = test
BUILD_DIR = build
SRC_DIR = src
ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = $(BUILD_DIR)/$(TARGET).bin

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
STFLASH = st-flash

C_SOURCES = $(wildcard $(SRC_DIR)/*.c) 
LD_SCRIPT = linker.ld

CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -nostdlib -nostartfiles -O0
CFLAGS += -I$(shell arm-none-eabi-gcc -print-file-name=include)
CFLAGS += -I$(shell arm-none-eabi-gcc -print-file-name=include-fixed)
LDFLAGS = -T$(LD_SCRIPT)

all: $(BIN)

$(BUILD_DIR):
	$(info =============== Creating build directory ===============)
	mkdir -p $@

$(ELF): $(C_SOURCES) $(LD_SCRIPT) | $(BUILD_DIR)
	$(info ===============     Compiling to ELF     ===============)
	$(CC) $(CFLAGS) $(C_SOURCES)  $(LDFLAGS) -o $@

$(BIN): $(ELF)
	$(info ===============     Compiling Binary     ===============)
	$(OBJCOPY) -O binary $< $@

flash: $(BIN)
	$(info ===============     Compiling Binary     ===============)
	$(STFLASH) write $< 0x8000000

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean
