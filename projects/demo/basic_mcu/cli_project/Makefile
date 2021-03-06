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


PRO_DIR				:= ../../../../projects
BOARD_NAME			:= stm32f051r8
PROJ_NAME			:= cli_project


DEMO_DIR			:= $(PRO_DIR)/demo
HAL_DIR				:= $(PRO_DIR)/hal
LIB_DIR				:= $(PRO_DIR)/lib
BOARD_DIR			:= $(PRO_DIR)/board/$(BOARD_NAME)

# output building folder
BUILD_DIR 			:= ./output


# list folder contains C language files
CC_DIRS				+= $(DEMO_DIR)/basic_mcu/$(PROJ_NAME)/src
CC_DIRS				+= $(HAL_DIR)/stm32/src
CC_DIRS				+= $(LIB_DIR)/cli/src
CC_DIRS				+= $(LIB_DIR)/common/src
CC_DIRS				+= $(BOARD_DIR)/src


# list folder contains ASM language files
ASM_DIRS			+= $(BOARD_DIR)/src

# include folders:
INC_DIRS			+= $(DEMO_DIR)/basic_mcu/$(PROJ_NAME)/inc
INC_DIRS			+= $(HAL_DIR)/stm32/inc
INC_DIRS			+= $(LIB_DIR)/cli/inc
INC_DIRS			+= $(LIB_DIR)/common/inc
INC_DIRS			+= $(BOARD_DIR)/inc

# linker file
LINKER_FILE			:= $(BOARD_DIR)/linker/stm32f0.ld

# setting compiler option
CC_OPT			= -march=armv6-m -mcpu=cortex-m0 -c -g -mthumb $(INC_DIR) -DCLI_STAND_ALONE
ASM_OPT			= -march=armv6-m -mcpu=cortex-m0 -c -mthumb --defsym __STARTUP_CLEAR_BSS=1 --defsym __STACK_SIZE=0x300 --defsym __HEAP_SIZE=0x800
LD_OPT			= -T $(LINKER_FILE) -Map $(BUILD_DIR)/$(PROJ_NAME).map $(OBJECT_FILE) -L $(COMPILER_DIR)/arm-none-eabi/lib/thumb/v6-m -lc_nano -lnosys

# include build system to use common taget rules:
BUILD_SYSTEM		:= $(PRO_DIR)/build_system
include	$(BUILD_SYSTEM)/common.mk

