/*
 * ramdisk.c
 *
 *  Created on: Nov 12, 2016
 *      Author: dinesh
 */

// Added because of compatibility issues in fust_main()
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#include"ramnode.h"

static int ramdisk_getattr(const char *path, struct stat *stbuf) {
	int res = 0;
	printf("MKD-getattr\n");
	memset(stbuf, 0, sizeof(struct stat));
	res = stat(path, stbuf);

	return res;
}

static int ramdisk_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {
	DIR *dp;
	struct dirent *de;
	printf("MKD-readdir\n");
	dp = opendir(path);
	if (dp == NULL)
		return errno;
	else {
		/*while ((de = readdir(dp)) != NULL) {
		 struct stat st;
		 memset(&st, 0, sizeof(st));
		 st.st_ino = de->d_ino;
		 st.st_mode = de->d_type << 12;
		 if (filler(buf, de->d_name, &st, offset))    // MKD - Might need to change
		 break;
		 }*/
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
	}
	return 0;
}

static int ramdisk_mkdir(const char *path, mode_t mode) {
	// int res;
	printf("MKD-mkdir\n");
	/*res = mkdir(path, mode);
	if (res == -1)
		return -errno;
	else
		return 0;*/
	return 0;
}
static void* ramdisk_init(struct fuse_conn_info *conn) {
	printf("MKD-init\n");
	return NULL;
}


static void ramdisk_destroy(void* private_data) {
	printf("MKD-destroy\n");
}

static int ramdisk_fgetattr(const char* path, struct stat* stbuf) {
	printf("MKD-fgetattr\n");
	return 0;
}

static int ramdisk_access(const char* path, int mask) {
	printf("MKD-Access\n");
	return 0;
}

static int ramdisk_unlink(const char* path){
	printf("MKD-Unlink\n");
	return 0;
}

static int ramdisk_rmdir(const char* path) {
	printf("MKD-remove DIR\n");
	return 0;
}

static int ramdisk_rename(const char* from, const char* to) {
	printf("MKD-Rename \n");
	return 0;
}

static int ramdisk_chmod(const char* path, mode_t mode) {
	printf("MKD-chmod\n");
	return 0;
}

static int ramdisk_chown(const char* path, uid_t uid, gid_t gid) {
	printf("MKD-CHown\n");
	return 0;
}

static int ramdisk_truncate(const char* path, off_t size) {
	printf("MKD-truncate\n");
	return 0;
}

static int ramdisk_ftruncate(const char* path, off_t size) {
	printf("MKD-ftruncate\n");
	return 0;
}


static int ramdisk_utimens(const char* path, const struct timespec ts[2]) {
	printf("MKD-utimens\n");
	return 0;
}

static int ramdisk_create(const char * path, mode_t mode, struct fuse_file_info *fi) {
	printf("MKD-create\n");
	return 0;
}

static int ramdisk_open(const char* path, struct fuse_file_info* fi) {
	printf("MKD-open\n");
	return 0;
}

static int ramdisk_read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	printf("MKD-read\n");
	return 0;
}

static int ramdisk_write(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	printf("MKD-write\n");
	return 0;
}

static int ramdisk_statfs(const char* path, struct statvfs* stbuf) {
	printf("MKD-statfs\n");
	return 0;
}

static int ramdisk_release(const char* path, struct fuse_file_info *fi) {
	printf("MKD-release\n");
	return 0;
}

static int ramdisk_opendir(const char* path, struct fuse_file_info* fi) {
	printf("MKD-opendir\n");
	return 0;
}

static int ramdisk_releasedir(const char* path, struct fuse_file_info *fi) {
	printf("MKD-release dir\n");
	return 0;
}

static int ramdisk_fsync(const char* path, int isdatasync, struct fuse_file_info* fi) {
	printf("MKD-fsync\n");
	return 0;
}

static int ramdisk_flush(const char* path, struct fuse_file_info* fi) {
	printf("MKD-flush\n");
	return 0;
}

static int ramdisk_fsyncdir(const char* path, int isdatasync, struct fuse_file_info* fi) {
	printf("MKD-fsyncdir\n");
	return 0;
}

static int ramdisk_lock(const char* path, struct fuse_file_info* fi, int cmd, struct flock* locks) {
	printf("MKD-lock\n");
	return 0;
}

static int ramdisk_bmap(const char* path, size_t blocksize, uint64_t* blockno) {
	printf("MKD-bmap\n");
	return 0;
}

static int ramdisk_ioctl(const char* path, int cmd, void* arg, struct fuse_file_info* fi, unsigned int flags, void* data) {
	printf("MKD-ioctl\n");
	return 0;
}

static int ramdisk_poll(const char* path, struct fuse_file_info* fi, struct fuse_pollhandle* ph, unsigned* reventsp) {
	printf("MKD-poll\n");
	return 0;
}
static struct fuse_operations ramdisk_operations = {
    .init        = ramdisk_init,
    .destroy     = ramdisk_destroy,
    .getattr     = ramdisk_getattr,
    .fgetattr    = ramdisk_fgetattr,
    .access      = ramdisk_access,
    .readdir     = ramdisk_readdir,
    .mkdir       = ramdisk_mkdir,
    .unlink      = ramdisk_unlink,
    .rmdir       = ramdisk_rmdir,
    .rename      = ramdisk_rename,
    .chmod       = ramdisk_chmod,
    .chown       = ramdisk_chown,
    .truncate    = ramdisk_truncate,
    .ftruncate   = ramdisk_ftruncate,
    .utimens     = ramdisk_utimens,
    .create      = ramdisk_create,
    .open        = ramdisk_open,
    .read        = ramdisk_read,
    .write       = ramdisk_write,
    .statfs      = ramdisk_statfs,
    .release     = ramdisk_release,
    .opendir     = ramdisk_opendir,
    .releasedir  = ramdisk_releasedir,
    .fsync       = ramdisk_fsync,
    .flush       = ramdisk_flush,
    .fsyncdir    = ramdisk_fsyncdir,
    .lock        = ramdisk_lock,
    .bmap        = ramdisk_bmap,
    .ioctl       = ramdisk_ioctl,
    .poll        = ramdisk_poll,
#ifdef HAVE_SETXATTR
    .setxattr    = ramdisk_setxattr,
    .getxattr    = ramdisk_getxattr,
    .listxattr   = ramdisk_listxattr,
    .removexattr = ramdisk_removexattr,
#endif
    .flag_nullpath_ok = 0,                /* See below */
};

int main(int argc, char *argv[]) {

	return fuse_main(argc, argv, &ramdisk_operations, NULL);
}

