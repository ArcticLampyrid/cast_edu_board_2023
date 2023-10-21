#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-gnu-cortex-m4.cmake" CACHE STRING "")
project(cast_edu_board_2023 C CXX ASM)

# CubeMX generated files
include_directories(Core/Inc Drivers/STM32F4xx_HAL_Driver/Inc Drivers/STM32F4xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/CMSIS/Include)
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC)
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS SOURCES "Core/*.*" "Drivers/*.*")
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F401CCUX_FLASH.ld)
add_executable(${PROJECT_NAME}.elf ${CORE_SOURCES})
target_link_options(${PROJECT_NAME}.elf PRIVATE -T ${LINKER_SCRIPT})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

# User configuration
include(${CMAKE_SOURCE_DIR}/CMakeProjectConfig.cmake)