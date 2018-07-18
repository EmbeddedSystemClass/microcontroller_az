# sharing basic makefile

PRO_DIR		:= .
PROJ_NAME	:= stm32f0_cli_demo
OUTPUT_PATH := $(PRO_DIR)/output

INC_DIR		:= $(PRO_DIR)/driver/inc
SRC_DIR 	:= $(PRO_DIR)/driver/src
LINKER_FILE	:= $(PRO_DIR)/linker/stm32f0.ld

COMPILER_DIR 	:= $(GCC_COMPILER)

PREFIX_GCC_COMPILER	:= arm-none-eabi
CC				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-gcc
ASM				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-as
LD				:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-ld
SIZE			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-size
OBJCPY			:= $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-objcopy

FILE_TO_LINK	:= $(OUTPUT_PATH)/clock.o $(OUTPUT_PATH)/gpio.o $(OUTPUT_PATH)/interrupt.o $(OUTPUT_PATH)/main.o $(OUTPUT_PATH)/startup_ARMCM0.o $(OUTPUT_PATH)/systick.o $(OUTPUT_PATH)/usart.o $(OUTPUT_PATH)/cli.o

CC_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -O0 -g -mthumb -I$(INC_DIR) -I$(PRO_DIR)/lib -I$(PRO_DIR)/lib/cli/inc -I$(PRO_DIR)/app/inc -DCLI_STAND_ALONE
ASM_OPT			:= -march=armv6-m -mcpu=cortex-m0 -c -mthumb
LD_OPT			:= -T $(LINKER_FILE) -Map $(OUTPUT_PATH)/$(PROJ_NAME).map $(FILE_TO_LINK) -L$(COMPILER_DIR)/arm-none-eabi/lib/thumb/v6-m -lc -lnosys

.PHONY: all

all: clean build size

$(OUTPUT_PATH)/clock.o: $(SRC_DIR)/clock.c
	@echo "compile $(SRC_DIR)/clock.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/clock.c -o $(OUTPUT_PATH)/clock.o

$(OUTPUT_PATH)/gpio.o: $(SRC_DIR)/gpio.c
	@echo "compile $(SRC_DIR)/gpio.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/gpio.c -o $(OUTPUT_PATH)/gpio.o

$(OUTPUT_PATH)/interrupt.o: $(SRC_DIR)/interrupt.c
	@echo "compile $(SRC_DIR)/interrupt.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/interrupt.c -o $(OUTPUT_PATH)/interrupt.o

$(OUTPUT_PATH)/main.o: $(PRO_DIR)/app/src/main.c
	@echo "compile $(PRO_DIR)/app/src/main.c file"
	$(CC) $(CC_OPT) $(PRO_DIR)/app/src/main.c -o $(OUTPUT_PATH)/main.o

$(OUTPUT_PATH)/startup_ARMCM0.o: $(PRO_DIR)/app/src/startup_ARMCM0.S
	@echo "compile $(PRO_DIR)/app/src/startup_ARMCM0.S file"
	$(ASM) $(ASM_OPT) $(PRO_DIR)/app/src/startup_ARMCM0.s -o $(OUTPUT_PATH)/startup_ARMCM0.o

$(OUTPUT_PATH)/systick.o: $(SRC_DIR)/systick.c
	@echo "compile $(SRC_DIR)/systick.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/systick.c -o $(OUTPUT_PATH)/systick.o

$(OUTPUT_PATH)/usart.o: $(SRC_DIR)/usart.c
	@echo "compile $(SRC_DIR)/usart.c file"
	$(CC) $(CC_OPT) $(SRC_DIR)/usart.c -o $(OUTPUT_PATH)/usart.o

$(OUTPUT_PATH)/cli.o: $(PRO_DIR)/lib/cli/src/cli.c
	@echo "compile $(PRO_DIR)/lib/cli/src/cli.c file"
	$(CC) $(CC_OPT) $(PRO_DIR)/lib/cli/src/cli.c -o $(OUTPUT_PATH)/cli.o

size: $(OUTPUT_PATH)/$(PROJ_NAME).elf
	$(SIZE) $(OUTPUT_PATH)/$(PROJ_NAME).elf

hex: $(OUTPUT_PATH)/$(PROJ_NAME).elf
	$(OBJCPY) -O ihex $(OUTPUT_PATH)/$(PROJ_NAME).elf $(OUTPUT_PATH)/$(PROJ_NAME).hex

build: $(FILE_TO_LINK) $(LINKER_FILE)
	@echo "link object files to create new binary image ($(PROJ_NAME).elf)"
	$(LD) $(LD_OPT) -o $(OUTPUT_PATH)/$(PROJ_NAME).elf

clean:
	@rm -rf $(OUTPUT_PATH)/*
