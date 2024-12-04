SCRIPT_DIR := $(shell pwd)
MILKV_SDK_PATH := $(HOME)/dev/tools/duo-buildroot-sdk
FIP_PATH := build/cv1812cp_milkv_duo256m_sd
SOURCE_FILES := src/kernel/start.S src/kernel/main.c src/drivers/uart.c \
                src/kernel/pma.c src/libs/string.c src/kernel/vmm.c \
                src/kernel/trap.c src/kernel/trap.S src/libs/panic.c
INCLUDE_DIR := ./include
OUTPUT_DIR := out
KERNEL_ELF := bl33.elf
KERNEL_BIN := bl33.bin
TOOLCHAIN_PREFIX := riscv64-elf-
GCC_FLAGS := -nostdlib -fno-builtin -march=rv64gc -mstrict-align \
             -mabi=lp64f -g -Wall -Wextra -Wpedantic -Ttext=0x80200000 \
             -ffreestanding -mcmodel=medany -I $(INCLUDE_DIR) -T kernel.ld \
			 -fuse-ld=bfd
FIPTOOL := sudo ./plat/cv180x/fiptool.py
FIP_OPTIONS := --MONITOR_RUNADDR="0x0000000080000000" \
               --CHIP_CONF="$(FIP_PATH)/chip_conf.bin" \
               --NOR_INFO='FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' \
               --NAND_INFO='00000000' \
               --BL2="$(FIP_PATH)/bl2.bin" \
               --DDR_PARAM='test/cv181x/ddr_param.bin' \
               --MONITOR='../opensbi/build/platform/generic/firmware/fw_dynamic.bin' \
               --LOADER_2ND='../$(KERNEL_BIN)' \
               --compress='lzma'

.PHONY: all clean build_kernel fip copy_to_sd

all: $(OUTPUT_DIR)/$(KERNEL_BIN) fip copy_to_sd

$(OUTPUT_DIR)/$(KERNEL_BIN): $(SOURCE_FILES)
	mkdir -p $(OUTPUT_DIR)
	$(TOOLCHAIN_PREFIX)gcc $(GCC_FLAGS) -o $(KERNEL_ELF) $^ -Wl,-Map=$(OUTPUT_DIR)/kernel.map
	$(TOOLCHAIN_PREFIX)objcopy -O binary $(KERNEL_ELF) $(KERNEL_BIN)
	cp $(KERNEL_ELF) $(KERNEL_BIN) $(OUTPUT_DIR)/
	mv $(KERNEL_BIN) $(MILKV_SDK_PATH)
	rm $(KERNEL_ELF)

fip: $(OUTPUT_DIR)/$(KERNEL_BIN)
	cd $(MILKV_SDK_PATH)/fsbl/ && \
	$(FIPTOOL) -v genfip "$(FIP_PATH)/fip.bin" $(FIP_OPTIONS)

copy_to_sd: fip
	udisksctl mount -b /dev/mmcblk0p1
	sudo cp --no-preserve=ownership "$(MILKV_SDK_PATH)/fsbl/$(FIP_PATH)/fip.bin" "/run/media/jon/boot/"
	sync
	cp "$(MILKV_SDK_PATH)/fsbl/$(FIP_PATH)/fip.bin" "./$(OUTPUT_DIR)/"
	udisksctl unmount -b /dev/mmcblk0p1

clean:
	rm -rf $(OUTPUT_DIR) $(MILKV_SDK_PATH)/$(KERNEL_BIN)

