EXAMPLE ?= breakout

ifeq ($(filter-out breakout rgbLed,$(EXAMPLE)),)
else
$(error Unknown EXAMPLE '$(EXAMPLE)')
endif

BUILD_DIR = build
SRC_DIR = src
EXAMPLE_DIR = examples/$(EXAMPLE)
TARGET = $(EXAMPLE)
ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = $(BUILD_DIR)/$(TARGET).bin

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
STFLASH = st-flash

# Source files
SRC_PATHS = $(SRC_DIR) $(EXAMPLE_DIR)
C_SOURCES = $(wildcard $(addsuffix /*.c,$(SRC_PATHS)))

SRCS := $(wildcard $(addsuffix /*.c,$(SRC_PATHS)))

# Exclude src/main.c if example defines its own main
ifneq ($(strip $(shell grep -l "int main" $(EXAMPLE_DIR)/*.c 2>/dev/null)),)
SRCS := $(filter-out $(SRC_DIR)/main.c,$(SRCS))
endif

# Check if example has its own main(), and exclude src/main.c if so
ifneq ($(strip $(shell grep -l "int main" $(EXAMPLE_DIR)/*.c 2>/dev/null)),)
C_SOURCES := $(filter-out $(SRC_DIR)/main.c,$(C_SOURCES))
endif

LD_SCRIPT = linker.ld

CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -nostdlib -nostartfiles -O0 -g
CFLAGS += -I$(shell $(CC) -print-file-name=include)
CFLAGS += -I$(shell $(CC) -print-file-name=include-fixed)
CFLAGS += -Isrc -I$(EXAMPLE_DIR)
LDFLAGS = -T$(LD_SCRIPT)

all: $(BIN)

$(BUILD_DIR):
	$(info =============== Creating build directory ===============)
	mkdir -p $@

$(ELF): $(C_SOURCES) $(LD_SCRIPT) | $(BUILD_DIR)
	$(info ===============     Compiling to ELF     ===============)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

$(BIN): $(ELF)
	$(info ===============     Compiling Binary     ===============)
	$(OBJCOPY) -O binary $< $@

flash: $(BIN)
	$(info ===============     Flashing Binary      ===============)
	$(STFLASH) write $< 0x8000000
	$(STFLASH) reset

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean

