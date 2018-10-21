#!/bin/bash

device="STM32F103xB"

device_lower=$(echo "$device" | tr '[:upper:]' '[:lower:]')
device_upper=$(echo "$device" | tr '[:lower:]' '[:upper:]')
path_cmsis="$(pwd)/CMSIS/Device/ST/STM32F1xx"

#compiler_flags=("")
compiler_flags=(-std=c99 -Wall -fno-common -nostdlib -Os)
compiler_flags+=(-mthumb -mcpu=cortex-m3 -D "$device")
compiler_flags+=(-fdata-sections -ffunction-sections )
compiler_flags+=(-I "$path_cmsis/Include")
compiler_flags+=(-I "$path_cmsis/STM32F1xx_HAL_Driver/Inc")
compiler_flags+=(-I "$path_cmsis/STM32F1xx_HAL_Driver/Src")
compiler_flags+=(-I "$path_cmsis/../../../Include/")

#linker_flags=("")
linker_flags=(-mthumb -mcpu=cortex-m3)
linker_flags=(-Wl,--gc-sections)
linker_flags+=(-T "$path_cmsis/linker/${device_upper}_FLASH.ld")

if [[ -z "${CROSS_COMPILE// }" ]]; then
	echo "Var CROSS_COMPILE is not defined!" >&2
	exit 1
fi

if [[ -n "${DEBUG// }" ]]; then
	compiler_flags+=(-D "DEBUG")
fi

if [[ -n "${DEBUG// }" ]]; then
	echo "Compiler flags: ${compiler_flags[@]}"
	echo "  Linker flags: ${linker_flags[@]}"
fi
#

if [[ -f "out.elf" ]] || [[ -f "out.bin" ]] || [[ -f "out_user.bin" ]]; then
	rm -Rf out.elf out.bin out_user.bin
fi

"$CROSS_COMPILE"gcc "${compiler_flags[@]}" "${linker_flags[@]}" "$path_cmsis/Source/startup_${device_lower}.s" "$path_cmsis/Source/system_stm32f1xx.c" main.c -o out.elf
if [[ "$?" -ne "0" ]]; then
	echo "Error compiling!" >&2
	exit 1
fi

"$CROSS_COMPILE"objcopy -O binary out.elf out.bin

echo "Completed!"

