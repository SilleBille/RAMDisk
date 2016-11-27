/*
 * ramnode.h
 *
 *  Created on: Nov 15, 2016
 *      Author: dinesh
 */

#ifndef RAMNODE_H_
#define RAMNODE_H_

#define MAX_FILE_NAME_SIZE 256
#define BLOCK_SIZE 1024

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>

typedef enum d_type {
	DIR_TYPE, FILE_TYPE
} f_type;

/* To maintain the file or directory meta-data
 * Not worrying about access controls
 */
typedef struct _fileMetaTable {
	char name[MAX_FILE_NAME_SIZE]; // This acts as the key

	// These are the values
	f_type type;
	mode_t mode;

	// Why the heck did I forget these
	gid_t gid;
	uid_t uid;

	size_t size;

	time_t atime;
	time_t mtime;
	time_t ctime;

	// The pointer to memory blocks
	struct _memoryBlocks *memHead;

	// Pointer to next block
	struct _fileMetaTable *next;

} ramNode;

/* To maintain the blocks allocated
 * The block size considered is 1KB
 *
 */
typedef struct _memoryBlocks {
	char data[BLOCK_SIZE+1];
	struct _memoryBlocks *next;
} memBlock;


ramNode* searchNode(ramNode *head, const char *name);
void addNode(ramNode *head, ramNode *node);
void printNodes(ramNode *head);
int computeSize(memBlock *mHead);
int deleteNode(ramNode *head, const char *path);
void addMemBlock(memBlock *head, memBlock *block);
int deleteFile(ramNode *head, const char *path);

#endif /* RAMNODE_H_ */
