#include <stdlib.h>             /* malloc, free, etc. */
#include <stdio.h>              /* stdin, stdout, stderr */
#include <string.h>             /* strdup */
#include <sys/ioctl.h>
#include <fcntl.h>


#define LINE_DRIVER_READ	0x1	// Read Single registers
#define LINE_DRIVER_WRITE	0x2	// Write
#define LINE_DRIVER_DUMP	0x3

//  syntax: ld [r/w] [offset(hex)] [value(hex, w only)] 
//  example ld r 18
//  example ld w 18 a8
int main(int argc, char *argv[])
{
	int fd, method, value = 0;
	unsigned char offset = 0, mask = 0;
	char *p;

	if (argc < 2)
	{
		printf("syntax: ldv [method(r/w/m/d)] [offset(Hex)] [mask[Hex, m only]] [value(hex, w/m only)]\n");
		printf("read example : ldv r 18\n");
		printf("write example : ldv w 18 a8\n");
		printf("write with mask example : ldv m 18 f0 a0\n");
		printf("dump example : ldv d\n");
		return 0;
	}
	
	p = argv[1];
	if (*p == 'r')
	{
		method = LINE_DRIVER_READ;
	}
	else if (*p == 'd')
	{
		method = LINE_DRIVER_DUMP;
	}
	else if (*p == 'w')
	{
		method = LINE_DRIVER_WRITE;
	}
	else if (*p == 'm')
	{
		mask = 0xff;
		method = LINE_DRIVER_WRITE; // write with mask
	}
	else
	{
		printf("supported method are r w m d\n");
	}
	
	if (method != LINE_DRIVER_DUMP)
	{
		p = argv[2];
		if (*p == '0' && *(p+1) == 'x')
			p += 2;
		if (strlen(p) > 2)
		{
			printf("invalid offset\n");
		}
    	
		while (*p != '\0')
		{
			if (*p >= '0' && *p <= '9')
				offset = offset * 16 + *p - 48;
			else 
			{
				if (*p >= 'A' && *p <= 'F')
					offset = offset * 16 + *p - 55;
				else if (*p >= 'a' && *p <= 'f')
					offset = offset * 16 + *p - 87;
				else
				{
					printf("invalid offset, should be hex value\n");
					return 0;
				}
			}
			p++;
		}
		//printf("offset = 0x%x\n", offset);
	}

	if (method == LINE_DRIVER_WRITE)
	{
		p = argv[3];
		if (*p == '0' && *(p+1) == 'x')
			p += 2;
		if (strlen(p) > 2)
		{
			if (mask)
				printf("invalid mask\n");
			else
				printf("invalid value\n");
		}
		
		while (*p != '\0')
		{
			if (*p >= '0' && *p <= '9')
				value = value * 16 + *p - 48;
			else 
			{
				if (*p >= 'A' && *p <= 'F')
					value = value * 16 + *p - 55;
				else if (*p >= 'a' && *p <= 'f')
					value = value * 16 + *p - 87;
				else
				{
					printf("invalid value\n");
					return 0;
				}
			}
			p++;
		}
		if (mask)
		{
			//printf("mask = 0x%x\n", value);

			if (value)
				method = (method | (value << 8));
			else
				printf("mask must not be 0\n");
			
			value = 0;

			p = argv[4];
			if (*p == '0' && *(p+1) == 'x')
				p += 2;
			if (strlen(p) > 2)
			{
				if (mask)
					printf("invalid mask\n");
				else
					printf("invalid value\n");
			}	
		
			while (*p != '\0')
			{
				if (*p >= '0' && *p <= '9')
					value = value * 16 + *p - 48;
				else 
				{
					if (*p >= 'A' && *p <= 'F')
						value = value * 16 + *p - 55;
					else if (*p >= 'a' && *p <= 'f')
						value = value * 16 + *p - 87;
					else
					{
						printf("invalid value\n");
						return 0;
					}
				}
				p++;
			}
		}
		//printf("value = 0x%x\n", value);
	}
	
	value = value | (offset << 8);
	
	//printf("method = %x\n", method);
	//printf("value = %x\n", value);


	fd = open("/dev/ldv0", O_RDONLY);
	if (fd < 0)
	{
		printf("Open pseudo device failed\n");
		return 0;
	}
	
	ioctl(fd, method, &value);

	close(fd);
	
	return 0;
}


