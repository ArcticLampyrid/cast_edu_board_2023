include(FetchContent)

# Requirement for C/C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# App sources
include_directories(App/Inc)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "App/*.*")
target_sources(${PROJECT_NAME}.elf PRIVATE ${APP_SOURCES})

# U8G2 v2.35.7
FetchContent_Declare(u8g2 GIT_REPOSITORY https://github.com/olikraus/u8g2 GIT_TAG b7a9144f56d113171fc834a376775e238aad8b28)
FetchContent_MakeAvailable(u8g2)
target_link_libraries(${PROJECT_NAME}.elf u8g2)

# Custom Link Options
target_link_options(${PROJECT_NAME}.elf PRIVATE -Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
target_link_options(${PROJECT_NAME}.elf PRIVATE -u _printf_float)

# Building Bin File
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${BIN_FILE}")

# Building Hex File
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMENT "Building ${HEX_FILE}")
