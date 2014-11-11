#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <mtd/mtd-user.h>
#include <sys/ioctl.h>

#include "do_mounts.h"
#include "kinit.h"

#include <sysinfo.h>
#include <image3_header.h>

int iNAND_vt = -1;
int blockSize = 0;
int pageSize = 0;

int execute(int *status, int stdinfd, char *file, char **argv)
{
	int pid;
	int ret;
	int stat;

	pid = fork();

	if (pid < 0) {
		if (status)
			*status = pid;
		return pid;
	}
	if (pid == 0) {
		if (stdinfd >= 0)
			dup2(stdinfd, 0);
		execv(file, argv);
		perror("execve");   /* execve() only returns on error */
		exit(errno);
	}

	ret = waitpid(pid, &stat, 0);
	if (ret < 0) {
		if (status)
			*status = ret;
		return ret;
	}
	if (WIFEXITED(stat)) {
		if (status)
			*status = stat;
	} else {
		if (status)
			*status = -1;
		return -1;
	}
	return 0;
}

version_table_t vt;
ver_table_entry_t vt_entry[32];

int get_version_table(int blockSize)
{
	int i;
	loff_t offs;

	DEBUG(("kinit: %s, TableSize %d\n", __func__, VT_SIZE));

	for (i=0; i<8; i++) {
		offs = i * blockSize + VT_OFFSET;
		fprintf(stderr, "read version table, offset=%Lu.\n", offs);
		lseek(iNAND_vt, offs, SEEK_SET);
		if((read(iNAND_vt, &vt, sizeof(version_table_t)) == sizeof(version_table_t))
		    && (vt.magic == MAGIC_NUMBER)
		    && (read(iNAND_vt, vt_entry, sizeof(ver_table_entry_t)* vt.num_entries)
		        == sizeof(ver_table_entry_t)*vt.num_entries) )
			break;
		fprintf(stderr, "vt.magic=%08x, vt.num_entries %d.\n", vt.magic, vt.num_entries);

	}
	if(i==8){
		fprintf(stderr, "read version table error.\n");
		return 1;
	}
	fprintf(stderr, "vt.magic=%08x, vt.num_entries %d.\n", vt.magic, vt.num_entries);

	return 0;
}
void is_rootfs_OU_failed(int *rootfs_failed)
{
	if (vt.ou_status == OU_UPGRADING) {
		printf("rootfs OU failure detected!\n");
		*rootfs_failed = 1;
	} else {
		printf("rootfs status is OK!\n");
		*rootfs_failed = 0;
	}
}

int load_recovery_rootfs_img_from_NAND(INT32 partind, INT32 part_size,
									   CHAR **buf, UINT32 *bufLen)
{
	CHAR *blockBuf = NULL;
	loff_t address;
	INT32 iErrorCode = 0;
	INT32 i, iNAND;
	CHAR *rootfs_img_buf = NULL;
	INT32 imglen;
	Image3_Attr *bootimgs_attr;
	INT32 skipblks;
	CHAR devname[32];

	sprintf(devname, "/dev/mtd%d", partind);

	if (access(devname, R_OK)) {
		DEBUG(("kinit: creating %s\n", devname));
		iErrorCode = mknod(devname, S_IFCHR | 0600, makedev(90, partind*2));
		fprintf(stdout, "ret=%d\n", iErrorCode);
	}
	iNAND = open(devname, O_RDONLY);
	if (iNAND == -1) {
		perror(__func__);
		fprintf(stderr, "Fail to open %s\n", devname);
		return iNAND;
	}

	blockBuf = malloc(blockSize);
	if (blockBuf == NULL)
		return -ENOMEM;

	/* get attributions of boot images */
	memset(blockBuf, 0xff, blockSize);
	address = 0;
	while (address < part_size) {
		iErrorCode = ioctl(iNAND, MEMGETBADBLOCK, &address);
		if (iErrorCode == 0) {
			lseek(iNAND, address, 0);
			if (read(iNAND, blockBuf, sizeof(Image3_Attr)) == 
					sizeof(Image3_Attr)) {
				bootimgs_attr = (Image3_Attr *)blockBuf;
				*bufLen = imglen = bootimgs_attr->recou_param.size;
				goto recovery_rootfs_found;
			}
		}
		address += blockSize;
	}
	iErrorCode = -EIO;
	goto finish;

recovery_rootfs_found:
	printf("recovery root file system image size=%d\n", imglen);

	/* allocate recovery root file system image buffer */
	*buf = rootfs_img_buf = malloc(imglen);
	if (rootfs_img_buf == NULL) {
		iErrorCode = -ENOMEM;
		goto finish;
	}

	/*
	 * skip boot image attribution block
	 */
	address += blockSize;

	i = 0;
	skipblks =
		(bootimgs_attr->sm_param.uism_image_final_size +
		 blockSize - 1)/blockSize +
		(bootimgs_attr->cpu0_param.uicpu0_image_final_size +
		 blockSize - 1)/blockSize +
		(bootimgs_attr->cpu1_param.uicpu1_image_final_size +
		 blockSize - 1)/blockSize;
	while (address < part_size) {
		INT32 toread;

		iErrorCode = ioctl(iNAND, MEMGETBADBLOCK, &address);
		if (iErrorCode == 0) {
			/*
			 * skip sm/cpu0/cpu1 blocks
			 */
			if (i < skipblks) {
				address += blockSize;
				i++;
				continue;
			}

			if (imglen > blockSize)
				toread = blockSize;
			else
				toread = imglen;

			lseek(iNAND, address, 0);
			if (read(iNAND, rootfs_img_buf, toread) != toread) {
				printf("read NAND block error @0x%Lx\n", address);
				iErrorCode = -EIO;
				goto finish1;
			}

			imglen -= toread;
			rootfs_img_buf += toread;
			i++;
			if (imglen <= 0)
				goto finish;
		}
		address += blockSize;
	}
	printf("%s,%d: too many bad blocks!\n", __func__, __LINE__);
	iErrorCode = -EIO;
	goto finish1;

finish1:
	printf("Error! Freeing rootfs_img_buf 0x%p\n", rootfs_img_buf);
	free(*buf);
	*buf = NULL;
finish:
	if (blockBuf)
		free(blockBuf);
	close(iNAND);
	return iErrorCode;
}

