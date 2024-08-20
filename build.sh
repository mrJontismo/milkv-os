#!/bin/bash

set -xe

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
MILKV_SDK_PATH="$HOME/Development/tools/duo-buildroot-sdk"
FIP_PATH="build/cv1812cp_milkv_duo256m_sd"
SOURCE_FILES=("src/kernel/start.S" "src/kernel/main.c" "src/drivers/uart.c" "src/kernel/pma.c" "src/libs/string.c" "src/kernel/vmm.c" "src/kernel/trap.c" "src/kernel/trap.S" "src/libs/panic.c")

riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv64gc -mstrict-align -mabi=lp64f -g -Wall -Wextra -Ttext=0x80200000 -ffreestanding -mcmodel=medany -I ./include -T kernel.ld -o bl33.elf "${SOURCE_FILES[@]}" -Wl,-Map=out/kernel.map 
riscv64-unknown-elf-objcopy -O binary bl33.elf bl33.bin

cp bl33.elf bl33.bin out/
mv bl33.bin $MILKV_SDK_PATH
rm bl33.elf
cd $MILKV_SDK_PATH

cd ./fsbl/
sudo ./plat/cv180x/fiptool.py -v genfip "$FIP_PATH/fip.bin" \
--MONITOR_RUNADDR="0x0000000080000000" \
--CHIP_CONF="$FIP_PATH/chip_conf.bin" \
--NOR_INFO='FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' \
--NAND_INFO='00000000' --BL2="$FIP_PATH/bl2.bin" \
--DDR_PARAM='test/cv181x/ddr_param.bin' \
--MONITOR='../opensbi/build/platform/generic/firmware/fw_dynamic.bin' \
--LOADER_2ND='../bl33.bin' \
--compress='lzma'
cd ..

sudo cp --no-preserve=ownership "$MILKV_SDK_PATH/fsbl/$FIP_PATH/fip.bin" "/media/$USER/boot2/"
cp "$MILKV_SDK_PATH/fsbl/$FIP_PATH/fip.bin" "$SCRIPT_DIR/out/"