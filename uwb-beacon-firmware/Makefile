##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16  -fstack-usage
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu99
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
  USE_CPPOPT +=  -std=gnu++11
  USE_CPPOPT += -fno-exceptions -fno-unwind-tables -fno-threadsafe-statics
  USE_CPPOPT += -DEIGEN_NO_DEBUG
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = no
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x400
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

# Enables the use of FPU on Cortex-M4 (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = hard
endif

ifeq ($(USE_BOOTLOADER),)
  USE_BOOTLOADER = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = uwb-beacon

# Imported source files and paths
CHIBIOS = ../lib/ChibiOS
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/test/rt/test.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk
include $(CHIBIOS)/os/various/shell/shell.mk

include app_src.mk

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(STARTUPSRC) \
	   $(KERNSRC) \
	   $(PORTSRC) \
	   $(OSALSRC) \
	   $(HALSRC) \
	   $(PLATFORMSRC) \
	   $(SHELLSRC) \
       $(CHIBIOS)/os/hal/lib/streams/chprintf.c \
	   $(APPCSRC)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(CHCPPSRC) $(APPCPPSRC)

# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM) $(APPASMSRC)

INCDIR = $(STARTUPINC) \
		 $(KERNINC) \
		 $(PORTINC) \
		 $(OSALINC) \
		 $(HALINC) \
		 $(PLATFORMINC) \
		 $(APPINC) \
		 $(CHCPPINC) \
		 $(SHELLINC) \
	 	 $(TESTINC) \
         $(CHIBIOS)/os/hal/lib/streams \
		 $(CHIBIOS)/os/license \
		 $(CHIBIOS)/os/various

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
#LD   = $(TRGT)gcc
LD   = $(TRGT)g++
CP   = $(TRGT)objcopy -j startup -j constructors -j destructors -j .text \
		-j .ARM.extab -j .ARM.exidx -j .eh_frame_hdr -j .eh_frame -j .textalign -j .data
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra -Wundef

#
# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# UAVCAN configuration
UDEFS += -DUAVCAN_STM32_CHIBIOS=1 \
		 -DUAVCAN_TOSTRING=0 \
		 -DUAVCAN_DEBUG=0 \
		 -DUAVCAN_STM32_TIMER_NUMBER=2 \
		 -DUAVCAN_STM32_NUM_IFACES=1 \
		 -DSHELL_CONFIG_FILE


include ../lib/uavcan/libuavcan/include.mk
include ../lib/uavcan/libuavcan_drivers/stm32/driver/include.mk

CPPSRC += $(LIBUAVCAN_SRC) $(LIBUAVCAN_STM32_SRC)
UINCDIR += $(LIBUAVCAN_INC) $(LIBUAVCAN_STM32_INC) ./dsdlc_generated

#
# End of user defines
##############################################################################

ifeq ($(USE_BOOTLOADER), yes)
  UDEFS += -DCORTEX_VTOR_INIT=0x0800C000
  LDSCRIPT = STM32F405xG_bootloader.ld
  DDEFS += -DCONFIG_ADDR=0x08004000 -DCONFIG_PAGE_SIZE=0x4000
else
  LDSCRIPT = STM32F405xG.ld
endif

GLOBAL_SRC_DEP = app_src.mk

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/
include $(RULESPATH)/rules.mk

# run uavcan dsdl compiler
.PHONY: dsdlc
dsdlc:
	@$(COLOR_PRINTF) "Running uavcan dsdl compiler"
	$(LIBUAVCAN_DSDLC) ../uavcan_data_types/cvra $(UAVCAN_DSDL_DIR)


.PHONY: flash
flash: build/$(PROJECT).elf
	openocd -f oocd.cfg -c "program build/$(PROJECT).elf verify reset" -c "shutdown"

.PHONY: ctags
ctags:
	@echo "Generating ctags file..."
	@cat .dep/*.d | grep ":$$" | sed "s/://" | sort | uniq | xargs ctags --file-scope=no --extra=+qf $(CSRC) $(CPPSRC)

.PHONY: dfu
dfu: build/$(PROJECT).bin
	dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000 -D build/$(PROJECT).bin
