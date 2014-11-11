#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define IOMAPPER_DEVICE "/dev/iomapper"
#define IR_DEVICE "/dev/girl"
int main(int argc, char *argv[])
{
    FILE *file;
    int fd;
	fd_set fdset;
	unsigned int irkey;
	unsigned int kbkey;
	int ret;

    file = fopen(IR_DEVICE, "r+");
    if (!file) {
        perror("Failed to open "IR_DEVICE".\n");
        return -1;
    }
    fd = fileno(file);

	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	while (1) {
		//kbkey = getchar();
		//printf("keyboard input: %c\n", kbkey);

		ret = select(fd + 1, &fdset, NULL, NULL, NULL);
		if (ret < 0) {
			perror("select");
		} else if (ret > 0) {
			printf("select has data.\n");
			if (FD_ISSET(fd, &fdset)) {
				ret = read(fd, &irkey, sizeof(irkey));
				if (ret != sizeof(irkey))
					printf("error ir key.\n");
				else
					printf("IR key is 0x%08x\n", irkey);
			}
		} else {
			printf("no data.\n");
		}
	}

	/* close */	
    fclose(file);
    return 0;
}
