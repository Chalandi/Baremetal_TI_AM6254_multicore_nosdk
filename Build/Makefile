# ******************************************************************************************
#   Filename    : Makefile
#
#   Author      : Chalandi Amine
#
#   Owner       : Chalandi Amine
#
#   Date        : 22.11.2022
#
#   Description : Build system
#
# ******************************************************************************************

############################################################################################
# Defines
############################################################################################
PRJ_NAME    = $(CORE_FAMILY)_baremetal_am6254_nosdk
OUTPUT_DIR  = ../Output
OBJ_DIR     = $(OUTPUT_DIR)/$(CORE_FAMILY)_obj
LD_SCRIPT   = $(SRC_DIR)/Startup/Core/$(CORE_FAMILY)/Memory_Map.ld
SRC_DIR     = ../Code
RTOS        = #osek
ALLOWED_CORES := a53 m4f r5f

ifeq ($(OS),Windows_NT)
    PYTHON     = python
else
    PYTHON     = python3
endif

ERR_MSG_FORMATER_SCRIPT = ../Tools/scripts/CompilerErrorFormater.py
LINKER_ERR_MSG_FORMATER_SCRIPT = ../Tools/scripts/LinkerErrorFormater.py
FORMAT_LINKER_ERR =
############################################################################################
# Toolchain
############################################################################################

ifeq ($(CORE_FAMILY), a53)
             TOOLCHAIN = aarch64-none-elf
             ARCH      = -mcpu=cortex-a53 -march=armv8-a -mabi=lp64
             DEFS      =
             SPEC      = --specs=nosys.specs

else ifeq ($(CORE_FAMILY), m4f)
    TOOLCHAIN          = arm-none-eabi
    ARCH               = -mcpu=cortex-m4 -mthumb  -mfloat-abi=softfp
    DEFS               =
    SPEC               = --specs=nano.specs  --specs=nosys.specs

else
    TOOLCHAIN          = arm-none-eabi
    ARCH               = -mcpu=cortex-r5 -marm -mfloat-abi=hard -mfpu=vfpv3-d16
    DEFS               =
    SPEC               = --specs=nano.specs  --specs=nosys.specs

endif

AS      = $(TOOLCHAIN)-gcc
CC      = $(TOOLCHAIN)-gcc
CPP     = $(TOOLCHAIN)-g++
LD      = $(TOOLCHAIN)-gcc
OBJDUMP = $(TOOLCHAIN)-objdump
OBJCOPY = $(TOOLCHAIN)-objcopy
SIZE    = $(TOOLCHAIN)-size
READELF = $(TOOLCHAIN)-readelf

############################################################################################
# Optimization Compiler flags
############################################################################################

OPT_MODIFIED_O2 = -O2                               \
                  -fno-reorder-blocks-and-partition \
                  -fno-reorder-functions

NO_OPT = -O0

OPT = $(NO_OPT)

############################################################################################
# GCC Compiler verbose flags
############################################################################################

VERBOSE_GCC = -frecord-gcc-switches -fverbose-asm

############################################################################################
# C Compiler flags
############################################################################################

COPS  = $(OPT)                                        \
        $(ARCH)                                       \
        $(DEFS)                                       \
        -ffreestanding                                \
        -MD                                           \
        -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
        -g3                                           \
        -Wconversion                                  \
        -Wsign-conversion                             \
        -Wunused-parameter                            \
        -Wuninitialized                               \
        -Wmissing-declarations                        \
        -Wshadow                                      \
        -Wunreachable-code                            \
        -Wmissing-include-dirs                        \
        -x c                                          \
        -std=c11                                      \
        -Wall                                         \
        -Wextra                                       \
        -fomit-frame-pointer                          \
        -gdwarf-2                                     \
        -fno-exceptions

############################################################################################
# C++ Compiler flags
############################################################################################

