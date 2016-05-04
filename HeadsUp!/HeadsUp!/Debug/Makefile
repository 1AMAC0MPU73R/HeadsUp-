################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../HEADERS \
../SOURCES \
../REFERENCES


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../SOURCES/bat.cpp \
../SOURCES/but.cpp \
../SOURCES/headsup.cpp \
../SOURCES/lcd.cpp \
../SOURCES/led.cpp \
../SOURCES/rtc.cpp \
../SOURCES/time.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
SOURCES/bat.o \
SOURCES/but.o \
SOURCES/headsup.o \
SOURCES/lcd.o \
SOURCES/led.o \
SOURCES/rtc.o \
SOURCES/time.o

OBJS_AS_ARGS +=  \
SOURCES/bat.o \
SOURCES/but.o \
SOURCES/headsup.o \
SOURCES/lcd.o \
SOURCES/led.o \
SOURCES/rtc.o \
SOURCES/time.o

C_DEPS +=  \
SOURCES/bat.d \
SOURCES/but.d \
SOURCES/headsup.d \
SOURCES/lcd.d \
SOURCES/led.d \
SOURCES/rtc.d \
SOURCES/time.d

C_DEPS_AS_ARGS +=  \
SOURCES/bat.d \
SOURCES/but.d \
SOURCES/headsup.d \
SOURCES/lcd.d \
SOURCES/led.d \
SOURCES/rtc.d \
SOURCES/time.d

OUTPUT_FILE_PATH +=HeadsUp!.elf

OUTPUT_FILE_PATH_AS_ARGS +=HeadsUp!.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler















SOURCES/%.o: ../SOURCES/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 4.9.2
	$(QUOTE)C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\PROGRAMS\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include" -I"../HEADERS" -I"../SOURCES"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g3 -Wall -mmcu=atmega1284p -B "C:\PROGRAMS\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega1284p" -c -std=c++11 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 4.9.2
	$(QUOTE)C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="HeadsUp!.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atmega1284p -B "C:\PROGRAMS\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega1284p"  
	@echo Finished building target: $@
	"C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "HeadsUp!.elf" "HeadsUp!.hex"
	"C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "HeadsUp!.elf" "HeadsUp!.eep" || exit 0
	"C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "HeadsUp!.elf" > "HeadsUp!.lss"
	"C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "HeadsUp!.elf" "HeadsUp!.srec"
	"C:\PROGRAMS\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "HeadsUp!.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "HeadsUp!.elf" "HeadsUp!.a" "HeadsUp!.hex" "HeadsUp!.lss" "HeadsUp!.eep" "HeadsUp!.map" "HeadsUp!.srec" "HeadsUp!.usersignatures"
	