int load_recovery_rootfs(void)
{
	char *recovery_rootfs_buf = NULL;
	UINT32 recovery_rootfs_len = 0;
	char *argv_gzip[] = {"/bin/gzip", "-d", "/tmp/recovery_rootfs.cpio.gz", NULL};
	char *argv_cpio[] = {"/bin/cpio", "-i", "<", "/tmp/recovery_rootfs.cpio", NULL};
	int ret, i;
	ver_table_entry_t *vte;

	for (i=0; i<vt.num_entries; i++) {
		vte = vt.table+i;
		if (strcmp(vte->name, IMG3_NAME) == 0)
			goto found_img3_vte;
	}
	fprintf(stderr, "Fail to find version table entry for %s\n", IMG3_NAME);
	return -1;

found_img3_vte:
	if (vte->part1_version.version >= vte->part2_version.version) {
		ret = load_recovery_rootfs_img_from_NAND(
				2, vte->part1_blks*blockSize,
				&recovery_rootfs_buf, &recovery_rootfs_len);
	} else {
		ret = load_recovery_rootfs_img_from_NAND(
				3, vte->part2_blks*blockSize,
				&recovery_rootfs_buf, &recovery_rootfs_len);
	}

	if (ret == 0) {
		int fd;
		if (recovery_rootfs_buf == NULL) {
			fprintf(stderr, "%s, line %d: critical error\n",
					__func__, __LINE__);
			ret = -ENOMEM;
			goto finish;
		}
		mkdir("/tmp", 0755);
		ret = open("/tmp/recovery_rootfs.cpio.gz", O_CREAT|O_RDWR, 0644);
		if (ret == -1) {
			perror(__func__);
			goto finish1;
		}
		if (write(ret, recovery_rootfs_buf, recovery_rootfs_len)
				!= recovery_rootfs_len) {
			perror(__func__);
			fprintf(stderr, "Fail to write /tmp/recovery_rootfs.cpio.gz\n");
			ret = -EIO;
			close(ret);
			goto finish1;
		}
		close(ret);

		execute(&ret, -1, "/bin/gzip", argv_gzip);
		if (ret) {
			fprintf(stderr, "%s, line %d: fail to gzip\n",
					__func__, __LINE__);
			goto finish1;
		}
		chdir("/root");
		fd = open("/tmp/recovery_rootfs.cpio", O_RDONLY);
		if (fd < 0) {
			ret = fd;
			perror(__func__);
			printf("error opening recovery_rootfs.cpio\n");
			goto finish1;
		}
		execute(&ret, fd, "/bin/cpio", argv_cpio);
		if (ret) {
			fprintf(stderr, "%s, line %d: fail to cpio\n",
					__func__, __LINE__);
			close(fd);
			goto finish1;
		}
		printf("kinit: initramfs extracted\n");
		close(fd);
	}

finish1:
	free(recovery_rootfs_buf);
finish:
	return ret;
}

void do_rootfs_OU(void)
{
	int confd;
	int rootfs_failed;
	struct mtd_info_user memInfo;
	char *initargs[] = {"/init.sh", NULL};
	int ret;

	if (access("/dev/mtd1", R_OK)) {
		DEBUG(("kinit: creating /dev/mtd1\n"));
		ret = mknod("/dev/mtd1", S_IFCHR | 0600, makedev(90, 2));
		fprintf(stdout, "ret=%d\n", ret);
	}
	iNAND_vt = open("/dev/mtd1", O_RDONLY);
	if (iNAND_vt == -1) {
		perror(__func__);
		fprintf(stderr, "Can't open /dev/mtd1\n");
		return;
	}
	if (ioctl(iNAND_vt, MEMGETINFO, &memInfo)) {
		perror(__func__);
		fprintf(stderr, "ioctl failed\n");
		close(iNAND_vt);
		return;
	}
	blockSize = memInfo.erasesize;
	pageSize = memInfo.writesize;

	if (get_version_table(blockSize)) {
		fprintf(stderr, "Can't get system version table.\n");
		exit(1);
	}

	is_rootfs_OU_failed(&rootfs_failed);
	if (!rootfs_failed)
		return;

	if (load_recovery_rootfs()) {
		fprintf(stderr, "Fail to load recovery root file system\n");
		exit(1);
	}
	close(iNAND_vt);

	mknod("/root/dev/console", S_IFCHR|0644, makedev(5, 1));
	mknod("/root/dev/null", S_IFCHR|0644, makedev(1, 3));
	if (chdir("/root") || chroot(".") || chdir("/")) {
		fprintf(stderr, "Fail to chroot to /root\n");
		return;
	}
	if ((confd = open("/dev/console", O_RDWR)) < 0) {
		fprintf(stderr, "Fail to open /dev/console\n");
		return;
	}
	dup2(confd, 0);
	dup2(confd, 1);
	dup2(confd, 2);
	close(confd);
	execv("/init.sh", initargs);
	fprintf(stderr, "Fail to execute /init.sh in recovery root file system\n");
	exit(1);
}

