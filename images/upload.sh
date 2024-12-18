openocd  -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program firmware/kernel/kernel.elf verify reset exit"
