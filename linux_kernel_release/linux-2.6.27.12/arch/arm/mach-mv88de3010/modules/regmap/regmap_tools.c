#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>

#define REGMAP_DEVICE "/dev/regmap"

unsigned int str2dec(char *str)
{
	char *ptr = str;
	int base;
	unsigned int value;
	if ((ptr[0] == '0') && ((ptr[1] == 'x') || (ptr[1] == 'X')))
		base = 16;
	else
		base = 10;
	value = strtoul(str, NULL, base);
	return value;
}

int main(int argc, char *argv[])
{
	int fd;
	unsigned int *vaddr;
	unsigned int offset, length;
	int i;

	if (argc != 3) {
		printf("argument numer error %d", argc);
		printf("Usage: %s offset length\n", argv[0]);
		return -1;
	}
	offset = str2dec(argv[1]);
	length = str2dec(argv[2]);
	printf("offset=0x%08x, length=0x%08x\n", offset, length);

	fd = open(REGMAP_DEVICE, O_RDWR);
	if (fd == -1) {
		perror("failed to open");
		return -1;
	}

	vaddr = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, offset);
	if (vaddr == MAP_FAILED) {
		perror("failed to mmap");
		return -1;
	}

	for (i = 0; i < 0x1000; i++) {
		if ((i > 0) && (i % 8 == 0))
			printf("\n");
		printf("%08x ", vaddr[i]);
	}
	printf("\n");

	munmap(vaddr, length);
	return 0;
}
