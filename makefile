##########################################################################
#
# Build variables
#
##########################################################################

BUILD_NAME = RV_SoC_c_demo
BUILD_DIR = ./build
BINARY_PATH = $(BUILD_DIR)/bin

##########################################################################
#
# Build system
#
##########################################################################

ifeq ($(OS), Windows_NT)
	PRESET = C:\Windows\sysnative\wsl.exe bash -ic "
	ENDSET = "
	MKDIR = if not exist $(@D) mkdir $(subst /,\,$(@D))
else
	MKDIR = @mkdir -p $(@D)
endif

##########################################################################
#
# Sources
#
##########################################################################
-include Makefile.include

##########################################################################
#
# Compiler definitions
#
##########################################################################

CPPCOMPILER 	= $(CROSS_COMPILE)g++
CCOMPILER		= $(CROSS_COMPILE)gcc
ASSEMBLER 		= $(CROSS_COMPILE)as
LD				= $(CROSS_COMPILE)g++
OBJCOPY			= $(CROSS_COMPILE)objcopy
OBJDUMP			= $(CROSS_COMPILE)objdump
SIZE			= $(CROSS_COMPILE)size

##########################################################################
#
# Compiler flags
#
##########################################################################

OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o,$(patsubst %.c, $(BUILD_DIR)/%.o, $(patsubst %.S, $(BUILD_DIR)/%.o, $(SRC))))

##########################################################################
#
# Make Targets
#
##########################################################################
all: $(BINARY_PATH)/$(BUILD_NAME).elf

$(BINARY_PATH)/$(BUILD_NAME).elf: $(OBJECTS)
	@echo 'Building target: $@'
	$(MKDIR)	
	$(PRESET) $(LD) $(OBJECTS) -T $(LINKER)  $(LDFLAGS) -Wl,-Map=$(BINARY_PATH)/$(BUILD_NAME).map -o $@ $(ENDSET)
	$(PRESET) $(OBJDUMP) --source --all-headers --demangle --line-numbers --wide "$(BINARY_PATH)/$(BUILD_NAME).elf" > "$(BINARY_PATH)/$(BUILD_NAME).lst" $(ENDSET)
	$(PRESET) $(OBJCOPY) -O ihex "$(BINARY_PATH)/$(BUILD_NAME).elf" "$(BINARY_PATH)/$(BUILD_NAME).hex" $(ENDSET)
	$(PRESET) $(SIZE) --format=berkeley "$(BINARY_PATH)/$(BUILD_NAME).elf" $(ENDSET)

$(BUILD_DIR)/%.o: %.cpp	
	@echo "Compiling Source $< into object $@"
	$(MKDIR)
	$(PRESET) $(CPPCOMPILER) $(CPLUSPLUSFLAGS) -c $< -o $@ $(ENDSET)

$(BUILD_DIR)/%.o: %.c	
	@echo "Compiling Source $< into object $@"
	$(MKDIR)
	$(PRESET) $(CCOMPILER) $(CFLAGS) -c $< -o $@ $(ENDSET)

$(BUILD_DIR)/%.o: %.S
	@echo "Compiling Source $< into object $@"
	$(MKDIR)
	$(PRESET) $(CCOMPILER) $(CFLAGS) -c $< -o $@ $(ENDSET)

.PHONY: clean
clean:
	$(PRESET) rm -r -f $(BUILD_DIR) all $(ENDSET)
