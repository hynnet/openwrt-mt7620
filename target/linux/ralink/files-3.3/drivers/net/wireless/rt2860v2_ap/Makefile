obj-$(CONFIG_RT2860V2_AP) += rt2860v2_ap.o

rt2860v2_ap-objs += rt2860v2_ap_dep.o

$(obj)/rt2860v2_ap_dep.o:
ifeq ($(CONFIG_RALINK_RT3050),y)
	cp $(obj)/rt2860v2_ap_rt3050.o_save $(obj)/rt2860v2_ap_dep.o
endif
ifeq ($(CONFIG_RALINK_RT3052),y)
	cp $(obj)/rt2860v2_ap_rt3052.o_save $(obj)/rt2860v2_ap_dep.o
endif
ifeq ($(CONFIG_RALINK_RT3350),y)
	cp $(obj)/rt2860v2_ap_rt3350.o_save $(obj)/rt2860v2_ap_dep.o
endif
ifeq ($(CONFIG_RALINK_RT3352),y)
	cp $(obj)/rt2860v2_ap_rt3352.o_save $(obj)/rt2860v2_ap_dep.o
endif
ifeq ($(CONFIG_RALINK_RT5350),y)
	cp $(obj)/rt2860v2_ap_rt5350.o_save $(obj)/rt2860v2_ap_dep.o
endif
ifeq ($(CONFIG_RALINK_MT7620),y)
	cp $(obj)/rt2860v2_ap_mt7620.o_save $(obj)/rt2860v2_ap_dep.o
endif