SPIN=spin
MODEX=modex

MODEX_FLAGS=-v -Y -q8

NETCAT_PRJ_DIR=netcat
NETCAT_DIR=$(NETCAT_PRJ_DIR)/nc
NETCAT_INCDIRS=$(NETCAT_DIR)
NETCAT_DEFS='size_t=int' 'ssize_t=int' 'socklen_t=int' \
			'time_t=int' 'FILE=void' 'uint32_t=int'

NCAT_PRJ_DIR=ncat
NMAP_DIR=$(NCAT_PRJ_DIR)/nmap
NMAP_INCDIRS=$(addprefix $(NMAP_DIR)/,nsock/include nsock/src ncat nbase libpcap)
NMAP_DEFS=__extension__ __PRETTY_FUNCTION__ __LOCK_ALIGNMENT \
	'__suseconds_t=unsigned' '__blksize_t=unsigned' '__time_t=unsigned' \
	'__bswap_32=unsigned' '__uint16_identity=unsigned' \
	'__uint32_identity=unsigned' '__uint64_identity=unsigned' \
	'__syscall_slong_t=signed' '__blkcnt_t=unsigned' 'int32_t=int' \
	'__fsblkcnt_t=unsigned' '__fsfilcnt_t=unsigned'
NMAP_UNDEFS=

all: help

help:
	@echo 'Targets:'
	@echo
	@echo '- all     same as help'
	@echo '- help    prints this help message'
	@echo '- default same as all'
	@echo '- netcat  extract model from netcat/nc'
	@echo '- ncat    extract model from nmap/ncat'

clean: ncat_clean netcat_clean
	rm -f _modex_.* model *.lut *.pml _modex_*.pml _modex_*.h pan.? pan pan.exe \
		_spin_nvr.tmp

.PHONY: clean ncat_clean netcat_clean

netcat:	$(NETCAT_PRJ_DIR)/netcat.prx
	$(MODEX) $(MODEX_FLAGS) \
		$(addprefix -I,$(NETCAT_INCDIRS)) \
		$(addprefix -D,$(NETCAT_DEFS)) \
		$<

netcat_clean:
	rm -f $(NETCAT_DIR)/netcat.[IM]

$(NMAP_DIR)/ncat/config.h:
	cd $(NMAP_DIR) && ./configure --without-libpcap \
		--without-zenmap \
		--without-nping \
		--without-openssl \
		--without-libz \
		--without-libssh2 \
		--without-liblua

ncat: $(NCAT_PRJ_DIR)/ncat.prx $(NMAP_DIR)/ncat/config.h
	$(MODEX) $(MODEX_FLAGS) \
		$(addprefix -I,$(NMAP_INCDIRS)) \
		$(addprefix -D,$(NMAP_DEFS)) \
		$(addprefix -U,$(NMAP_UNDEFS)) \
		$<

ncat_clean:
	rm -f $(NMAP_DIR)/nsock/src/engine_select.[IM]
