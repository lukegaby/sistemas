################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: cpu

dependents:
	-cd /home/lucas/git/tp-2014-1c-codec/commons/Debug && $(MAKE) all
	-cd /home/lucas/git/tp-2014-1c-codec/parser/Debug && $(MAKE) all

# Tool invocations
cpu: $(OBJS) $(USER_OBJS) /home/lucas/git/tp-2014-1c-codec/commons/Debug/libcommons.so /home/lucas/git/tp-2014-1c-codec/parser/Debug/libparser.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/lucas/git/tp-2014-1c-codec/parser/Debug" -L"/home/lucas/git/tp-2014-1c-codec/commons/Debug" -o "cpu" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) cpu
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/lucas/git/tp-2014-1c-codec/commons/Debug/libcommons.so:
/home/lucas/git/tp-2014-1c-codec/parser/Debug/libparser.so:

-include ../makefile.targets
