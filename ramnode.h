/*
 * ramnode.h
 *
 *  Created on: Nov 15, 2016
 *      Author: dinesh
 */

#ifndef RAMNODE_H_
#define RAMNODE_H_

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>


typedef enum d_type {
	DIR_TYPE,
	FILE_TYPE
} f_type;

/* To maintain the file or directory information */
struct _fileMetaTable {
	char *name;
	f_type type;
	mode_t mode;

	size_t size;

	time_t atime;
	time_t mtime;
	time_t ctime;

	unsigned int startBlock;

	struct _fileMetaTable *parent;


};
typedef struct _fileMetaTable *ramNode;

/* To maintain the blocks allocated
 * The block size considered is 1KB
 *
 */
struct _memoryBlocks {
	char data[1020];
	struct _memoryBlocks *next;

};
typedef struct _memoryBlocks *memBlock;


#endif /* RAMNODE_H_ */
