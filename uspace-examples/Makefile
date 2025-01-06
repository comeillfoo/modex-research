SPIN=spin
MODEX=modex

MODEX_FLAGS=-v -Y -q8

NCAT_DIR=ncat
NMAP_DIR=$(NCAT_DIR)/nmap
NCAT_INCLUDE_DIRS=$(addprefix $(NMAP_DIR)/,nsock/include nsock/src ncat nbase libpcap)
NCAT_DEFS=__extension__ __PRETTY_FUNCTION__ __LOCK_ALIGNMENT \
	'__suseconds_t=unsigned' '__blksize_t=unsigned' '__time_t=unsigned' \
	'__bswap_32=unsigned' '__uint16_identity=unsigned' \
	'__uint32_identity=unsigned' '__uint64_identity=unsigned' \
	'__syscall_slong_t=signed' '__blkcnt_t=unsigned' 'int32_t=int' \
	'__fsblkcnt_t=unsigned' '__fsfilcnt_t=unsigned'
NCAT_UNDEFS=

all: help

help:
	@echo 'Targets:'
	@echo
	@echo '- all     same as help'
	@echo '- help    prints this help message'
	@echo '- default same as all'
	@echo '- ncat    extract model from nmap/ncat'


$(NMAP_DIR)/ncat/config.h:
	cd $(NMAP_DIR) && ./configure --without-libpcap \
		--without-zenmap \
		--without-nping \
		--without-openssl \
		--without-libz \
		--without-libssh2 \
		--without-liblua

ncat: $(NCAT_DIR)/ncat.prx $(NMAP_DIR)/ncat/config.h
	$(MODEX) $(MODEX_FLAGS) \
		$(addprefix -I,$(NCAT_INCLUDE_DIRS)) \
		$(addprefix -D,$(NCAT_DEFS)) \
		$(addprefix -U,$(NCAT_UNDEFS)) \
		$<

ncat_clean:
	rm -f $(NMAP_DIR)/nsock/src/engine_select.[IM]

clean: ncat_clean
	rm -f _modex_.* model *.lut

.PHONY: clean ncat_clean
