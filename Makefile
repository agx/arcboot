#
# Copyright 1999 Silicon Graphics, Inc.
# Copyright 2002-2004 Guido Guenther <agx@sigxcpu.org>
# Copyright (C) 2004 by Thiemo Seufer
#  

# default subarch
SUBARCH ?= IP22

ifeq ($(SUBARCH),IP22)
KERNELADDR=0x88002000
MAXLOADSIZE=0x1700000
LOADADDR=0x88802000
TIP_LOADADDR=0x89702000
OUTPUTFORMAT=ecoff-bigmips
endif
ifeq ($(SUBARCH),IP32)
KERNELADDR=0x80004000
MAXLOADSIZE=0x1400000
LOADADDR=0x81404000
TIP_LOADADDR=$(LOADADDR)
OUTPUTFORMAT=elf32-tradbigmips
endif

# these contain subarch independent files
SUBARCH_INDEP_DIRS=	\
	arclib

# these contain subarch dependent files
SUBARCH_DIRS=		\
	common		\
	ext2load        \
	tip22

define indep-tgt
$(foreach sd,$(SUBARCH_INDEP_DIRS),$(1)-subarch-indep-$(sd))
endef

define dep-tgt
$(foreach sd,$(SUBARCH_DIRS),$(1)-subarch-dep-$(sd))
endef

define build-indep-tgt
$(call indep-tgt,build)
endef

define build-dep-tgt
$(call dep-tgt,build)
endef

define install-indep-tgt
$(call indep-tgt,install)
endef

define install-dep-tgt
$(call dep-tgt,install)
endef

define clean-indep-tgt
$(call indep-tgt,clean)
endef

define clean-dep-tgt
$(call dep-tgt,clean)
endef

define submake
@$(MAKE) -C $(1) SUBARCH=$(SUBARCH)           \
                 LOADADDR=$(LOADADDR)         \
                 TIP_LOADADDR=$(TIP_LOADADDR) \
                 MAXLOADSIZE=$(MAXLOADSIZE)   \
                 KERNELADDR=$(KERNELADDR)     \
                 OUTPUTFORMAT=$(OUTPUTFORMAT) \
                 $(2)
endef


all: build

build: build-subarch-indep build-subarch-dep
build-subarch-indep: $(build-indep-tgt)
build-subarch-dep: $(build-dep-tgt)

$(build-indep-tgt):
	$(call submake,$(patsubst build-subarch-indep-%,%,$@),all)

$(build-dep-tgt):
	$(call submake,$(patsubst build-subarch-dep-%,%,$@),all)

install: install-subarch-indep install-subarch-dep
install-subarch-indep: $(install-indep-tgt)
install-subarch-dep: $(install-dep-tgt)

$(install-indep-tgt):
	$(call submake,$(patsubst install-subarch-indep-%,%,$@),install)

$(install-dep-tgt):
	$(call submake,$(patsubst install-subarch-dep-%,%,$@),install)

clean: clean-subarch-indep clean-subarch-dep
clean-subarch-indep: $(clean-indep-tgt)
clean-subarch-dep: $(clean-dep-tgt)

$(clean-indep-tgt):
	$(call submake,$(patsubst clean-subarch-indep-%,%,$@),clean)

$(clean-dep-tgt):
	$(call submake,$(patsubst clean-subarch-dep-%,%,$@),clean)


.PHONY: all \
	build build-subarch-indep build-subarch-dep \
	$(build-indep-tgt) $(build-dep-tgt) \
	clean clean-subarch-indep clean-subarch-dep \
	$(clean-indep-tgt) $(clean-dep-tgt)
