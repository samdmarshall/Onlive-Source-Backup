#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include "yaffs_guts.h"
#include "fstest.h"
#include "yaffsfs.h"

typedef struct __ftCmd__ {
	char *name; 	 /* name of command */
	int (*func)(struct __ftCmd__ *, char *); /* pointer to callback function */
	int leastArgc;
	char *help; 	 /* descriptive help string */
}fstestCmd;

int fstest_quit = 0;

static int check_argc(fstestCmd * cmd, int argc)
{
	if(argc < cmd->leastArgc) {
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	} else {
		return 0;
	}
}

int fstest_exit(fstestCmd * cmd, char * param)
{
	fstest_quit = 1;
	return 0;
}

int do_mount(char * mountpt)
{
	fstest_InitDeviceCfg();
	return yaffs_mount(mountpt);
}

int fstest_mount(fstestCmd * cmd, char * param)
{
	return do_mount(rootPrefix);
}

int fstest_umount(fstestCmd * cmd, char * param)
{
	return yaffs_unmount(rootPrefix);
}

int fstest_write_file(fstestCmd * cmd, char * param)
{
	int wordsToWrite, writtenBytes;
	char path[512];
	char * wholepath;
	int argc = 0;
	int i, h;
	int retval = 0;

	wholepath = (char *)alloca(512);

	if(param)
		argc = sscanf(param, "%s%d", path, &wordsToWrite);
	if(argc != 2){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	do_mount(rootPrefix);

	sprintf(wholepath, "%s/%s", rootPrefix, path);
	yaffs_unlink(wholepath);
	
	h = yaffs_open(wholepath, O_CREAT| O_TRUNC | O_RDWR, S_IREAD | S_IWRITE);

	for(i = 100000;i < 100000+wordsToWrite; i++){
		writtenBytes = yaffs_write(h,&i,sizeof(i));

		if(writtenBytes != 4)
		{
			printf("write error\n");
			retval = -1;break;
		}
	}
	yaffs_close(h);
	yaffs_unmount(rootPrefix);

	return retval;
}

int fstest_verify_file(fstestCmd * cmd, char * param)
{
	int wordsToVerify;
	char path[512];
	char * wholepath;
	int argc = 0;
	int i, h, x, result, error = 0;
	struct yaffs_stat state;

	wholepath = (char *)alloca(512);

	if(param)
		argc = sscanf(param, "%s%d", path, &wordsToVerify);
	if(argc != 2){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	do_mount(rootPrefix);

	sprintf(wholepath, "%s/%s", rootPrefix, path);
	h = yaffs_open(wholepath, O_RDONLY, 0);
	if(h<0) {
		error++;
		goto verify_finish;
	}
	if(yaffs_fstat(h, &state)<0) {
		error++;
		goto verify_finish;
	}
	if(state.st_size != wordsToVerify*sizeof(int)){
		error++;
		fprintf(stderr, "file size verification fails\n");
		goto verify_finish;
	}
	yaffs_lseek(h,0,SEEK_SET);

	error = 0;
	for(i = 100000; i < 100000+wordsToVerify; i++){
		result = yaffs_read(h,&x,sizeof(x));

		if(result != 4 || x != i){
			printf("read error(%x) %d->%d\n",result,i,x);
			error++;
			break;
		}
	}

verify_finish:
	yaffs_close(h);
	yaffs_unmount(rootPrefix);

	if(error){
		printf("Simple rw test: %d ERRORS!!!!!!!!!!!\n", error);
		return -1;
	} else {
		printf("Simple rw test passed!\n");
		return 0;
	}
}

int fstest_cat(fstestCmd * cmd, char * param)
{
	char path[512];
	char * wholepath;
	int argc = 0;
	int h, result;

	wholepath = (char *)alloca(512);

	if(param)
		argc = sscanf(param, "%s", path);
	if(argc != 1){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	do_mount(rootPrefix);

	sprintf(wholepath, "%s/%s", rootPrefix, path);
	h = yaffs_open(wholepath, O_RDONLY, 0);
	if(h<0) {
		return -1;
	}

	result = 0;
	while(result>=0){
		char x;
		result = yaffs_read(h,&x,sizeof(x));
		if(result == sizeof(x))
			printf("0x%02x ", x);
		if(!result)
			break;
	}

	yaffs_close(h);
	yaffs_unmount(rootPrefix);

	return 0;
}

int do_rm_path(char *path, int depth)
{
	struct yaffs_stat state;
	int retval = 0;

	if( yaffs_stat(path, &state)<0 ){
		fprintf(stderr, "cannot stat '%s'\n", path);
		return -1;
	}
	if(S_ISDIR(state.st_mode)){
		yaffs_DIR * dp;
		struct yaffs_dirent *d;

		/* Recursively diff files according to path1 */
		dp = yaffs_opendir(path);
		if (dp == NULL)
			return -1;

		while ((d = yaffs_readdir(dp)) != NULL) {
			char new_path[512];

			if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
				continue;

			sprintf(new_path, "%s/%s", path, d->d_name);
			if (do_rm_path(new_path, depth+1) < 0){
				return -1;
			}
		}
		yaffs_closedir(dp);
		if(depth)
			retval = yaffs_rmdir(path);
	}else{
		retval = yaffs_unlink(path);
	}
	return retval;
}

int fstest_rm(fstestCmd * cmd, char * param)
{
	char path[512];
	char * wholepath;
	int argc = 0;
	int result;

	wholepath = (char *)alloca(512);

	if(param)
		argc = sscanf(param, "%s", path);
	if(argc != 1){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	sprintf(wholepath, "%s/%s", rootPrefix, path);

	do_mount(rootPrefix);
	result = do_rm_path(wholepath, 0);
	yaffs_unmount(rootPrefix);

	return 0;
}

/* revised from busybox-1.11.2 */

static off_t copyfd_eof_toem(int src_fd, int dst_fd)
{
	int buffer_size = 2048;
	char * buffer;
	off_t total = 0;
	int rd, wr;

	buffer = (char *)alloca(buffer_size);

	while(1){
		rd = read(src_fd, buffer, buffer_size);

		if (!rd) /* eof - all done */
			break;

		if (rd < 0)
			break;

		if (dst_fd >= 0) {
			wr = yaffs_write(dst_fd, buffer, rd);
			if (wr < rd) {
				fprintf(stderr, "copy error\n");
				break;
			}
		}
		total += rd;
	}
	return (rd<0) ? -1 : total;
}

/* Return:
 * -1 error, copy not made
 *  0 copy is made or user answered "no" in interactive mode
 */
int copy_file_toem(const char *source, const char *dest)
{
	/* This is a recursive function, try to minimize stack usage */
	/* NB: each struct stat is ~100 bytes */
	struct stat source_stat;
	struct yaffs_stat dest_stat;
	signed char retval = 0;
	signed char dest_exists = 0;
	signed char ovr;

	if (lstat(source, &source_stat) < 0) {
		fprintf(stderr, "cannot stat '%s'\n", source);
		return -1;
	}
	if (yaffs_stat(dest, &dest_stat) < 0) {
		if (yaffsfs_GetLastError() != (-ENOENT)) {
			fprintf(stderr, "cannot stat '%s'\n", dest);
			return -1;
		}
	} else {
		dest_exists = 1;
	}

	if (S_ISDIR(source_stat.st_mode)) {
		DIR *dp;
		struct dirent *d;

		/* Create DEST */
		if (dest_exists) {
			if (!S_ISDIR(dest_stat.st_mode)) {
				fprintf(stderr, "target '%s' is not a directory\n", dest);
				return -1;
			}
			/* race here: user can substitute a symlink between
			 * this check and actual creation of files inside dest */
		} else {
			mode_t mode;

			mode = source_stat.st_mode;

			/* Allow owner to access new dir (at least for now) */
			mode |= S_IRWXU;
			if (yaffs_mkdir(dest, mode) < 0) {
				if(yaffsfs_GetLastError() != -EEXIST){
					fprintf(stderr, "cannot create directory '%s'\n", dest);
					return -1;
				}
			}

			/* need stat info for add_to_ino_dev_hashtable */
			if (yaffs_stat(dest, &dest_stat) < 0) {
				fprintf(stderr, "cannot stat '%s'\n", dest);
				return -1;
			}
		}

		/* Recursively copy files in SOURCE */
		dp = opendir(source);
		if (dp == NULL) {
			return -1;
		}

		while ((d = readdir(dp)) != NULL) {
			char new_source[512];
			char new_dest[512];

			if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
				continue;

			sprintf(new_source, "%s/%s", source, d->d_name);
			sprintf(new_dest, "%s/%s", dest, d->d_name);
			if (copy_file_toem(new_source, new_dest) < 0)
				retval = -1;
		}
		closedir(dp);

		if (!dest_exists
		 && yaffs_chmod(dest, source_stat.st_mode) < 0
		) {
			fprintf(stderr, "cannot preserve %s of '%s'", "permissions\n", dest);
			/* retval = -1; - WRONG! copy *WAS* made */
		}

	} else if (S_ISREG(source_stat.st_mode)) {

		int src_fd;
		int dst_fd;
		mode_t new_mode;

		src_fd = open(source, O_RDONLY, 0);
		if (src_fd < 0)
			return -1;

		new_mode = source_stat.st_mode;

		/* safe way: unlink an existing one and create an exclusive one */
		dst_fd = yaffs_open(dest, O_WRONLY|O_CREAT|O_EXCL, new_mode);
		if (dst_fd == -1) {
			ovr = yaffs_unlink(dest);
			if (ovr < 0) {
				fprintf(stderr, "cannot unlink %s before cp\n", dest);
				close(src_fd);
				return ovr;
			}
			/* It shouldn't exist. */
			dst_fd = yaffs_open(dest, O_WRONLY|O_CREAT|O_EXCL, new_mode);
			if (dst_fd < 0) {
				close(src_fd);
				return -1;
			}
		}

		if (copyfd_eof_toem(src_fd, dst_fd) == -1)
			retval = -1;
		/* Ok, writing side I can understand... */
		if (yaffs_close(dst_fd) < 0) {
			fprintf(stderr, "cannot close '%s'\n", dest);
			retval = -1;
		}
		/* ...but read size is already checked by bb_copyfd_eof */
		close(src_fd);
	}

	return retval;
}

/* copy source file out of the emulator */
static off_t copyfd_eof_frem(int src_fd, int dst_fd)
{
	int buffer_size = 2048;
	char * buffer;
	off_t total = 0;
	int rd, wr;

	buffer = (char *)alloca(buffer_size);

	while(1){
		rd = yaffs_read(src_fd, buffer, buffer_size);

		if (!rd) /* eof - all done */
			break;

		if (rd < 0)
			break;

		if (dst_fd >= 0) {
			wr = write(dst_fd, buffer, rd);
			if (wr < rd) {
				break;
			}
		}
		total += rd;
	}
	return (rd<0 || wr<rd) ? -1 : total;
}

/* Return:
 * -1 error, copy not made
 *  0 copy is made or user answered "no" in interactive mode
 */
int copy_file_frem(const char *source, const char *dest)
{
	/* This is a recursive function, try to minimize stack usage */
	/* NB: each struct stat is ~100 bytes */
	struct yaffs_stat source_stat;
	struct stat dest_stat;
	signed char retval = 0;
	signed char dest_exists = 0;
	signed char ovr;

	if (yaffs_lstat(source, &source_stat) < 0) {
		fprintf(stderr, "cannot stat '%s'\n", source);
		return -1;
	}
	if (lstat(dest, &dest_stat) < 0) {
		if (errno != (ENOENT)) {
			fprintf(stderr, "cannot stat '%s'\n", dest);
			return -1;
		}
	} else {
		dest_exists = 1;
	}

	if (S_ISDIR(source_stat.st_mode)) {
		yaffs_DIR *dp;
		struct yaffs_dirent *d;

		/* Create DEST */
		if (dest_exists) {
			if (!S_ISDIR(dest_stat.st_mode)) {
				fprintf(stderr, "avoid duplicate target copy: '%s' is not a directory\n", dest);
				return -1;
			}
			/* race here: user can substitute a symlink between
			 * this check and actual creation of files inside dest */
		} else {
			mode_t mode;

			mode = source_stat.st_mode;

			/* Allow owner to access new dir (at least for now) */
			mode |= S_IRWXU;
			if (mkdir(dest, mode) < 0) {
				if(errno != -EEXIST){
					fprintf(stderr, "cannot create directory '%s'\n", dest);
					return -1;
				}
			}

			/* need stat info for add_to_ino_dev_hashtable */
			if (lstat(dest, &dest_stat) < 0) {
				fprintf(stderr, "cannot stat '%s'\n", dest);
				return -1;
			}
		}

		/* Recursively copy files in SOURCE */
		dp = yaffs_opendir(source);
		if (dp == NULL) {
			return -1;
		}

		while ((d = yaffs_readdir(dp)) != NULL) {
			char new_source[512];
			char new_dest[512];

			if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
				continue;

			sprintf(new_source, "%s/%s", source, d->d_name);
			sprintf(new_dest, "%s/%s", dest, d->d_name);
			if (copy_file_frem(new_source, new_dest) < 0)
				retval = -1;
		}
		yaffs_closedir(dp);

		if (!dest_exists
		 && chmod(dest, source_stat.st_mode) < 0
		) {
			fprintf(stderr, "cannot preserve %s of '%s'", "permissions\n", dest);
			/* retval = -1; - WRONG! copy *WAS* made */
		}

	} else if (S_ISREG(source_stat.st_mode)) {

		int src_fd;
		int dst_fd;
		mode_t new_mode;

		src_fd = yaffs_open(source, O_RDONLY, 0);
		if (src_fd < 0)
			return -1;

		new_mode = source_stat.st_mode;

		/* safe way: unlink an existing one and create an exclusive one */
		dst_fd = open(dest, O_WRONLY|O_CREAT|O_EXCL, new_mode);
		if (dst_fd == -1) {
			ovr = unlink(dest);
			if (ovr < 0) {
				fprintf(stderr, "cannot unlink %s before cp\n", dest);
				yaffs_close(src_fd);
				return ovr;
			}
			/* It shouldn't exist. */
			dst_fd = open(dest, O_WRONLY|O_CREAT|O_EXCL, new_mode);
			if (dst_fd < 0) {
				yaffs_close(src_fd);
				return -1;
			}
		}

		if (copyfd_eof_frem(src_fd, dst_fd) == -1)
			retval = -1;
		/* Ok, writing side I can understand... */
		if (close(dst_fd) < 0) {
			fprintf(stderr, "cannot close '%s'\n", dest);
			retval = -1;
		}
		/* ...but read size is already checked by bb_copyfd_eof */
		yaffs_close(src_fd);
	} else if (S_ISLNK(source_stat.st_mode)) {
		char symdest[4096];

		retval = yaffs_readlink( source, symdest, sizeof(symdest));
		if(retval<0) {
			fprintf(stderr, "cannot read the content of a symbolic link '%s'\n", source);
			return retval;
		}
		unlink(dest);
		if(symlink(symdest, dest)<0){
			fprintf(stderr, "can't create a symbolic link '%s' -> '%s'\n", dest, symdest);
			return -1;
		}
	} else if (S_ISSOCK(source_stat.st_mode)) {
		unlink(dest);
		if( mknod(dest, 0644|S_IFSOCK, 0) < 0){
			fprintf(stderr, "%s", dest);
			perror(NULL);
			return -1;
		}
	} else if (S_ISFIFO(source_stat.st_mode)) {
		unlink(dest);
		if( mkfifo(dest, 0644) < 0){
			fprintf(stderr, "%s", dest);
			perror(NULL);
			return -1;
		}
	} else if (S_ISCHR(source_stat.st_mode)) {
		unlink(dest);
		if(mknod(dest, 0644|S_IFCHR, source_stat.st_dev) < 0) {
			fprintf(stderr, "%s", dest);
			perror(NULL);
			return -1;
		}
	} else if (S_ISBLK(source_stat.st_mode)) {
		unlink(dest);
		if( mknod(dest, 0644|S_IFBLK, source_stat.st_dev) < 0) {
			fprintf(stderr, "%s", dest);
			perror(NULL);
			return -1;
		}
	}

	return retval;
}
#define HOLE_PATTERN 0xA5
int check_pattern(const void *buffer, unsigned char patt, int size)
{
	int i;
	for (i=0;i<size;i++)
		if (((const unsigned char*)buffer)[i] != patt)
			return 1;
	return 0;
}
/* dst_regfile should be subset of src_regfile */
int do_diff_reg_subset(char *src_regfile, char *dst_regfile)
{
	struct stat src_stat;
	struct yaffs_stat dst_stat;
	int retval = 0;
	int fd1, fd2;
	char * buffer1;
	char * buffer2;
	int nread;
	int ntry;

	buffer1 = (char *)alloca(2032);
	buffer2 = (char *)alloca(2032);

	if( lstat(src_regfile, &src_stat)<0 ){
		fprintf(stderr, "cannot stat '%s'.", src_regfile);
		perror(NULL);
		return -1;
	}
	if( yaffs_stat(dst_regfile, &dst_stat)<0 ){
		fprintf(stderr, "cannot stat '%s'.", dst_regfile);
		perror(NULL);
		return -1;
	}
	if( !S_ISREG(dst_stat.st_mode) ){
		fprintf(stderr, "%s not a regular file\n", dst_regfile);
		return -1;
	}
	if (src_stat.st_size < dst_stat.st_size) {
		fprintf(stderr, "two files mismatch in size src '%s(%lu)' < dst '%s(%lu)'\n",\
			src_regfile, src_stat.st_size, dst_regfile, dst_stat.st_size);
		return -1;
	}

	fd1 = open(src_regfile, O_RDONLY, 0);
	if(fd1<0){
		fprintf(stderr, "cannot open %s, error %d\n", src_regfile, errno);
		return -1;
	}
	fd2 = yaffs_open(dst_regfile, O_RDONLY, 0);
	if(fd2<0){
		fprintf(stderr, "cannot open %s, error %d\n", dst_regfile, errno);
		return -1;
	}

	ntry = 0;
	nread = 0;
	while(1) {
		ssize_t rd1, rd2;
		int diff = 0;
		int i;
		off_t pos;
		
		pos = yaffs_lseek(fd2, 0, SEEK_CUR);

		rd1 = read(fd1, buffer1, 2032);
		rd2 = yaffs_read(fd2, buffer2, 2032);

		if (rd1<0 || rd2<0 || rd1<rd2) {
			fprintf(stderr, "read error during %s\n", __FUNCTION__);
			retval = -1;
			break;
		}

		nread += rd2;
		for (i=0; i<rd2; i++) {
			if(buffer1[i] != buffer2[i] && buffer2[i]!=HOLE_PATTERN) {
				fprintf(stderr, "[%ld, 0x%lX] 0x%02X -> 0x%02X\n", pos+i, pos+i, buffer1[i], buffer2[i]);
				diff = 1;
			}
		}
		if (diff) {
			if (!check_pattern(buffer2, HOLE_PATTERN, rd2)) {
				fprintf(stdout, "'%s', hole is met at off %ld\n", dst_regfile, pos);
				continue;
			}
			fprintf(stderr, "src '%s' differs with dst '%s' at offset %ld rd1 %d rd2 %d\n", \
				src_regfile, dst_regfile, pos, rd1, rd2);
			retval = -1;
			break;
		}

		/* finish reading (maybe) smaller file */
		if(nread==dst_stat.st_size) {
			if (dst_stat.st_size < src_stat.st_size)
				printf("reach end of dst(%ld)\n", dst_stat.st_size);
			break;
		} else {
			ntry++;
			if(ntry > 1000000){
			    off_t pos = yaffs_lseek(fd2, 0, SEEK_CUR);
				fprintf(stderr, "%s try=%d, can't reach end of file2 len(%ld), pos(%ld), nread(%d)\n", \
					dst_regfile, ntry, dst_stat.st_size, pos, nread);
				break;
			}
		}
	}
	close(fd1);
	yaffs_close(fd2);
	return retval;
}

int do_diff_reg(char *regfile1, char *regfile2)
{
	struct stat stat1;
	struct yaffs_stat stat2;
	int retval = 0;
	int fd1, fd2;
	int buffer_size = 2048;
	char * buffer1;
	char * buffer2;

	buffer1 = (char *)alloca(buffer_size);
	buffer2 = (char *)alloca(buffer_size);

	if( lstat(regfile1, &stat1)<0 ){
		fprintf(stderr, "cannot stat '%s'\n", regfile1);
		return -1;
	}
	if( yaffs_stat(regfile2, &stat2)<0 ){
		fprintf(stderr, "cannot stat '%s'\n", regfile2);
		return -1;
	}
	if( !S_ISREG(stat2.st_mode) ){
		fprintf(stderr, "%s not a regular file\n", regfile2);
		return -1;
	}
	if (stat1.st_size!=stat2.st_size) {
		fprintf(stderr, "two file mismatch in size %s %s\n", regfile1, regfile2);
		return -1;
	}
	fd1 = open(regfile1, O_RDONLY, 0);
	if(fd1<0){
		fprintf(stderr, "cannot open %s\n", regfile1);
		return -1;
	}
	fd2 = yaffs_open(regfile2, O_RDONLY, 0);
	if(fd2<0){
		fprintf(stderr, "cannot open %s, error %d\n", regfile2, yaffsfs_GetLastError());
		return -1;
	}

	while(1){
		ssize_t rd1, rd2;
		rd1 = read(fd1, buffer1, buffer_size);
		rd2 = yaffs_read(fd2, buffer2, buffer_size);

		if(rd1<0 || rd2<0 || rd1!=rd2 ){
			fprintf(stderr, "read error during %s\n", __FUNCTION__);
			retval = -1;
			break;
		}
		if(!rd1){
			//printf("same between %s & %s\n", regfile1, regfile2);
			break;
		}
		if(memcmp(buffer1, buffer2, rd1)){
			fprintf(stderr, "%s differs with %s\n", regfile1, regfile2);
			retval = -1;
			break;
		}
	}
	close(fd1);
	yaffs_close(fd2);
	return retval;
}
int do_diff_path(char *pathOutEm, char *wholePathInEm)
{
	struct stat stat1;
	struct yaffs_stat stat2;
	int retval = 0;

	if( lstat(pathOutEm, &stat1)<0 ){
		fprintf(stderr, "cannot stat '%s'\n", pathOutEm);
		perror(NULL);
		return -1;
	}
	if( yaffs_stat(wholePathInEm, &stat2)<0 ){
		fprintf(stderr, "cannot stat '%s'\n", wholePathInEm);
		return -1;
	}
	if( S_ISDIR(stat1.st_mode)){
		DIR *dp;
		struct dirent *d;

		/* Recursively diff files according to pathOutEm */
		dp = opendir(pathOutEm);
		if (dp == NULL) {
			return -1;
		}

		while ((d = readdir(dp)) != NULL) {
			char new_source[512];
			char new_dest[512];

			if (!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
				continue;

			sprintf(new_source, "%s/%s", pathOutEm, d->d_name);
			sprintf(new_dest, "%s/%s", wholePathInEm, d->d_name);
			if (do_diff_path(new_source, new_dest) < 0){
				retval = -1;
			}
		}
		closedir(dp);
	} else if (S_ISREG(stat1.st_mode)){
		//return do_diff_reg(pathOutEm, wholePathInEm);
		return do_diff_reg_subset(pathOutEm, wholePathInEm);
	} else {
		printf("skip file type:%x\n", stat1.st_mode);
	}

	return retval;
}

int fstest_diff(fstestCmd * cmd, char * param)
{
	char pathOutEm[512];
	char pathInEm[512];
	char wholePathInEm[512];
	int argc = 0;
	int ret;

	if(param)
		argc = sscanf(param, "%s%s", pathOutEm, pathInEm);
	if(check_argc(cmd, argc)) {
		return 1;
	}

	sprintf(wholePathInEm, "%s/%s", rootPrefix, pathInEm);
	do_mount(rootPrefix);
	ret = do_diff_path(pathOutEm, wholePathInEm);
	yaffs_unmount(rootPrefix);

	if(ret)
		printf("differs!!!!\n");
	else
		printf("the same!!!\n");
	return ret;
}

int fstest_cpfrem(fstestCmd * cmd, char * param)
{
	char source[512];
	char whole_path[512];
	char dest[512];
	int argc = 0;

	if(param)
		argc = sscanf(param, "%s%s", source, dest);
	if(argc != 2){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	sprintf(whole_path, "%s/%s", rootPrefix, source);
	do_mount(rootPrefix);
	copy_file_frem(whole_path, dest);
	yaffs_unmount(rootPrefix);
	return 0;
}

int fstest_cptoem(fstestCmd * cmd, char * param)
{
	int argc = 0;
	char pathOutEm[512];
	char pathInEm[512];
	char whole_path[512];

	if(param)
		argc = sscanf(param, "%s%s", pathOutEm, pathInEm);
	if(check_argc(cmd,argc)){
		return 1;
	}

	sprintf(whole_path, "%s/%s", rootPrefix, pathInEm);
	do_mount(rootPrefix);
	copy_file_toem(pathOutEm, whole_path);
	yaffs_unmount(rootPrefix);
	return 0;
}

int fstest_verify(fstestCmd * cmd, char * param)
{
	int res = 0;
	int argc = 0;
	char pathOutEm[512];
	char pathInEm[512];
	char whole_path[512];
	int iterations = 0;
	int nErr=0;
	int i;

	if(param)
		argc = sscanf(param, "%s%s%d", pathOutEm, pathInEm, &iterations);
	if(check_argc(cmd, argc)){
		return 1;
	}

	sprintf(whole_path, "%s/%s", rootPrefix, pathInEm);
	for(i=0;i<iterations;i++){
		do_mount(rootPrefix);
		copy_file_toem(pathOutEm, whole_path);
		yaffs_unmount(rootPrefix);

		do_mount(rootPrefix);
		res = do_diff_path(pathOutEm, whole_path);
		if(res<0)
			nErr++;
		yaffs_unmount(rootPrefix);
	}
	if(res)
		printf("%d errors in verify test!\n", nErr);
	else
		printf("verify test passed!\n");

	return res;
}

int fstest_fill_with_dirs(fstestCmd * cmd, char * param)
{
	char dirPath[512];
	int i = 0;
	int ret = 0;
	int nMkDirs = 0;

	do_mount(rootPrefix);

	while(1){
		sprintf(dirPath, "%s/%d", rootPrefix, i);
		ret = yaffs_mkdir(dirPath, 0644);
		if(ret<0)
			break;
		else
			nMkDirs++;
		i++;
	}

	yaffs_unmount(rootPrefix);

	printf("%d directories created\n", nMkDirs);
	return 0;
}

void dump_directory_tree_worker(const char *dname,int recursive)
{
	yaffs_DIR *d;
	yaffs_dirent *de;
	struct yaffs_stat s;
	char str[1000];

	d = yaffs_opendir(dname);

	if(!d)
	{
		printf("opendir %s failed\n", dname);
	}
	else
	{
		while((de = yaffs_readdir(d)) != NULL)
		{
			sprintf(str,"%s/%s",dname,de->d_name);

			yaffs_lstat(str,&s);

			printf("%s inode %d obj %x length %d mode %X ",str,s.st_ino,de->d_dont_use,(int)s.st_size,s.st_mode);
			switch(s.st_mode & S_IFMT)
			{
				case S_IFREG: printf("data file"); break;
				case S_IFDIR: printf("directory"); break;
				case S_IFLNK: 
					printf("symlink -->");
				  if(yaffs_readlink(str,str,100) < 0)
						printf("no alias");
				  else
						printf("\"%s\"",str);    
				  break;
				default: printf("unknown"); break;
			}

			printf("\n");

			if((s.st_mode & S_IFMT) == S_IFDIR && recursive)
				dump_directory_tree_worker(str,1);
		}

		yaffs_closedir(d);
	}

}

void dump_directory_tree(const char *dname)
{
	dump_directory_tree_worker(dname,1);
	printf("\n");
	printf("Free space in %s is %d\n\n",dname,(int)yaffs_freespace(dname));
}


static int fstest_ls(fstestCmd * cmd, char * param)
{
	int recursive;
	char path[512];
	char * wholepath;
	int argc = 0;

	wholepath = (char *)alloca(512);

	if(param)
		argc = sscanf(param, "%s%d",path,&recursive);
	if(argc != 2){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	do_mount(rootPrefix);

	sprintf(wholepath, "%s/%s", rootPrefix, path);
	dump_directory_tree_worker(wholepath, recursive);

	yaffs_unmount(rootPrefix);

	return 0;
}

int do_read_cmd(char * filename)
{
	char buffer[MAX_LINE+1];
	FILE *fp;

	if( (fp=fopen(filename, "r")) ) /* open command file */
	{
		printf("Reading command from file '%s'\n", filename);
		while (fgets(buffer, MAX_LINE, fp) && !fstest_quit) /* read lines until 'quit' */
		{
			fstest_parse_line(buffer); /* process line as typed in */
		}

		fclose(fp);
		return 0;
	}
	else {
		fprintf(stderr,"Error: cannot read configuration file '%s'\n",
					filename);
		return 1;
	}
}

int fstest_load_cmd(fstestCmd * cmd, char * param)
{
	char filePath[MAX_LINE+1];
	int argc = 0;

	argc = sscanf(param, "%s", filePath);
	if(argc != 1){
		if(cmd->help)
			printf("%s\n", cmd->help);
		return 1;
	}

	return do_read_cmd(filePath);
}

int fstest_reconfig(fstestCmd * cmd, char * param)
{
	fstest_yaffs_StartUp(".ftconfig", NULL);
	return 0;
}

int fstest_cmdhelp(fstestCmd * cmd, char * param);

static fstestCmd fstestCmdList[] = 
{
	{"mount", fstest_mount, 0, "mount"},
	{"umount", fstest_umount, 0, "umount"},
	{"write", fstest_write_file, 2, "write PATH_in_sim wordsToWrite"},
	{"verifyfile", fstest_verify_file, 2, "verifyfile PATH_in_sim wordsToVerify"},
	{"fillwithdirs", fstest_fill_with_dirs, 0, "fill NAND with directories"},
	{"cptoem", fstest_cptoem, 2, "cptoem DIR_out_sim DIR_in_sim Iterations"},
	{"verify", fstest_verify, 3, "verify DIR_out_sim DIR_in_sim Iterations"},
	{"diff", fstest_diff, 2, "diff DIR_out_sim DIR_in_sim"},
	{"cat", fstest_cat, 1, "cat PATH_in_sim"},
	{"cpfrem", fstest_cpfrem, 2, "cpfrem DIR_in_sim DIR_out_sim"},
	{"ls" , fstest_ls, 2, "ls PATH recursive [1|0]"},
	{"rm", fstest_rm, 1, "rm PATH_in_sim"},

	{"quit", fstest_exit, 0, "exit"},
	{"exit", fstest_exit, 0, "exit"},
	{"q", fstest_exit, 0, "exit"},
	{"help", fstest_cmdhelp, 0, "print fstest command information"},

	{"load", fstest_load_cmd, 1, "load CMD_PATH"},
	{"reconfig", fstest_reconfig, 0, "reconfig using .ftconfig"},
	{NULL, NULL, 0, NULL}
};

int fstest_cmdhelp(fstestCmd * cmd, char * param)
{
	int i;
	for (i=0; fstestCmdList[i].name; i++) {/* walk command table */
		printf("%s: \t\t", fstestCmdList[i].name);
		printf("%s\n", fstestCmdList[i].help);
	}

	return 0;
}

int fstest_parse_line(char * buffer)
{
	int result=0; /* default return status */
	int len; /* length of parsed command */
	int i; /* traversal variable for command table */

	if (*buffer == '#')
		return 0;

	if (*buffer=='!') /* check for shell escape */
		result = system((strlen(buffer)>1)? buffer+1 : getenv("SHELL"));
	else {
		for (i=0; fstestCmdList[i].name; i++) /* walk command table */
			if (!strncmp(fstestCmdList[i].name,buffer,
									len=strlen(fstestCmdList[i].name)))
			{ /* if command matches... */
				result=(fstestCmdList[i].func)
					(&fstestCmdList[i], ((int)strlen(buffer)>len)?buffer+len+1:NULL );
				break; /* call function and pass remainder of line as parameter */
			}

		 if (!fstestCmdList[i].name) /* if no commands were called... */
				printf("Eh? %s\n",buffer); /* tribute to Canadian diction */
	}

	return(result); /* return 1 unless exit requested, then return 0 */
}

char *fstest_read_line(char * buffer, int size)
{
   char *result;

   printf("%s",PROMPT);                 /* print prompt */
   fflush(stdout);                      /* force prompt to print */
   if( (result=fgets(buffer,size,stdin)) ) /* read line safely */
   {
      if (!strcmp(buffer,"?"))           /* translate aliases */
         strcpy(buffer,"help");
      if (!strcmp(buffer,"exit"))
         strcpy(buffer,"quit");
   }

   return(result);                      /* return success of fgets */
}

void fstest_usage(int argc, char ** argv)
{
printf(
	"Usage: %s [OPTION] [INPUTFILE]\n"
	"\n"
	"  -s, --singlecmd CMD_LINE  Use a single command rather than the command batch file.\n"
	"                            If there is any space in CMD_LINE, please use \" to quote it!\n"
	"                            i.e. %s -s \"ls / 0\"\n"
	"  -h, --help 	             Display this help\n"
	, argv[0], argv[0]
);
}

char defCmdFile[] = ".ftcmd";
char defConfigFile[] = ".ftconfig";

extern unsigned errACPPageSeq;

int main(int argc, char ** argv)
{
	char buffer[MAX_LINE+1];
	char * configFileName = defConfigFile;
	char * cmdFileName = defCmdFile;
	char * singleCmd = NULL;
	char * diskFilePath = NULL;

	for (;;) {
		static const char *short_options="hb:s:";
		static const struct option long_options[] = {
			{ "help", no_argument, 0, 'h' },
			{ "configfile", required_argument, 0, 'c' },
			{ "cmdfile", required_argument, 0, 'm' },
			{ "singlecmd", required_argument, 0, 's' },
			{0, 0, 0, 0},
		};
		int option_index = 0;
		int c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == EOF)
			break;

		switch (c) {
		case 'h':
			fstest_usage(argc, argv);
			exit(0);
		case '?':
			fstest_usage(argc, argv);
			exit(1);

		case 'c':
			configFileName = optarg;
			break;

		case 'm':
			cmdFileName = optarg;
			break;

		case 's':
			singleCmd = optarg;
			break;
		}
	}

	if((argc - optind) == 1) {
		diskFilePath = argv[optind];
	}

	fstest_yaffs_StartUp(configFileName, diskFilePath);

	if(singleCmd) {
		fstest_parse_line(singleCmd);
	} else {
		do_read_cmd( cmdFileName );

		while (!fstest_quit && fstest_read_line(buffer,MAX_LINE))
			fstest_parse_line(buffer);
	}

	fstest_yaffs_Shutdown();

	return 0;
}

