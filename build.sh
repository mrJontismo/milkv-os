#!/bin/bash

set -xe

cp linker.ld main.c start.S $MILKV_SDK_PATH

cd $MILKV_SDK_PATH
riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv64gc -mabi=lp64f -g -Wall -T linker.ld -o bl33.elf start.S main.c
riscv64-unknown-elf-objcopy -O binary bl33.elf bl33.bin

cd ./fsbl/
sudo ./plat/cv180x/fiptool.py -v genfip 'build/cv1812cp_milkv_duo256m_sd/fip.bin' \
--MONITOR_RUNADDR="0x0000000080000000" \
--CHIP_CONF='build/cv1812cp_milkv_duo256m_sd/chip_conf.bin' \
--NOR_INFO='FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' \
--NAND_INFO='00000000' --BL2='build/cv1812cp_milkv_duo256m_sd/bl2.bin' \
--DDR_PARAM='test/cv181x/ddr_param.bin' \
--MONITOR='../opensbi/build/platform/generic/firmware/fw_dynamic.bin' \
--LOADER_2ND='../bl33.bin' \
--compress='lzma'
cd ..

rm bl33.elf bl33.bin linker.ld main.c start.S

sudo cp --no-preserve=ownership $MILKV_SDK_PATH/fsbl/build/cv1812cp_milkv_duo256m_sd/fip.bin /media/$USER/boot/