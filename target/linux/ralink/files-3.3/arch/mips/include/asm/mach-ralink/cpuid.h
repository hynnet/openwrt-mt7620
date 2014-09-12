#ifndef _CPUID_H
#define _CPUID_H

enum ralink_soc_type {
	SOC_UNKNOWN = 0,
	SOC_RT2880,
	SOC_RT3050,
	SOC_RT3052,
	SOC_RT3350,
	SOC_RT3352,
	SOC_RT5350,
	SOC_RT3662,
	SOC_RT3883,
	SOC_MT7620,
};

enum boot_type {
	UNKNOWN = 0,
	NOR_FLASH,
	SPI_FLASH,
	NAND_FLASH,

};

enum endian {
	BIG_ENDIAN,
	LITTLE_ENDIAN,
};


enum dram_type {
	UNKNOWN_DRAM = 0,
	SD_RAM,
	DDR_RAM,
	DDR2_RAM,
	DDR3_RAM,
};

struct cpu_info
{
  char 			*name;
  unsigned long		cpu_freq;
  unsigned long		sys_freq;
  unsigned long		uart_freq;
  enum  endian		cpu_endian;
  unsigned long		ram_size;
  enum dram_type	ram_type;
  enum ralink_soc_type	type;
  enum boot_type 	boot;
};

//struct cpu_info ralink_cpu;

#define SYS_TYPE_LEN	64

#define RT305X_SYSC_BASE	0xB0000000

/* SYSC registers */
#define SYSC_REG_CHIP_NAME0	0x000	/* Chip Name 0 */
#define SYSC_REG_CHIP_NAME1	0x004	/* Chip Name 1 */
#define SYSC_REG_CHIP_ID	0x00c	/* Chip Identification */
#define SYSC_REG_SYSTEM_CONFIG	0x010	/* System Configuration */

#define RT3052_CHIP_NAME0	0x30335452
#define RT3052_CHIP_NAME1	0x20203235

#define RT3350_CHIP_NAME0	0x33335452
#define RT3350_CHIP_NAME1	0x20203035

#define RT3352_CHIP_NAME0	0x33335452
#define RT3352_CHIP_NAME1	0x20203235

#define RT5350_CHIP_NAME0	0x33355452
#define RT5350_CHIP_NAME1	0x20203035

#define RT3883_CHIP_NAME0	0x33355452
#define RT3883_CHIP_NAME1	0x20203035

#define MT7620_CHIP_NAME0	0x3637544d
#define MT7620_CHIP_NAME1	0x20203032

#define CHIP_ID_ID_MASK		0xff
#define CHIP_ID_ID_SHIFT	8
#define CHIP_ID_REV_MASK	0xff

void get_soc_type(void);

#endif /* CPUID_H */
