#
# Copyright (C) 20012 DreamBox
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

define PatchKernelLzma
	cp $(KDIR)/vmlinux $(KDIR)/vmlinux-$(1)
#	$(STAGING_DIR_HOST)/bin/patch-cmdline $(KDIR)/vmlinux-$(1) '$(strip $(2))'
	$(STAGING_DIR_HOST)/bin/patch-cmdline $(KDIR)/vmlinux-$(1)  $(2)
	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux-$(1) -lc1 -lp2 -pb2 $(KDIR)/vmlinux-$(1).lzma
	mkimage -A mips -O linux -T kernel -a 80000000 -C lzma -e \
		0x80000000 \
		-n 'PandoraBox Linux-$(LINUX_VERSION)' \
		-d $(KDIR)/vmlinux-$(1).lzma $(KDIR)/uImage-$(1).lzma
endef

define Image/BuildKernel
#	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.lzma
#	mkimage -A mips -O linux -T kernel -a 80000000 -C lzma -e \
#		0x80000000 \
#		-n 'MIPS DreamBox Linux-$(LINUX_VERSION)' \
#		-d $(KDIR)/vmlinux-$(1).lzma $(KDIR)/uImage.lzma
#       file nanme,Board comdline
	$(call PatchKernelLzma,mw305r,board=MW305R)
	$(call PatchKernelLzma,dir-620,board=DIR-620-B2)
	$(call PatchKernelLzma,wr8305rt,board=WR8305RT)
endef

define Image/Build/squashfs
	# Align and append a JFFS marker
	$(call prepare_generic_squashfs,$(KDIR)/root.$(1))
endef

define makefirmware
	( \
		dd if=$(KDIR)/uImage-$(2).lzma bs=128k conv=sync; \
		dd if=$(KDIR)/root.$(1) bs=65536 conv=sync; \
	) > $(BIN_DIR)/PandoraBox-$(BOARD)-$(2)-$(1)-$(shell date +%Y-%m-%d).bin
endef

define Image/Build
	$(call Image/Build/$(1),$(1))
	$(call makefirmware,$(1),wr8305rt)
	$(call makefirmware,$(1),mw305r)
	$(call makefirmware,$(1),dir-620)
endef

$(eval $(call BuildImage))
