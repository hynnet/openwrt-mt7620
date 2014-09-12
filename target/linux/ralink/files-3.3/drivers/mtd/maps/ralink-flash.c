/* ralink-flash.c
 *
 * Steven Liu <steven_liu@ralinktech.com.tw>:
 *   - initial approach
 *
 * Winfred Lu <winfred_lu@ralinktech.com.tw>:
 *   - 32MB flash support for RT3052
 *   - flash API
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>

#include <linux/slab.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>
#include <linux/mtd/partitions.h>
#include <asm/addrspace.h>
#include "ralink-flash.h"
#include <asm/rt2880/rt_mmap.h>

#ifndef CONFIG_RT2880_FLASH_32M 
#define WINDOW_ADDR		CPHYSADDR(CONFIG_MTD_PHYSMAP_START)
#define WINDOW_SIZE		CONFIG_MTD_PHYSMAP_LEN
#define NUM_FLASH_BANKS		1
#else
#define WINDOW_ADDR_0		CPHYSADDR(CONFIG_MTD_PHYSMAP_START)
#define WINDOW_ADDR_1		CPHYSADDR(0xBB000000)
#define WINDOW_SIZE		(CONFIG_MTD_PHYSMAP_LEN / 2)
#define NUM_FLASH_BANKS		2
#endif

#define BUSWIDTH		CONFIG_MTD_PHYSMAP_BUSWIDTH


#if defined (CONFIG_MTD_NOR_RALINK) || defined (CONFIG_MTD_ANY_RALINK)

static struct mtd_info *ralink_mtd[NUM_FLASH_BANKS];
#ifndef CONFIG_RT2880_FLASH_32M 
static struct map_info ralink_map[] = {
	{
	.name = "Ralink SoC physically mapped flash",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR
	}
};
#else
static struct mtd_info *merged_mtd;
static struct map_info ralink_map[] = {
	{
	.name = "Ralink SoC physically mapped flash bank 0",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR_0
	},
	{
	.name = "Ralink SoC physically mapped flash bank 1",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR_1
	}
};
#endif

#if 0
#if defined (CONFIG_RT2880_FLASH_32M)
static struct mtd_partition rt2880_partitions[] = {
    	{
                name:           "ALL",
                size:           MTDPART_SIZ_FULL,
                offset:         0,
        },
        {
                name:           "Bootloader",  /* mtdblock0 */
                size:           MTD_BOOT_PART_SIZE,  /* 192K */
                offset:         0,
        }, {
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
                name:           "Kernel",
                size:           CONFIG_MTD_KERNEL_PART_SIZ,
                offset:         MTDPART_OFS_APPEND,
        }, {
                name:           "RootFS",
                size:           MTD_ROOTFS_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
#else //CONFIG_RT2880_ROOTFS_IN_RAM
                name:           "Kernel",
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
#endif
                name:           "Config",
                size:           0x20000,  /* 128K */
                offset:         MTDPART_OFS_APPEND
        }, {
                name:           "Factory",
                size:           0x20000,  /* 128K */
                offset:         MTDPART_OFS_APPEND
#ifdef CONFIG_DUAL_IMAGE
	}, {
		name:		"Kernel2", /* mtdblock4 */
		size:		MTD_KERN2_PART_SIZE, 
		offset:		MTD_KERN2_PART_OFFSET,
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
	}, {
                name:           "RootFS2",
                size:           MTD_ROOTFS2_PART_SIZE,
                offset:         MTD_ROOTFS2_PART_OFFSET,
#endif
        }
#else
	}
#endif 
};
#else //not 32M flash
static struct mtd_partition rt2880_partitions[] = {
    	{
                name:           "ALL",
                size:           MTDPART_SIZ_FULL,
                offset:         0,
        },

