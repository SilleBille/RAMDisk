/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>

#include "ramnode.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static const char *bye_str = "Bye world!\n";
static const char *bye_path = "/bye";

ramNode *head;
void testContents() {
	int numberOfBlocks;
	ramNode *n = (ramNode *) malloc(sizeof(ramNode));
	strcpy(n->name, hello_path);
	n->type = FILE_TYPE;
	n->mode = 0666;

	n->atime = time(NULL);
	n->ctime = time(NULL);
	n->mtime = time(NULL);

	numberOfBlocks = strlen(hello_str)/BLOCK_SIZE;
	if(strlen(hello_str)%BLOCK_SIZE != 0) {
		numberOfBlocks++;
	}

	printf("Number of blocks allocated: %d", numberOfBlocks);


	memBlock *mem = (memBlock *) malloc(sizeof(memBlock) * numberOfBlocks);
	strcpy(mem->data, hello_str);
	mem->next = NULL;


	n->memHead = mem;
	n->size = computeSize(mem);

	n->next = NULL;

	addNode(head, n);



	ramNode *m = (ramNode *) malloc(sizeof(ramNode));
	strcpy(m->name, bye_path);
	m->type = FILE_TYPE;
	m->mode = 0666;
	m->atime = time(NULL);
	m->ctime = time(NULL);
	m->mtime = time(NULL);


	numberOfBlocks = strlen(bye_str)/BLOCK_SIZE;
	if(strlen(bye_str)%BLOCK_SIZE != 0) {
		numberOfBlocks++;
	}

	printf("Number of blocks allocated for BYE: %d", numberOfBlocks);


	memBlock *mem1 = (memBlock *) malloc(sizeof(memBlock) * numberOfBlocks);
	strcpy(mem1->data, bye_str);
	mem1->next = NULL;


	m->memHead = mem1;
	m->size = computeSize(mem1);

	m->next = NULL;

	addNode(head, m);


}
void initNodes() {

	ramNode *n = (ramNode *) malloc(sizeof(ramNode));
	struct fuse_context *f = fuse_get_context();
	strcpy(n->name, "/");

	n->gid = f->gid;
	n->uid = f->uid;

	n->type = DIR_TYPE;
	n->mode = 0777;

	n->atime = time(NULL);
	n->ctime = time(NULL);
	n->mtime = time(NULL);

	n->memHead = NULL;
	n->next = NULL;

	head = n;

	testContents();
}


static int ramdisk_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;
	printf("path %s\n", path);
	memset(stbuf, 0, sizeof(struct stat));

	ramNode *temp = searchNode(head, path);

	if(temp == NULL)
		return -ENOENT;

	stbuf->st_ctime = temp->ctime;
	stbuf->st_atime = temp->atime;
	stbuf->st_mtime = temp->mtime;
	stbuf->st_gid = temp->gid;
	stbuf->st_uid = temp->uid;

	if (temp != NULL && temp->type == DIR_TYPE) {
		stbuf->st_mode = S_IFDIR | temp->mode;
		stbuf->st_nlink = 2;
	} else if (temp != NULL && temp->type == FILE_TYPE) {
		stbuf->st_mode = S_IFREG | temp->mode;
		stbuf->st_nlink = 1;
		stbuf->st_size = temp->size;

	}

	return res;
}

static int ramdisk_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	char *tempName;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	ramNode *temp = head;
	while (temp != NULL) {
		tempName = (char *) malloc(temp->size);
		strcpy(tempName, temp->name);

		char *parentName = dirname(tempName);
		// Check whether the node and the path are same. It SHOULDN'T be
		if(strcmp(temp->name, path) != 0) {
			// Now check whether the parent of the file and the path are same.
			if(strcmp(parentName, path) == 0) {
				strcpy(tempName, temp->name);
				filler(buf, basename(tempName), NULL, 0);
			}
		}

		temp = temp->next;
	}

	printNodes(head);

	return 0;
}

static int ramdisk_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	ramNode *temp = searchNode(head, path);
	if (temp == NULL)
		return -ENOENT;

	return 0;
}

static int ramdisk_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;

	ramNode *temp = searchNode(head, path);

	if (temp == NULL)
		return -ENOENT;
	char *contents = (char *)malloc(temp->size);
	strcpy(contents, "");

	len = temp->size;
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memBlock *m = temp->memHead;

		while(m != NULL) {
			strcat(contents, m->data);
			m = m->next;
		}
		memcpy(buf, contents + offset, size);
	} else
		size = 0;

	return size;
}

static int ramdisk_mkdir(const char *path, mode_t mode) {
	int res = 0;
	printf("The path where mkdir is being called: %s\n", path);

	ramNode *n = (ramNode *) malloc(sizeof(ramNode));
	struct fuse_context *f = fuse_get_context();

	strcpy(n->name, path);
	n->gid = f->gid;
	n->uid = f->uid;

	n->mode = mode;
	n->type = DIR_TYPE;
	n->atime = time(NULL);
	n->ctime = time(NULL);
	n->mtime = time(NULL);
	n->next = NULL;
	n->size = 0;
	n->memHead = NULL;

	addNode(head, n);

	return res;
}

static int ramdisk_rmdir(const char *path) {
	int res;

	res = deleteNode(head, path);

	return res;
}

