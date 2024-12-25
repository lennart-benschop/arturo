# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		CMakeLists.txt
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		18th December 2024
#		Reviewed :	No
#		Purpose :	Outer level CMake configuration
#
# ***************************************************************************************
# ***************************************************************************************

# ***************************************************************************************
#
#							Set up the program components
#
# ***************************************************************************************

file(GLOB_RECURSE USER_C_SOURCES FOLLOW_SYMLINKS "user_code/*.c")
file(GLOB_RECURSE HARDWARE_C_SOURCES "hardware/*.c")
file(GLOB_RECURSE SUPPORT_C_SOURCES "support/*.c")

add_executable(kernel
	main.c 
	${USER_C_SOURCES} ${HARDWARE_C_SOURCES} ${SUPPORT_C_SOURCES}
)

# ***************************************************************************************
#
#										Build options
#
# ***************************************************************************************

include_directories(
	include
	user_code/include
)

target_compile_options(kernel PRIVATE -Wall -Wno-unused-function -Wno-unused-variable)

target_compile_definitions(kernel PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	DVI_VERTICAL_REPEAT=1
	DVI_N_TMDS_BUFFERS=3
	DVI_1BPP_BIT_REVERSE=1
	)

target_link_libraries(kernel
	pico_stdlib
	pico_multicore
	libdvi
   	tinyusb_host
  	tinyusb_device
 	tinyusb_board
 	fatfs
)

pico_add_extra_outputs(kernel)

# ***************************************************************************************
#
#						Set up for the Extended Flash ROM / Stage 2 boot.
#
# ***************************************************************************************

target_compile_definitions(kernel
    PRIVATE PICO
    PRIVATE PICO_FLASH_SPI_CLKDIV=4
)
	
pico_define_boot_stage2(firmware_boot2 ${PICO_SDK_PATH}/src/rp2040/boot_stage2/boot2_generic_03h.S)

target_compile_definitions(firmware_boot2 PRIVATE
	PICO_FLASH_SPI_CLKDIV=4
	PICO_FLASH_SIZE_BYTES=2097152
)

pico_set_boot_stage2(kernel firmware_boot2)

