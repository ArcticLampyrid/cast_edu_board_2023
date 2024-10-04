#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
#Add the user configuration to CMakeProjectConfig.cmake if necessary.
cmake_minimum_required(VERSION 3.21)
project(cast_edu_board_2023 C CXX ASM)

# CubeMX generated files
include_directories(Core/Inc Drivers/STM32F4xx_HAL_Driver/Inc Drivers/STM32F4xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/CMSIS/Include)
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC)
remove_definitions(-DDEBUG)
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS SOURCES "Core/*.*" "Drivers/*.*")
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F401CCUX_FLASH.ld)
add_executable(${PROJECT_NAME}.elf ${CORE_SOURCES})
target_link_options(${PROJECT_NAME}.elf PRIVATE -T ${LINKER_SCRIPT})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(${PROJECT_NAME}.elf PRIVATE -Wl,--print-memory-usage,-Map=$<PATH:REPLACE_EXTENSION,$<TARGET_FILE:${PROJECT_NAME}.elf>,map>)

# Building Bin file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> $<PATH:REPLACE_EXTENSION,$<TARGET_FILE:${PROJECT_NAME}.elf>,bin>
        COMMENT "Building Bin file")

# Building Hex file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> $<PATH:REPLACE_EXTENSION,$<TARGET_FILE:${PROJECT_NAME}.elf>,hex>
        COMMENT "Building Hex file")

# User configuration
include(${CMAKE_SOURCE_DIR}/CMakeProjectConfig.cmake)