static int ramdisk_create(const char * path, mode_t mode, struct fuse_file_info *fi) {
	int res = 0;
	printf("The path where create file is being called: %s\n", path);
	printf("The mode is: (%3o)", mode&0777);
	ramNode *n = (ramNode *) malloc(sizeof(ramNode));
	struct fuse_context *f = fuse_get_context();


	strcpy(n->name, path);
	n->gid = f->gid;
	n->uid = f->uid;

	n->mode = mode;
	n->type = FILE_TYPE;
	n->atime = time(NULL);
	n->ctime = time(NULL);
	n->mtime = time(NULL);

	n->next = NULL;
	n->size = 0;
	n->memHead = NULL;

	addNode(head, n);


	return res;
}

static int ramdisk_rename(const char *source, const char *dest) {

	ramNode *temp = searchNode(head, source);

	if(temp == NULL)
		return -ENOENT;

	strcpy(temp->name, dest);

	return 0;
}

static int ramdisk_utimens(const char* path, const struct timespec tv[2]) {
	ramNode *temp = searchNode(head, path);

	if(temp == NULL)
		return -ENOENT;

	temp->atime = tv[0].tv_sec;
	temp->mtime = tv[1].tv_sec;

	return 0;
}

static int ramdisk_write(const char *path, const char *buf, size_t size,
	     off_t offset, struct fuse_file_info *fi){

	(void) fi;
	int length;
	ramNode *n = searchNode(head, path);

	if (n == NULL)
		return -ENOENT;
	char *contents = (char *)malloc(n->size);
	strcpy(contents, "");

	memBlock *m = n->memHead;
	while(m != NULL) {
		strcat(contents, m->data);
		m = m->next;
	}

	char *finalContents = (char *)malloc(n->size + size);
	strncpy(finalContents, contents, offset);
	finalContents[offset] = '\0';
	strcat(finalContents, buf);
	strcat(finalContents, contents + offset);


	length = strlen(finalContents);
	int x=0;

	n->memHead = NULL;
	while(( length / BLOCK_SIZE) != 0) {
		m = (memBlock *)malloc(sizeof(memBlock));
		strncpy(m->data, finalContents + x, BLOCK_SIZE);
		length -=BLOCK_SIZE;
		x += BLOCK_SIZE;
		if(n->memHead == NULL) {
			m->next = NULL;
			n->memHead = m;
		}
		else
			addMemBlock(n->memHead, m);
	}
	printf("The remaining string length: %d\n", length);
	if(length != 0) {
		m = (memBlock *)malloc(sizeof(memBlock));
		strncpy(m->data, finalContents + x, length);
		m->data[length] = '\0';
		addMemBlock(n->memHead, m);
	}

	n->size = computeSize(n->memHead);

	return size;
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

static int ramdisk_unlink(const char* path) {
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
	printf("MKD-TRUNCATE------------ %s   size: %d\n", path, size);
	return 0;
}

static int ramdisk_ftruncate(const char* path, off_t size) {
	printf("MKD-ftruncate >>>>>>>>>>>>>>>>>>>>> %s      %d\n", path, size);
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

static int ramdisk_releasedir(const char* path, struct fuse_file_info *fi) {
	printf("MKD-release dir\n");
	return 0;
}

static int ramdisk_fsync(const char* path, int isdatasync,
		struct fuse_file_info* fi) {
	printf("MKD-fsync\n");
	return 0;
}

static int ramdisk_flush(const char* path, struct fuse_file_info* fi) {
	printf("MKD-flush\n");
	return 0;
}

static int ramdisk_fsyncdir(const char* path, int isdatasync,
		struct fuse_file_info* fi) {
	printf("MKD-fsyncdir\n");
	return 0;
}

static int ramdisk_lock(const char* path, struct fuse_file_info* fi, int cmd,
		struct flock* locks) {
	printf("MKD-lock\n");
	return 0;
}

static int ramdisk_bmap(const char* path, size_t blocksize, uint64_t* blockno) {
	printf("MKD-bmap\n");
	return 0;
}

static int ramdisk_ioctl(const char* path, int cmd, void* arg,
		struct fuse_file_info* fi, unsigned int flags, void* data) {
	printf("MKD-ioctl\n");
	return 0;
}

static int ramdisk_poll(const char* path, struct fuse_file_info* fi,
		struct fuse_pollhandle* ph, unsigned* reventsp) {
	printf("MKD-poll\n");
	return 0;
}














static struct fuse_operations hello_oper = {
	.getattr	= ramdisk_getattr,
	.readdir	= ramdisk_readdir,
	.open		= ramdisk_open,
	.read		= ramdisk_read,
	.mkdir 		= ramdisk_mkdir,
	.rmdir		= ramdisk_rmdir,
	.create 	= ramdisk_create,
	.rename 	= ramdisk_rename,
	.utimens 	= ramdisk_utimens,
	.write		= ramdisk_write,

	.flush 		= ramdisk_flush,
	.destroy = ramdisk_destroy,
	//.fgetattr = ramdisk_fgetattr,
	.access = ramdisk_access,
	.unlink = ramdisk_unlink,
	.chmod = ramdisk_chmod,
	.chown = ramdisk_chown,
	.truncate = ramdisk_truncate,
	.ftruncate = ramdisk_ftruncate,
	.statfs = ramdisk_statfs,
	.release = ramdisk_release,
	.releasedir = ramdisk_releasedir,
	.fsync = ramdisk_fsync,
	.flush = ramdisk_flush,
	.fsyncdir = ramdisk_fsyncdir,
	.lock = ramdisk_lock,
	.bmap = ramdisk_bmap,
	.ioctl = ramdisk_ioctl,
	.poll = ramdisk_poll,


};


int main(int argc, char *argv[])
{
	initNodes();
	return fuse_main(argc, argv, &hello_oper, NULL);
}
