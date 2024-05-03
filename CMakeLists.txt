# Cmake Configure
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
cmake_minimum_required(VERSION 3.14)

# Specify Cross-Compilers and Tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


#####################################
# Project Settings
#####################################
project(RM23-Engineer-Renewed
        VERSION 0.0.1
        DESCRIPTION "RoboMaster 23 Engineer Robot Firmware"
        LANGUAGES C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Set Target Directory Name
set(TGTDIR "DM-MC02" CACHE STRING "Target folder name in ./Targets")


#####################################
# ASCII Art
#####################################
message(STATUS "Dharma protector of the universe, destroyer of evil, and guardian of the righteous.
|
|                  ,~^^```\"~-,,<```.`;smm#####m,
|              ,^  ,s###MT7;#M ]Q#` ,a###########M\"`````'``\"~-,
|             /   \"^^\  *W\"3@\"  ||  ~'7#######\"\                 '\".
|          :\"          a ,##\         ]######               e#######M
|         L         ,######~        ,#######              ############m
|        f         ]######b         #######,           ,,##############b
|       ;         .%#####J W####Mm, #####b    ,;,ap ,###################p
|       ~ @#Q|^5mg##8#####  %#\"\"Wgs#######T `@#mw,s#@#@########WW@#######
|      ,^  ^= .MT\ j###C            @#@##\"    `we\"|   @#####b` ,m 3######
|     ;    ,      #####      p   ,###,#\"    .      ,######## ###m  ######
|     ,%\"\"Y\"   ,]#\",##\    `|p  @##,###M\"1^3`   ;##########Q]##`  @######
|    [     7W  ##,#### @m,   ^1 #######m#mw,@m  \@########C^|`.;s#######`
|     y@Mw,   \"#######C  ^\"     |5###### mm ^\"~ ,#########  ;##########`
|    ^   ,aQ   ^@#####       N   ^j###7  `     ,######################
|   D   #####mg,######      M##p   ##b     ,##################^,#####
|   [   ##############       ##########m,###################, ;#####
|    o  ^############       @##########M`^`~\"%############\"  @#####b
|      \"m,j#########b      @#######M|          @#######M\"^^
|           ,^^\"||^7^\"7\.   \"#####\"              \#M7|
|                           \"||`
|
|         Karl Marx     Friedrich Engels     Vladimir Lenin
|")


#####################################
# Compiler Options
#####################################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

# Set Global Compiler Flags
set(CPU -mcpu=cortex-m7)
set(FPU -mfpu=fpv5-d16)
set(FLOAT_ABI -mfloat-abi=hard)
set(MCU ${CPU} -mthumb ${FPU} ${FLOAT_ABI})
add_compile_options(${MCU} -Wall -Wno-packed-bitfield-compat -fdata-sections -ffunction-sections)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

add_executable(FIRMWARE.elf)
include(Targets/${TGTDIR}/target.cmake)

# Set Target Linker Options
if (NOT LINKERSCRIPT)
    message(FATAL_ERROR "Please set linker script file directories in Targets/<CURRENT_TARGET>/target.cmake file!")
endif ()
target_link_options(FIRMWARE.elf PUBLIC ${MCU} -specs=nano.specs)
target_link_options(FIRMWARE.elf PUBLIC -Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
target_link_options(FIRMWARE.elf PUBLIC -T ${PROJECT_SOURCE_DIR}/${LINKERSCRIPT})


#####################################
# Include CMake Project Modules
#####################################
include(Drivers/Drivers.cmake)
include(Middlewares/Middlewares.cmake)
include(Applications/Applications.cmake)


#####################################
# Convert ELF to HEX&BIN
#####################################
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET FIRMWARE.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:FIRMWARE.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:FIRMWARE.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}")