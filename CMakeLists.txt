cmake_minimum_required(VERSION 3.23)

if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "DEVKITPRO not found.\n")
endif()

set(DEVKITPRO $ENV{DEVKITPRO})
if(DEFINED ENV{DEVKITA64})
    set(DEVKITA64 $ENV{DEVKITA64})
else()
    set(DEVKITA64 ${DEVKITPRO}/devkitA64)
endif()

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(_TOOL_PREFIX ${DEVKITA64}/bin/aarch64-none-elf-)
set(CMAKE_C_COMPILER        ${_TOOL_PREFIX}gcc       CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER      ${_TOOL_PREFIX}g++       CACHE PATH "" FORCE)
set(CMAKE_ASM_COMPILER      ${_TOOL_PREFIX}gcc       CACHE PATH "" FORCE)
set(CMAKE_AR                ${_TOOL_PREFIX}gcc-ar    CACHE PATH "" FORCE)
set(CMAKE_RANLIB            ${_TOOL_PREFIX}gcc-ranlib CACHE PATH "" FORCE)

set(CMAKE_FIND_ROOT_PATH        ${DEVKITPRO})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE  ONLY)

project(ff4_nx LANGUAGES C CXX)

set(CMAKE_C_STANDARD          11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD        17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TARGET ff4)

set(APP_TITLE   "Final Fantasy IV (3D Remake)" CACHE STRING "NACP title")
set(APP_AUTHOR  "GlitchedDeveloper"            CACHE STRING "NACP author")
set(APP_VERSION "1.0.4"                        CACHE STRING "NACP version")
set(APP_ICON    ${CMAKE_SOURCE_DIR}/icon.jpg   CACHE FILEPATH "NRO icon (.jpg)")

set(ARCH_FLAGS    -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE)
set(SWITCH_SPECS  ${DEVKITPRO}/libnx/switch.specs)

set(LIBNX_INCLUDE    ${DEVKITPRO}/libnx/include       CACHE PATH "libnx headers")
set(LIBNX_LIB        ${DEVKITPRO}/libnx/lib           CACHE PATH "libnx libs")
set(PORTLIBS_INCLUDE ${DEVKITPRO}/portlibs/switch/include CACHE PATH "Switch portlibs headers")
set(PORTLIBS_LIB     ${DEVKITPRO}/portlibs/switch/lib     CACHE PATH "Switch portlibs libs")

add_compile_options(
    ${ARCH_FLAGS}
    -g -Wall -O2 -ffunction-sections
    -D__SWITCH__
)
add_link_options(
    ${ARCH_FLAGS}
    -specs=${SWITCH_SPECS}
    -Wl,-Map,${TARGET}.map
)

include_directories(
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/disarm
    ${LIBNX_INCLUDE}
    ${PORTLIBS_INCLUDE}
)
link_directories(${LIBNX_LIB} ${PORTLIBS_LIB})

set(DISARM_BUILD_TESTS OFF CACHE BOOL "Build disarm64 unit tests" FORCE)
add_subdirectory(source/disarm EXCLUDE_FROM_ALL)

file(GLOB_RECURSE FF4_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/source/*.cpp
    ${CMAKE_SOURCE_DIR}/source/*.c
    ${CMAKE_SOURCE_DIR}/source/*.s
)
list(FILTER FF4_SOURCES EXCLUDE REGEX "/source/disarm/.*")

add_executable(${TARGET}.elf ${FF4_SOURCES})

target_link_libraries(${TARGET}.elf PRIVATE
    disarm64::disarm64
    -lavformat -lavcodec -lswscale -lswresample -lavutil
    -ldav1d -lbz2
    -lSDL2 -lEGL -lGLESv1_CM -lglapi -ldrm_nouveau
    -lm -lnx -lz
)

find_program(NACP_TOOL nacptool PATHS ${DEVKITPRO}/tools/bin NO_DEFAULT_PATH)
find_program(NRO_TOOL  elf2nro  PATHS ${DEVKITPRO}/tools/bin NO_DEFAULT_PATH)

set(NACP_FILE ${CMAKE_BINARY_DIR}/${TARGET}.nacp)
set(NRO_FILE  ${CMAKE_BINARY_DIR}/${TARGET}.nro)

if(NACP_TOOL AND NRO_TOOL)
    add_custom_command(
        OUTPUT  ${NACP_FILE}
        COMMAND ${NACP_TOOL} --create ${APP_TITLE} ${APP_AUTHOR} ${APP_VERSION} ${NACP_FILE}
        DEPENDS ${TARGET}.elf
        COMMENT "[ff4_nx] generating ${TARGET}.nacp"
        VERBATIM
    )
    add_custom_command(
        OUTPUT  ${NRO_FILE}
        COMMAND ${NRO_TOOL} $<TARGET_FILE:${TARGET}.elf> ${NRO_FILE}
                --icon=${APP_ICON}
                --nacp=${NACP_FILE}
        DEPENDS ${TARGET}.elf ${NACP_FILE} ${APP_ICON}
        COMMENT "[ff4_nx] generating ${TARGET}.nro"
        VERBATIM
    )
    # ALL so `cmake --build build` produces the .nro directly.
    add_custom_target(ff4_nx_nro ALL DEPENDS ${NRO_FILE})
else()
    message(WARNING "nacptool/elf2nro not found.\n")
endif()