        {
                name:           "Bootloader",  /* mtdblock0 */
                size:           MTD_BOOT_PART_SIZE,  /* 192K */
                offset:         0,
        }, {
                name:           "Config", /* mtdblock1 */
                size:           MTD_CONFIG_PART_SIZE,  /* 64K */
                offset:         MTDPART_OFS_APPEND
        }, {
                name:           "Factory", /* mtdblock2 */
                size:           MTD_FACTORY_PART_SIZE,  /* 64K */
                offset:         MTDPART_OFS_APPEND
        }, {
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
                name:           "Kernel", /* mtdblock3 */
                size:           CONFIG_MTD_KERNEL_PART_SIZ,
                offset:         MTDPART_OFS_APPEND,
        }, {
                name:           "RootFS", /* mtdblock4 */
                size:           MTD_ROOTFS_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
        }, {
                name:           "Kernel_RootFS",
                size:           MTD_KERN_PART_SIZE + MTD_ROOTFS_PART_SIZE,
                offset:         MTD_BOOT_PART_SIZE + MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE,
#endif
#else //CONFIG_RT2880_ROOTFS_IN_RAM
                name:           "Kernel", /* mtdblock3 */
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
#endif
#ifdef CONFIG_DUAL_IMAGE
	}, {
		name:		"Kernel2", /* mtdblock4 */
		size:		MTD_KERN2_PART_SIZE, 
		offset:		MTD_KERN2_PART_OFFSET,
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
	}, {
                name:           "RootFS2",
                size:           MTD_ROOTFS2_PART_SIZE,
                offset:         MTD_ROOTFS2_PART_OFFSET,
#endif
        }
#else
	}
#endif 
};
#endif
#endif
static struct mtd_partition rt2880_partitions[] =
{
  	/* The following partitions are board dependent. */
	{
	  name:"u-boot",
	  offset:0,
	  size:0x30000,
	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"u-boot-env",
	  offset:0x30000,
	  size:0x10000,
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"Factory",
	  offset:0x40000,
	  size:0x10000,
//	  mask_flags:MTD_WRITEABLE	  
	}, {
	  name:"kernel",
	  offset:0x50000,
	  size:0xe0000,
//	  mask_flags:MTD_WRITEABLE
	  
	}, {
	  name:"rootfs",
	  offset:0x130000,
	  size:0x800000 - 0x130000,  
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"firmware",
	  offset:0x50000,
	  size:0x800000 - 0x50000,
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"fullflash",
	  offset:0,
	  size:0,
//	  mask_flags:MTD_WRITEABLE
	}


};

#endif // defined (CONFIG_MTD_NOR_RALINK) || defined (CONFIG_MTD_ANY_RALINK) //

#if defined (CONFIG_MTD_NOR_RALINK) || defined (CONFIG_MTD_ANY_RALINK)
static int ralink_lock(struct mtd_info *mtd, loff_t ofs, size_t len)
{
	return 0;
}

static int ralink_unlock(struct mtd_info *mtd, loff_t ofs, size_t len)
{
	return 0;
}
#endif


int ra_check_flash_type(void)
{
#if defined (CONFIG_MTD_NOR_RALINK)
    return BOOT_FROM_NOR;
#elif defined (CONFIG_MTD_SPI_RALINK)
    return BOOT_FROM_SPI;
#elif defined (CONFIG_MTD_NAND_RALINK)
    return BOOT_FROM_NAND;
#else // CONFIG_MTD_ANY_RALINK //

    uint8_t Id[10];
    int syscfg=0;
    int boot_from=0;
    int chip_mode=0;

    memset(Id, 0, sizeof(Id));
    strncpy(Id, (char *)RALINK_SYSCTL_BASE, 6);
    syscfg = (*((int *)(RALINK_SYSCTL_BASE + 0x10)));

    if((strcmp(Id,"RT3052")==0) || (strcmp(Id, "RT3350")==0)) {
	boot_from = (syscfg >> 16) & 0x3; 
	switch(boot_from)
	{
	case 0:
	case 1:
	    boot_from=BOOT_FROM_NOR;
	    break;
	case 2:
	    boot_from=BOOT_FROM_NAND;
	    break;
	case 3:
	    boot_from=BOOT_FROM_SPI;
	    break;
	}
    }else if(strcmp(Id,"RT3883")==0) {
	boot_from = (syscfg >> 4) & 0x3; 
	switch(boot_from)
	{
	case 0:
	case 1:
	    boot_from=BOOT_FROM_NOR;
	    break;
	case 2:
	case 3:
	    chip_mode = syscfg & 0xF;
	    if((chip_mode==0) || (chip_mode==7)) {
		boot_from = BOOT_FROM_SPI;
	    }else if(chip_mode==8) {
		boot_from = BOOT_FROM_NAND;
	    }else {
		printk("unknow chip_mode=%d\n",chip_mode);
	    }
	    break;
	}
    }else if(strcmp(Id,"RT3352")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"RT5350")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"RT2880")==0) {
	boot_from = BOOT_FROM_NOR;
    }else if(strcmp(Id,"RT6855")==0) {
	boot_from = BOOT_FROM_SPI;
    }else if(strcmp(Id,"MT7620")==0) {
	chip_mode = syscfg & 0xF;
	switch(chip_mode)
	{
	case 0:
	case 2:
	case 3:
		boot_from = BOOT_FROM_SPI;
		break;
	case 1:
	case 10:
	case 11:
	case 12:
		boot_from = BOOT_FROM_NAND;
		break;	
	}	
    }else if(strcmp(Id,"MT7621")==0) {
	chip_mode = syscfg & 0xF;
	switch(chip_mode)
	{
	case 0:
	case 2:
	case 3:
		boot_from = BOOT_FROM_SPI;
		break;
	case 1:
	case 10:
	case 11:
	case 12:
		boot_from = BOOT_FROM_NAND;
		break;	
	}	
    } else {
	printk("%s: %s is not supported\n",__FUNCTION__, Id);
    }

    return boot_from;
