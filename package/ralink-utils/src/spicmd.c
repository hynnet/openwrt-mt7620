#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "spi_drv.h"

char usage[] = 	"spicmd read/write eeprom_address data(if write)\n"\
	"spicmd format:\n"\
	"  spicmd read [address in hex]\n"\
	"  spicmd write [size] [address] [value]\n"\
	"  spicmd vtss read [block] [sub-block] [register]\n"\
	"  spicmd vtss write [block] [sub-block] [register] [value]\n"\
	"  spicmd vtss p0\n"\
	"  spicmd vtss p4\n"\
	"  spicmd vtss novlan\n"\
	"NOTE -- size is 1, 2, 4 bytes only, address and value are in hex\n";


int main(int argc, char *argv[])
{
	int chk_match, size, fd;
	unsigned long addr, value;
	int address;
	SPI_WRITE spi_write;

	if (argc < 3) {
		printf("Usage:\n%s\n", usage);
		return 1;
	}
	if (!strncmp(argv[1], "vtss", 4)) {
		SPI_VTSS vtss;

		fd = open("/dev/spiS0", O_RDONLY); 
		if (fd <= 0) {
			printf("Please insmod module spi_drv.o!\n");
			return -1;
		}

		if (!strncmp(argv[2], "p0", 3)) {
			ioctl(fd, RT2880_SPI_INIT_VTSS_WANATP0);
			close(fd);
			return 0;
		}
		else if (!strncmp(argv[2], "p4", 3)) {
			ioctl(fd, RT2880_SPI_INIT_VTSS_WANATP4);
			close(fd);
			return 0;
		}
		else if (!strncmp(argv[2], "novlan", 7)) {
			ioctl(fd, RT2880_SPI_INIT_VTSS_NOVLAN);
			close(fd);
			return 0;
		}

		if (argc < 6) {
			printf("Usage:\n%s\n", usage);
			return 1;
		}
		vtss.blk = strtol(argv[3], NULL, 16);
		vtss.sub = strtol(argv[4], NULL, 16);
		vtss.reg = strtol(argv[5], NULL, 16);
		if (!strncmp(argv[2], "read", 5)) {
			ioctl(fd, RT2880_SPI_VTSS_READ, &vtss);
			printf("r %x %x %x -> %x\n", vtss.blk, vtss.sub, vtss.reg, vtss.value);
		}
		else if (!strncmp(argv[2], "write", 6)) {
			vtss.value = strtol(argv[6], NULL, 16);
			ioctl(fd, RT2880_SPI_VTSS_WRITE, &vtss);
			printf("w %x %x %x -> %x\n", vtss.blk, vtss.sub, vtss.reg, vtss.value);
		}
		close(fd);
		return 0;
	}

/* We use the last specified parameters, unless new ones are entered */
	switch (argc) {
		case RT2880_SPI_READ:
			value = 0;
        		chk_match = strcmp(argv[1], RT2880_SPI_READ_STR);
        		if ( chk_match != 0) {
				printf("Usage:\n%s\n", usage);
                		return 1;
        		}
        		addr = strtoul(argv[2], NULL, 16);
			address = addr;

			fd = open("/dev/spiS0", O_RDONLY); 
			if (fd <= 0) {
				printf("Please insmod module spi_drv.o!\n");
				return -1;
			}
			ioctl(fd, RT2880_SPI_READ, address);
			close(fd);
        		break;
		case RT2880_SPI_WRITE:
			chk_match = strcmp(argv[1], RT2880_SPI_WRITE_STR);
			if ( chk_match != 0) {
				printf("Usage:\n%s\n", usage);
				return 1;
			}
			size = strtoul(argv[2], NULL, 16);
			addr = strtoul(argv[3], NULL, 16);
			value = strtoul(argv[4], NULL, 16);
			// fd = open(SPI_DEVNAME, O_RDONLY); 
			fd = open("/dev/spiS0", O_RDONLY); 
			spi_write.address = addr;
			spi_write.size = size;
			spi_write.value = value;
			if (fd <= 0) {
				printf("Please insmod module spi_drv.o!\n");
				return -1;
			}
			ioctl(fd, RT2880_SPI_WRITE, &spi_write);
			close(fd);
			printf("0x%04x : 0x%08x in %d bytes\n", addr, value, size);
			break;
		default:
			printf("Usage:\n%s\n\n", usage);
	}
	return 0;
}
