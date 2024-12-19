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

add_executable(kernel
	main.c taskmanager.c
	hardware/dvi_640x480.c hardware/dvi_pinout.c hardware/usb_storage.c hardware/usb_driver.c hardware/timing.c
	support/console.c support/directory_test.c support/keyboard.c
)

# ***************************************************************************************
#
#										Build options
#
# ***************************************************************************************

include_directories(
	include
)

target_compile_options(kernel PRIVATE -Wall -Wno-unused-function)

target_compile_definitions(kernel PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	DVI_VERTICAL_REPEAT=1
	DVI_N_TMDS_BUFFERS=3
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

