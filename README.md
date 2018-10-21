# stm32-oscilloscope

Simple oscilloscope for stm32

Supported speeds:

-1Khz

-5Khz

Note that those speeds is affected by the serial spped

Maximum supported speed = Serial baud / (16 (1 adc sample) + 1 start bit + 1 parity bit + 2 stop bits + 1 reserved bit)

Compilation:

Export CROSS_COMPILE to your toolchain path + name (Like in linux kernel CROSS_COMPILE)

run make.sh

