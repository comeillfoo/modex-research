CC=gcc
SPIN=spin
MODEX=modex

WG_PRJ_DIR=wireguard
WG_SRC_DIR=$(WG_PRJ_DIR)/wireguard
WG_INC_DIRS=$(WG_SRC_DIR) $(WG_PRJ_DIR)/stubs
WG_DEFS=

CC_OPTS=-nostdinc -isystem $(shell $(CC) -print-file-name=include)
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
	@echo '- wg-clean cleans artifacts of wireguard model extraction'

clean: wg-clean
	rm -f _modex_.* model *.lut *.pml _modex_*.pml _modex_*.h pan.? pan pan.exe \
		_spin_nvr.tmp pan.pre

.PHONY: clean wg-clean

wg: $(WG_PRJ_DIR)/wg.prx
	$(MODEX) $(MODEX_FLAGS) \
		-Q "$(CC_OPTS)" \
		$(addprefix -I,$(WG_INC_DIRS)) \
		$(addprefix -D,$(WG_DEFS)) \
		$<

wg-clean:
	rm -f $(WG_SRC_DIR)/*.[IM]
