# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(kernel
	main.c
)

target_compile_options(kernel PRIVATE -Wall)

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

# create map/bin/hex file etc.
pico_add_extra_outputs(kernel)

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