#endif


}


#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
typedef struct __image_header {
	uint8_t unused[60];
	uint32_t ih_ksz;
} _ihdr_t;
#endif

int __init rt2880_mtd_init(void)
{
#if defined (CONFIG_MTD_NOR_RALINK) || defined (CONFIG_MTD_ANY_RALINK)
	int ret = -ENXIO;
	int i, found = 0;

	if(ra_check_flash_type()!=BOOT_FROM_NOR) { /* NOR */
	    return 0;
	}

#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
	_ihdr_t hdr;
	char *ptr = (char *)CKSEG1ADDR(CONFIG_MTD_PHYSMAP_START + MTD_BOOT_PART_SIZE +
			MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE);
	memcpy(&hdr, ptr, sizeof(_ihdr_t));
	if (hdr.ih_ksz != 0) {
		rt2880_partitions[4].size = ntohl(hdr.ih_ksz);
		rt2880_partitions[5].size = IMAGE1_SIZE - (MTD_BOOT_PART_SIZE +
				MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE +
				ntohl(hdr.ih_ksz));
	}
#endif

	for (i = 0; i < NUM_FLASH_BANKS; i++) {
		printk(KERN_NOTICE "ralink flash device: 0x%x at 0x%x\n",
				ralink_map[i].size, ralink_map[i].phys);

		ralink_map[i].virt = ioremap_nocache(ralink_map[i].phys, ralink_map[i].size);
		if (!ralink_map[i].virt) {
			printk("Failed to ioremap\n");
			return -EIO;
		}
		simple_map_init(&ralink_map[i]);

		ralink_mtd[i] = do_map_probe("cfi_probe", &ralink_map[i]);
		if (ralink_mtd[i]) {
			ralink_mtd[i]->owner = THIS_MODULE;
			ralink_mtd[i]->lock = ralink_lock;
			ralink_mtd[i]->unlock = ralink_unlock;
			++found;
		}
		else
			iounmap(ralink_map[i].virt);
	}
	if (found == NUM_FLASH_BANKS) {
#ifdef CONFIG_RT2880_FLASH_32M
		merged_mtd = mtd_concat_create(ralink_mtd, NUM_FLASH_BANKS,
				"Ralink Merged Flash");
		ret = mtd_device_register(merged_mtd, rt2880_partitions,
				ARRAY_SIZE(rt2880_partitions));
#else
		ret = mtd_device_register(ralink_mtd[0], rt2880_partitions,
				ARRAY_SIZE(rt2880_partitions));
#endif
		if (ret) {
			for (i = 0; i < NUM_FLASH_BANKS; i++)
				iounmap(ralink_map[i].virt);
			return ret;
		}
	}
	else {
		printk("Error: %d flash device was found\n", found);
		return -ENXIO;
	}
#endif
	return 0;
}

static void __exit rt2880_mtd_cleanup(void)
{
#if defined (CONFIG_MTD_NOR_RALINK) || defined (CONFIG_MTD_ANY_RALINK)
	int i;

#ifdef CONFIG_RT2880_FLASH_32M 
	if (merged_mtd) {
		del_mtd_device(merged_mtd);
		mtd_concat_destroy(merged_mtd);
	}
#endif
	for (i = 0; i < NUM_FLASH_BANKS; i++) {
		if (ralink_mtd[i])
			map_destroy(ralink_mtd[i]);
		if (ralink_map[i].virt) {
			iounmap(ralink_map[i].virt);
			ralink_map[i].virt = NULL;
		}
	}
#endif
}

fs_initcall(rt2880_mtd_init);
module_exit(rt2880_mtd_cleanup);

MODULE_AUTHOR("Steven Liu <steven_liu@ralinktech.com.tw>");
MODULE_DESCRIPTION("Ralink APSoC Flash Map");
MODULE_LICENSE("GPL");
