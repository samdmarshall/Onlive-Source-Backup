#include <stdio.h>
#include <sys/ioctl.h>

#define PFTIMER_DEVICE "/dev/pftimer"
int main(void)
{
    FILE *file;
    int fd;
    unsigned value;

    file = fopen(PFTIMER_DEVICE, "r+");
    if (!file) {
        perror("Failed to open /dev/pftimer.\n");
        return -1;
    }
    fd = fileno(file);
    if (ioctl(fd, 0x1000, &value)) {
        perror("ioctl error.\n");
        fclose(file);
        return -1;
    } else {
        printf("pftimer value: 0x%08x\n", value);
    }
    fclose(file);
    return 0;
}
