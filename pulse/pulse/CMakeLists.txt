# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(PICO_PLATFORM rp2040 CACHE STRING "Pico platform")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(${PICO_SDK_INIT_CMAKE_FILE})

project(pulse C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


# Add executable. Default name is the project name, version 0.1

add_executable( pulse main.c 
                src/actuators/brakes.c
                src/actuators/inverter_rs485.c
                src/actuators/vesc6.c

                src/cores/core0.c
                src/cores/core1.c  

                src/sensors/dht11.c
                src/sensors/mpu9250.c
                src/sensors/optics.c

                src/communication.c
                src/shared_data.c )

pico_set_program_name(pulse "pulse")
pico_set_program_version(pulse "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pulse 0)
pico_enable_stdio_usb(pulse 0)

# Add the standard library to the build
target_link_libraries(pulse
        pico_stdlib pico_multicore)

# Add the standard include files to the build
target_include_directories(pulse PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(pulse PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(pulse 
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_timer
        hardware_adc
        hardware_sync
        hardware_pwm
        
        )

pico_add_extra_outputs(pulse)

