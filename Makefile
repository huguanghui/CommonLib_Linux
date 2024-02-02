BUILD_DATE = $(shell date +"%Y%m%d")
SDK_ROOT := $(PWD)

export PATH:=$(SDK_ROOT)/tools/mke2fs/bin:$(SDK_ROOT)/tools/mksquashfs/bin:$(SDK_ROOT)/tools/pack/bin:$(SDK_ROOT)/tools/mkimage:$(SDK_ROOT)/tools/mkenvimage:$(SDK_ROOT)/tools/mtools:$(SDK_ROOT)/tools/mkbootimg/bin:$(SDK_ROOT)/tools/mkbootimg/bin:$(PATH)
export LD_LIBRARY_PATH:=$(SDK_ROOT)/tools/mke2fs/lib:$(SDK_ROOT)/tools/pack/lib:$(LD_LIBRARY_PATH)
export MKE2FS_CONFIG:=$(SDK_ROOT)/tools/mke2fs/etc/mke2fs.conf

.PHONY: help menuconfig all app clean

help:
	@echo "Usage:"
	@echo "		make menuconfig"
	@echo "     make all"

menuconfig:
	@./tools/mconf ./tools/rootconf

all: system
	# @./pack.sh upg
	# @./pack.sh rom

app:
	@./build.sh

upg_pre:
	rm -rf ./build

date:
	@echo ${BUILD_DATE}
	# @sed -i  's!\("date":"\).*!\1'"${BUILD_DATE}"'",!g' ./system/version

clean:
	@echo "clean"
	@rm -rf build output

