# Toolchain
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
STFLASH  = st-flash

# Paths
SRC_DIR      = src
EXAMPLE_DIR  = examples
INCLUDE_DIR  = include
BUILD_DIR    = build
OBJ_DIR      = $(BUILD_DIR)/obj
LIB_NAME     = libarmory.a
LD_SCRIPT    = linker.ld

# Flags
CFLAGS  = -mcpu=cortex-m4 -mthumb -Wall -nostdlib -nostartfiles -O0 -g -I$(INCLUDE_DIR)
LDFLAGS = -T$(LD_SCRIPT)

# Source and object files for libarmory
SRC_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/lib/%.o,$(wildcard $(SRC_DIR)/*.c))

# Example targets
EXAMPLES := $(notdir $(wildcard $(EXAMPLE_DIR)/*))

# Default build target
.PHONY: all
all: $(addprefix $(BUILD_DIR)/,$(addsuffix .bin,$(EXAMPLES)))

# Rule to build the static library
$(BUILD_DIR)/$(LIB_NAME): $(SRC_OBJS)
	@mkdir -p $(dir $@)
	arm-none-eabi-ar rcs $@ $^

# Rule to compile library .c into .o
$(OBJ_DIR)/lib/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Template to build examples
define EXAMPLE_template

# Example-specific variables
$(1)_SRC := $$(wildcard $$(EXAMPLE_DIR)/$(1)/*.c)
$(1)_OBJ := $$(patsubst $$(EXAMPLE_DIR)/$(1)/%.c, $$(OBJ_DIR)/examples/$(1)/%.o, $$($(1)_SRC))

# Rule to build the .elf file
$(BUILD_DIR)/$(1).elf: $$($(1)_OBJ) $(BUILD_DIR)/$(LIB_NAME)
	@mkdir -p $$(BUILD_DIR)
	$(CC) $$(CFLAGS) $$^ $(LDFLAGS) -o $$@

# Rule to build .bin from .elf
$(BUILD_DIR)/$(1).bin: $(BUILD_DIR)/$(1).elf
	$(OBJCOPY) -O binary $$< $$@

# Rule to compile example .c files into .o files
$$(OBJ_DIR)/examples/$(1)/%.o: $$(EXAMPLE_DIR)/$(1)/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $$(CFLAGS) -c $$< -o $$@

endef

# Generate build rules for each example
$(foreach ex,$(EXAMPLES),$(eval $(call EXAMPLE_template,$(ex))))

# Flash a selected example
.PHONY: flash
flash:
	@if [ -z "$(EXAMPLE)" ]; then echo "Set EXAMPLE=name to flash."; exit 1; fi
	$(STFLASH) write $(BUILD_DIR)/$(EXAMPLE).bin 0x8000000

# Clean all build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
