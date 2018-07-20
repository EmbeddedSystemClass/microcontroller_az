# 
# The MIT License (MIT)
# 
# Copyright (c) 2018 Anh Vo Tuan <votuananhs@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies
# or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
# AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# 
# build system for C project

PRO_DIR		:= .
PROJ_NAME	?= stm32f0_cli_demo
BUILD_DIR 	:= $(PRO_DIR)/output

INC_DIR		:= -I$(PRO_DIR)/driver/inc -I$(PRO_DIR)/lib -I$(PRO_DIR)/lib/cli/inc -I$(PRO_DIR)/app/inc
LINKER_FILE	:= $(PRO_DIR)/linker/stm32f0.ld

COMPILER_DIR 	:= $(GCC_COMPILER)

PREFIX_GCC_COMPILER	:= arm-none-eabi
CC				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-gcc
ASM				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-as
LD				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-ld
SIZE			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-size
OBJCPY			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-objcopy
NM				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-nm

CC_FILE			:= $(shell find $(PRO_DIR) -name *.c)
ASM_FILE		:= $(shell find $(PRO_DIR) -name *.s -or -name *.S)
BUILD_FILE		:= $(CC_FILE) $(ASM_FILE)
OBJECT_FILE		:= $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(BUILD_FILE)))))
DEPS 			:= $(OBJECT_FILE:.o=.d)

CC_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -g -mthumb $(INC_DIR) -DCLI_STAND_ALONE
ASM_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -mthumb --defsym __STARTUP_CLEAR_BSS=1 --defsym __STACK_SIZE=0x300 --defsym __HEAP_SIZE=0x800
LD_OPT			:= -T $(LINKER_FILE) -Map $(BUILD_DIR)/$(PROJ_NAME).map $(OBJECT_FILE) -L $(COMPILER_DIR)/arm-none-eabi/lib/thumb/v6-m -lc_nano -lnosys

.PHONY: all

all: clean build size


# generate object file of c file
ifndef generate_c_rule
define generate_c_rule
$(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(1))))): $(1)
	@echo Processing $$<
	$(CC) $(CC_OPT) $$< -o $$@
endef
endif # generate_c_rule

# generate object file of c file
ifndef generate_asm_rule
define generate_asm_rule
$(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(notdir $(1))))): $(1)
	@echo Processing $$<
	$(ASM) $(ASM_OPT) $$< -o $$@
endef
endif # generate_asm_rule

#generate all c and asm file of project
$(foreach file,$(CC_FILE),$(eval $(call generate_c_rule,$(file))))
$(foreach file,$(ASM_FILE),$(eval $(call generate_asm_rule,$(file))))

size: $(BUILD_DIR)/$(PROJ_NAME).elf
	@echo "Size area memories in binary image $(BUILD_DIR)/$(PROJ_NAME).elf"
	$(SIZE) $(BUILD_DIR)/$(PROJ_NAME).elf

d_size: $(BUILD_DIR)/$(PROJ_NAME).elf
	$(NM) --demangle --print-size --size-sort --reverse-sort -S $(BUILD_DIR)/$(PROJ_NAME).elf

show_flag:
	@echo $(CC_FILE)
	@echo $(ASM_FILE)
	@echo $(OBJECT_FILE)
	@echo $(CC_OPT)
	@echo $(ASM_OPT)
	@echo $(LD_OPT)

hex: $(BUILD_DIR)/$(PROJ_NAME).elf
	$(OBJCPY) -O ihex $(BUILD_DIR)/$(PROJ_NAME).elf $(BUILD_DIR)/$(PROJ_NAME).hex

build: $(OBJECT_FILE)
	@echo "Linking object files to create new binary image $(BUILD_DIR)/$(PROJ_NAME).elf"
	$(LD) $(LD_OPT) -o $(BUILD_DIR)/$(PROJ_NAME).elf

clean:
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

-include $(DEPS)


