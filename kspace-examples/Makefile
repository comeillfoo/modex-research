SPIN=spin
MODEX=modex
MAKE?=make

SUBARCH:=$(shell uname -m | sed -e s/i.86/x86/ -e s/x86_64/x86/ \
				  -e s/sun4u/sparc64/ \
				  -e s/arm.*/arm/ -e s/sa110/arm/ \
				  -e s/s390x/s390/ -e s/parisc64/parisc/ \
				  -e s/ppc.*/powerpc/ -e s/mips.*/mips/ \
				  -e s/sh[234].*/sh/ -e s/aarch64.*/arm64/ \
				  -e s/riscv.*/riscv/)
ARCH?=$(SUBARCH)
SRCARCH=$(ARCH)
# Additional ARCH settings for x86
ifeq ($(ARCH),i386)
    SRCARCH:=x86
endif
ifeq ($(ARCH),x86_64)
    SRCARCH:=x86
endif

# Additional ARCH settings for sparc
ifeq ($(ARCH),sparc32)
    SRCARCH:=sparc
endif
ifeq ($(ARCH),sparc64)
    SRCARCH:=sparc
endif

# Additional ARCH settings for sh
ifeq ($(ARCH),sh64)
    SRCARCH:=sh
endif

LX_KERNEL=linux
WG_PRJ_DIR=wireguard
WG_SRC_DIR=$(LX_KERNEL)/drivers/net/wireguard
WG_INC_DIRS=$(LX_KERNEL)/arch/$(SRCARCH)/include \
	$(LX_KERNEL)/arch/$(SRCARCH)/include/generated \
	$(LX_KERNEL)/include/generated $(LX_KERNEL)/include \
	$(LX_KERNEL)/arch/$(SRCARCH)/include/uapi \
	$(LX_KERNEL)/arch/$(SRCARCH)/include/generated/uapi \
	$(LX_KERNEL)/include/uapi $(LX_KERNEL)/include/generated/uapi $(WG_SRC_DIR)
WG_DEFS=__KERNEL__
CC_OPTS=-include $(LX_KERNEL)/include/linux/kconfig.h \
	-include $(LX_KERNEL)/include/linux/compiler_types.h

MODEX_FLAGS=-v -Y -q8

all: help

help:
	@echo 'Targets:'
	@echo
	@echo '- all      same as help'
	@echo '- clean    cleans all artifacts'
	@echo '- default  same as all'
	@echo '- help     prints this help message'
	@echo '- wg       extracts model from `linux/drivers/net/wireguard`'
	@echo '- wg_clean cleans artifacts of wireguard model extraction'

clean: wg_clean
	rm -f _modex_.* model *.lut *.pml _modex_*.pml _modex_*.h pan.? pan pan.exe \
		_spin_nvr.tmp pan.pre

.PHONY: clean wg_clean kernelclean

wg: $(WG_PRJ_DIR)/wg.prx $(LX_KERNEL)/vmlinux
	$(MODEX) $(MODEX_FLAGS) \
		$(addprefix -I,$(WG_INC_DIRS)) \
		$(addprefix -D,$(WG_DEFS)) \
		$<

wg_clean:
	rm -f $(WG_SRC_DIR)/*.[IM]

kernelclean:
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) distclean
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) clean
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) mrproper
	$(MAKE) -C $(LX_KERNEL)/tools ARCH=$(ARCH) clean
	$(MAKE) -C $(LX_KERNEL)/tools/testing/selftests ARCH=$(ARCH) clean

$(LX_KERNEL)/vmlinux:
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) tinyconfig
	(cd $(LX_KERNEL) && ./scripts/config -e NET -e INET -e NETDEVICES \
		-e NET_CORE -m WIREGUARD)
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) olddefconfig
	$(MAKE) -C $(LX_KERNEL) ARCH=$(ARCH) -j $$(nproc)
