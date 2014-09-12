###############################################################################
#  Jan 2007 Bruce Chang
#
#  Initial Release
#
#  lintel<lintel.huang@gmail.com>  MTK Inc.
#
###############################################################################


.S.s:
	$(CPP) $(CFLAGS) $< -o $*.s
.S.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

obj-y    := reset.o \
	    init.uu irq.uu eeprom.uu setup.uu cpuid.uu gpio.uu memory.uu \
            printf.o cmdline.o time.o dev-gpio-buttons.o dev-gpio-leds.o \
            boards/
obj-$(CONFIG_KERNEL_NVRAM) += nvram.o
obj-$(CONFIG_PCI) += pci.o
obj-$(CONFIG_RALINK_TIMER_WDG) +=  ralink_wdt.o
obj-$(CONFIG_RALINK_TIMER_DFS) +=  rt_timer.o
obj-$(CONFIG_RT_DMA) +=  dev-dma.o

obj-$(CONFIG_RALINK_RT3052) +=   lm.o
obj-$(CONFIG_RALINK_RT3350) +=   lm.o
obj-$(CONFIG_RALINK_RT3883) +=  ehci_ohci.o udc.o
obj-$(CONFIG_RALINK_RT3352) +=  ehci_ohci.o udc.o
obj-$(CONFIG_RALINK_RT5350) +=  ehci_ohci.o udc.o
obj-$(CONFIG_RALINK_RT6855) +=  ehci_ohci.o udc.o
obj-$(CONFIG_RALINK_MT7620) +=  ehci_ohci.o udc.o

obj-$(CONFIG_RALINK_WIFI_RT3052) +=  rt3052-wifi.uu
obj-$(CONFIG_RALINK_WIFI_RT3352) +=  rt3352-wifi.uu
obj-$(CONFIG_RALINK_WIFI_RT5350) +=  rt5350-wifi.uu
obj-$(CONFIG_RALINK_WIFI_MT7620) +=  mt7620-wifi.uu