CPPOPS  = $(OPT)                                        \
          $(ARCH)                                       \
          $(DEFS)                                       \
          -ffreestanding                                \
          -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
          -g3                                           \
          -Wconversion                                  \
          -Wsign-conversion                             \
          -Wunused-parameter                            \
          -Wuninitialized                               \
          -Wmissing-declarations                        \
          -Wshadow                                      \
          -Wunreachable-code                            \
          -Wmissing-include-dirs                        \
          -Wall                                         \
          -Wextra                                       \
          -fomit-frame-pointer                          \
          -gdwarf-2                                     \
          -fno-exceptions                               \
          -x c++                                        \
          -fno-rtti                                     \
          -fno-use-cxa-atexit                           \
          -fno-nonansi-builtins                         \
          -fno-threadsafe-statics                       \
          -fno-enforce-eh-specs                         \
          -ftemplate-depth=128                          \
          -Wzero-as-null-pointer-constant

############################################################################################
# Assembler flags
############################################################################################
ifeq ($(AS), $(TOOLCHAIN)-as)
  ASOPS =  $(ARCH)       \
           -alh          \
           -g
else
  ASOPS = $(OPT)                                        \
          $(ARCH)                                       \
          $(DEFS)                                       \
          -MD                                           \
          -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
          -g3                                           \
          -Wconversion                                  \
          -Wsign-conversion                             \
          -Wunused-parameter                            \
          -Wuninitialized                               \
          -Wmissing-declarations                        \
          -Wshadow                                      \
          -Wunreachable-code                            \
          -Wmissing-include-dirs                        \
          -x assembler                                  \
          -std=c11                                      \
          -Wall                                         \
          -Wextra                                       \
          -fomit-frame-pointer                          \
          -gdwarf-2                                     \
          -fno-exceptions
endif

############################################################################################
# Linker flags
############################################################################################

ifeq ($(LD), $(TOOLCHAIN)-ld)
  LOPS = -nostartfiles                          \
         -nostdlib                              \
         $(ARCH)                                \
         $(DEFS)                                \
         -e Startup_Init                        \
         --print-memory-usage                   \
         --print-map                            \
         -dT $(LD_SCRIPT)                       \
         -Map=$(OUTPUT_DIR)/$(PRJ_NAME).map     \
         --no-warn-rwx-segments                 \
         -z,max-page-size=4096                  \
         $(SPEC)
else
  LOPS = -nostartfiles                          \
         -nostdlib                              \
         $(ARCH)                                \
         $(DEFS)                                \
         -e Startup_Init                        \
         -Wl,--print-memory-usage               \
         -Wl,--print-map                        \
         -Wl,-dT $(LD_SCRIPT)                   \
         -Wl,-Map=$(OUTPUT_DIR)/$(PRJ_NAME).map \
         -Wl,--no-warn-rwx-segments             \
         -Wl,-z,max-page-size=4096              \
         $(SPEC)
endif

############################################################################################
# Source Files
############################################################################################

SRC_FILES := $(SRC_DIR)/Appli/Core/$(CORE_FAMILY)/main.c                     \
             $(SRC_DIR)/Mcal/Timer/timer.c                                   \
             $(SRC_DIR)/Mcal/VIM/vim.c                                       \
             $(SRC_DIR)/Startup/Core/$(CORE_FAMILY)/Startup.c                \
             $(SRC_DIR)/Std/StdLib.c

ifneq ($(CORE_FAMILY), m4f)
    SRC_FILES += $(SRC_DIR)/Startup/Core/$(CORE_FAMILY)/boot.s
    SRC_FILES += $(SRC_DIR)/Startup/Core/$(CORE_FAMILY)/intvect.s
else
    SRC_FILES += $(SRC_DIR)/Startup/Core/$(CORE_FAMILY)/IntVect.c
    SRC_FILES += $(SRC_DIR)/Mcal/SysTickTimer/SysTickTimer.c
endif





############################################################################################
# Include Paths
############################################################################################
INC_FILES := $(SRC_DIR)                             \
             $(SRC_DIR)/Appli/                      \
             $(SRC_DIR)/Appli/Core/$(CORE_FAMILY)   \
             $(SRC_DIR)/Mcal                        \
             $(SRC_DIR)/Mcal/Gpio                   \
             $(SRC_DIR)/Mcal/SysTickTimer           \
             $(SRC_DIR)/Mcal/Timer                  \
             $(SRC_DIR)/Mcal/VIM                    \
             $(SRC_DIR)/Startup                     \
             $(SRC_DIR)/Startup/Core/$(CORE_FAMILY) \
             $(SRC_DIR)/Std


