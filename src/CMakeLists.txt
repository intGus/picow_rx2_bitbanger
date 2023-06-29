add_executable(picow_rx2
	main.c
	bt_hid.c
)

pico_enable_stdio_usb(picow_rx2 1)
pico_enable_stdio_uart(picow_rx2 0)
pico_enable_stdio_semihosting(picow_rx2 0)

target_include_directories(picow_rx2 PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(picow_rx2
	hardware_pwm
	hardware_gpio
	pico_stdlib
	pico_cyw43_arch_none
        pico_btstack_classic
        pico_btstack_ble
        pico_btstack_cyw43
	pico_multicore
)

pico_add_extra_outputs(picow_rx2)
