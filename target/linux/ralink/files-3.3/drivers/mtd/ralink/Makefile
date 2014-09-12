obj-$(CONFIG_MTD_NAND_RALINK)	+= rt_nand.o
obj-$(CONFIG_MTD_SPI_RALINK)	+= rt_spi.o 

#support NOR/NAND/SPI flash
obj-$(CONFIG_MTD_ANY_RALINK)    += rt_any.o
ifeq ($(CONFIG_RALINK_RT3052),y)
rt_any-objs = ralink_nand_rt3052.o gdma.o ralink_spi.o
else
rt_any-objs = ralink_nand.o gdma.o ralink_spi.o
endif

ifeq ($(CONFIG_RALINK_RT3052),y)
rt_nand-objs = ralink_nand_rt3052.o gdma.o
else
rt_nand-objs = ralink_nand.o gdma.o
endif
ifeq ($(CONFIG_RALINK_RT3883),y)
	rt_nand-objs = ralink_nand_rt3052.o gdma.o
else
	rt_nand-objs = ralink_nand.o gdma.o
endif

rt_spi-objs = ralink_spi.o

