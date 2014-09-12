/*
 * RT2880/RT3052 read/write register utility.
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <errno.h>

//#define DD printf("%s %d\n", __FUNCTION__, __LINE__); fflush(stdout);

#define PCIEPHY0_CFG	0x10140090

#define PAGE_SIZE		0x1000 	/* 4096 */

#define READMODE	0x0
#define WRITEMODE	0x1
#define WRITE_DELAY	10			/* ms */

unsigned int rareg(int mode, unsigned int addr, long long int new_value)
{
	int fd; 
	unsigned int round;
	void *start;
	volatile unsigned int *v_addr;
	unsigned int rc;

	fd = open("/dev/mem", O_RDWR | O_SYNC );
	if ( fd < 0 ) { 
		printf("open file /dev/mem error. %s\n", strerror(errno)); 
		exit(-1);
	} 

	// round addr to PAGE_SIZE
	round = addr;								// keep old value
	addr = (addr / PAGE_SIZE) * PAGE_SIZE;
	round = round - addr;

	start = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);
	if(	(int)start == -1 ){
		printf("mmap() failed at phsical address:%d %s\n", addr, strerror(errno)); 
		close(fd);
		exit(-1);
	}
	//printf("mmap() starts at 0x%08x successfuly\n", (unsigned int) start);

	v_addr = (void *)start + round;
	addr = addr + round;
	//printf("0x%08x: 0x%08x\n", addr, *v_addr);

	if(mode == WRITEMODE){
		*v_addr = new_value;
		usleep(WRITE_DELAY * 1000);
		//printf("0x%08x: 0x%08x\n", addr, *v_addr);
	}

	rc = *v_addr;
	munmap(start, PAGE_SIZE);
	close(fd);

	return rc;
}

void usage(char *str)
{
	printf("Usage:\n");
	printf("%s r addr(hex)			-- read value from addr\n", str);
	printf("%s w addr(hex) value(hex)		-- write value to addr\n", str);
	printf("%s d addr1(hex) addr2(hex)		-- dump value from addr1 to addr2\n", str);
}

#define WRITE_MODE	(1UL<<23)
#define DATA_SHIFT	0
#define ADDR_SHIFT	8
#define BUSY		0x80000000
#define RETRY_MAX	3	/* 3 secs */

void wait_pciephy_busy(void)
{
	unsigned long reg_value = 0x0;
	int retry = 0;

	// wait until not busy
	while(1){
		reg_value = rareg(READMODE, PCIEPHY0_CFG, 0);
		if(reg_value & BUSY){
			printf("PCIE-PHY is busy, sleep 1 sec.\n");
			sleep(1);
			retry++;
		}else
			break;
		if(retry > RETRY_MAX){
			printf("PCIE-PHY retry failed.\n");
			exit(-1);
		}
	}

}

int main(int argc, char *argv[])
{
	char rwmode = 'r';
	unsigned long addr, value = 0;
	unsigned long reg_value = 0x0;
	long	addr_end = -1;

	if(argc < 3 || argc > 4){
		usage(argv[0]);
		exit(0);
	}

	rwmode = argv[1][0];

	if(rwmode == 'r'){
		if(argc != 3){
			usage(argv[0]);
			exit(0);
		}
	}else{
		if(argc != 4){
			usage(argv[0]);
			exit(0);
		}
	}

	addr = strtol(argv[2], NULL, 16);
	if(rwmode == 'w')
		value = strtol(argv[3], NULL, 16);
	if(rwmode == 'd'){
		addr_end = strtol(argv[3], NULL, 16);
		if(addr > addr_end){
			printf("Error: wrong range.\n");
			exit(0);
		}
	}

	if(addr > 255 || addr_end > 255 || value > 255){
		printf("Error: too large: Addr=0x%x, value=0x%x\n", addr, value);
		exit(0);		
	}


	do{
		wait_pciephy_busy();

		reg_value = 0x0;
		if(rwmode == 'w'){
			reg_value |= WRITE_MODE;
			reg_value |= (value) << DATA_SHIFT;
		}
		reg_value |= (addr) << ADDR_SHIFT;

		// apply the action
		rareg(WRITEMODE, PCIEPHY0_CFG, reg_value);

		usleep(1000);	/* 1ms */
		wait_pciephy_busy();

		if(rwmode == 'r' || rwmode == 'd'){
			reg_value = rareg(READMODE, PCIEPHY0_CFG, 0);
			printf("[%02x]=0x%02x\n", addr, reg_value & 0xff);
		}
		addr++;
	}while(addr_end >= 0 && addr <= addr_end);
}
