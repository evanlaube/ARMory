SHELL := /bin/zsh

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

# Colors
YELLOW  = \033[1;33m
BLUE    = \033[1;34m
GREEN   = \033[1;32m
RESET   = \033[0m
MAGENTA = \033[1;35m
CYAN    = \033[1;36m
ECHO    = @echo

# Source and object files for libarmory
SRC_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/lib/%.o,$(wildcard $(SRC_DIR)/*.c))

# Example targets
EXAMPLES := $(notdir $(wildcard $(EXAMPLE_DIR)/*))

# Default build target
.PHONY: all
all: $(addprefix $(BUILD_DIR)/,$(addsuffix .bin,$(EXAMPLES)))

# Rule to build the static library
$(BUILD_DIR)/$(LIB_NAME): $(SRC_OBJS)
	$(ECHO) "$(GREEN)[AR]       $@$(RESET)"
	arm-none-eabi-ar rcs $@ $^

# Rule to compile library .c into .o
$(OBJ_DIR)/lib/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(ECHO) "$(YELLOW)[CC]       $<$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

# Template to build examples
define EXAMPLE_template

# Example-specific variables
$(1)_SRC := $$(wildcard $$(EXAMPLE_DIR)/$(1)/*.c)
$(1)_OBJ := $$(patsubst $$(EXAMPLE_DIR)/$(1)/%.c, $$(OBJ_DIR)/examples/$(1)/%.o, $$($(1)_SRC))

# Rule to build the .elf file
$(BUILD_DIR)/$(1).elf: $$($(1)_OBJ) $(BUILD_DIR)/$(LIB_NAME)
	@mkdir -p $$(BUILD_DIR)
	$(ECHO) "$(BLUE)[LD]       $$@$(RESET)"
	$(CC) $(CFLAGS) $$^ $(LDFLAGS) -o $$@

# Rule to build .bin from .elf
$(BUILD_DIR)/$(1).bin: $(BUILD_DIR)/$(1).elf
	$(ECHO) "$(GREEN)[OBJCOPY]  $$@$(RESET)"
	$(OBJCOPY) -O binary $$< $$@

# Rule to compile example .c files into .o files
$$(OBJ_DIR)/examples/$(1)/%.o: $$(EXAMPLE_DIR)/$(1)/%.c
	@mkdir -p $$(dir $$@)
	$(ECHO) "$(YELLOW)[CC]       $$<$(RESET)"
	$(CC) $(CFLAGS) -c $$< -o $$@

endef

# Generate build rules for each example
$(foreach ex,$(EXAMPLES),$(eval $(call EXAMPLE_template,$(ex))))

# Flash a selected example
.PHONY: flash
flash: $(BUILD_DIR)/$(EXAMPLE).bin
	@if [ -z "$(EXAMPLE)" ]; then echo "Set EXAMPLE=name to flash."; exit 1; fi
	$(ECHO) "[FLASH]    $(CYAN)Flashing $(EXAMPLE) with ST-Link$(RESET)"
	$(STFLASH) write $(BUILD_DIR)/$(EXAMPLE).bin 0x8000000
	$(ECHO) "[FLASH]    $(CYAN) Resetting board $(RESET)"
	$(STFLASH) reset

# Clean all build artifacts
.PHONY: clean
clean:
	@echo "$(BLUE)Cleaning build...$(RESET)"
	rm -rf $(BUILD_DIR)

# Help info
.PHONY: help
help:
	@echo "Usage:"
	@echo "  make                 - Build all examples and the library"
	@echo "  make flash EXAMPLE=name - Flash a selected example"
	@echo "  make clean           - Remove build artifacts"

.PHONY: install
install: $(BUILD_DIR)/$(LIB_NAME)
	@echo "$(CYAN)[INSTALL]  Copying library to /usr/local/lib$(RESET)"
	install -Dm644 $< /usr/local/lib/$(LIB_NAME)

	@echo "$(CYAN)[INSTALL]  Copying headers to /usr/local/include/armory$(RESET)"
	mkdir -p /usr/local/include/armory
	cp -r $(INCLUDE_DIR)/armory/* /usr/local/include/armory/

.PHONY: uninstall
uninstall:
	@echo "$(RED)[UNINSTALL]  Removing library $(LIB_NAME) from /usr/local/lib$(RESET)"
	rm -f /usr/local/lib/$(LIB_NAME)

	@echo "$(RED)[UNINSTALL]  Removing headers from /usr/local/include/armory$(RESET)"
	rm -rf /usr/local/include/armory
