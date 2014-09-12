#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "i2c_drv.h"

void usage(char *cmd)
{
	printf("Usage:\n");
	printf("  %s addr <address>                - set i2c address\n", cmd);
	printf("  %s dump \n", cmd);
	printf("  %s read <offset>                 - read from offset\n", cmd);
	printf("  %s write <size> <offset> <value> - write value to offset (size 1,2,or 4)\n", cmd);
	printf("Ex:\n");
	printf("  %s addr a0          - set i2c address to 0xa0\n", cmd);
	printf("  %s read 11          - read from offset 0x11\n", cmd);
	printf("  %s write 1 11 33    - write 1 byte value 0x33 to offset 0x11\n", cmd);
	exit(-1);
}

int open_fd(void)
{
	char nm[16];
	int fd;

	snprintf(nm, 16, "/dev/%s", I2C_DEV_NAME);
	if ((fd = open(nm, O_RDONLY)) < 0) {
		perror(nm);
		exit(fd);
	}
	return fd;
}

int main(int argc, char *argv[])
{
	unsigned long addr, off, val;
	int fd, size;
	struct i2c_write_data wdata;

	if (argc < 2)
		usage(argv[0]);
	switch (argv[1][0]) {
		case 'a':
		case 'A':
			addr = strtoul(argv[2], NULL, 16);
			fd = open_fd();
			ioctl(fd, RT2880_I2C_SET_ADDR, addr);
			break;
		case 'd':
		case 'D':
			fd = open_fd();
			ioctl(fd, RT2880_I2C_DUMP, off);
			break;
		case 'r':
		case 'R':
			off = strtoul(argv[2], NULL, 16);
			fd = open_fd();
			ioctl(fd, RT2880_I2C_READ, off);
			break;
		case 'w':
		case 'W':
			if (argc != 5)
				usage(argv[0]);
			size = atoi(argv[2]);
			off = strtoul(argv[3], NULL, 16);
			val = strtoul(argv[4], NULL, 16);
			wdata.address = off;
			wdata.size = size;
			wdata.value = val;
			fd = open_fd();
			ioctl(fd, RT2880_I2C_WRITE, &wdata);
			break;
		default:
			usage(argv[0]);
	}
	close(fd);
	return 0;
}