############################################################################################
# RTOS Files
############################################################################################
ifeq ($(RTOS),osek)
 OS_CORE_FAMILY = ARMv8-A
 OS_OIL_FILE = $(SRC_DIR)/Appli/osek_am6254_a53.oil
 OS_TCB_GEN_FOLDER = $(SRC_DIR)/Appli/gen
 include $(SRC_DIR)/OSEK/Os.makefile
 SRC_FILES += $(SRC_DIR)/Appli/tasks.c
 SRC_FILES += $(OS_TCB_GEN_FOLDER)/OsTcb.c
 INC_FILES += $(OS_TCB_GEN_FOLDER)
 DEFS      += -DOSEK
endif

############################################################################################
# Rules
############################################################################################

VPATH := $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))

FILES_O := $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(basename $(SRC_FILES)))))



ifeq ($(MAKECMDGOALS), BUILD_STAGE_2)
-include $(subst .o,.d,$(FILES_O))
endif

REBUILD_STAGE_1 : CLEAN PRE_BUILD  $(CUSTOM_PRE_BUILD_TARGET)
REBUILD_STAGE_2 : link
REBUILD_STAGE_3 : GENERATE POST_BUILD

BUILD_STAGE_1   : PRE_BUILD  $(CUSTOM_PRE_BUILD_TARGET)
BUILD_STAGE_2   : link
BUILD_STAGE_3   : GENERATE POST_BUILD

############################################################################################
# Recipes
############################################################################################
.PHONY : link
link : $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@-echo "" > /dev/null


.PHONY : PRE_BUILD
PRE_BUILD:
	@$(if $(filter $(CORE_FAMILY),$(ALLOWED_CORES)),,$(error Error: the Entered CORE_FAMILY '$(CORE_FAMILY)' is not supported! Allowed values are: $(ALLOWED_CORES)))
	@-echo +++ Building AM6254 baremetal image for ARM Cortex-$(shell echo $(CORE_FAMILY) | tr a-z A-Z) core
	@git log -n 1 --decorate-refs=refs/heads/ --pretty=format:"+++ Git branch: %D (%h)" 2>/dev/null || true
	@git log -n 1 --clear-decorations 2> /dev/null > /dev/null || true
	@echo +++ info: "$(shell $(CC) -v 2>&1 | tail -n 1)"
	@echo +++ info: "$(shell make -v 2>&1 | head -n 1)"
	@$(if $(shell test -d $(OBJ_DIR) && echo yes),,mkdir -p $(subst \,/,$(OBJ_DIR)))

.PHONY : POST_BUILD
POST_BUILD:
	@-echo +++ End

.PHONY : CLEAN
CLEAN :
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).bin     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).dis     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).elf     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).hex     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).map     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).readelf 2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).sym     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).uf2     2>/dev/null || true
	@-rm -rf $(OBJ_DIR)                        2>/dev/null || true
	@-mkdir -p $(subst \,/,$(OUTPUT_DIR))

$(OBJ_DIR)/%.o : %.c
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

ifeq ($(AS), $(TOOLCHAIN)-as)
$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR
else
$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(AS) $(ASOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR
endif

$(OBJ_DIR)/%.o : %.cpp
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O) $(LD_SCRIPT)
	@-echo +++ link: $(subst \,/,$@)
ifeq ($(FORMAT_LINKER_ERR), )
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
else
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf 2> $(OBJ_DIR)/linker.err || true
	@-$(PYTHON) $(LINKER_ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/linker.err
endif

.PHONY : GENERATE
GENERATE:
	@$(if $(wildcard $(OUTPUT_DIR)/$(PRJ_NAME).elf), ,$(error Error: Link not succeeded !))
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).readelf
	@$(READELF) -WhS $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).readelf
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).sym
	@$(READELF) -Ws $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).sym
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).dis
	@$(OBJDUMP) -d --wide $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).dis
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).bin
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O binary $(OUTPUT_DIR)/$(PRJ_NAME).bin
ifneq ($(OS_SIZE_SCRIPT), )
	@-echo  
	@$(PYTHON) $(OS_SIZE_SCRIPT) $(OUTPUT_DIR)/$(PRJ_NAME).map --sections .text .data .bss
	@-echo  
endif
