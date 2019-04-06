#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#include "protocol.h"
#include "print.h"

int getSharedMemoryKey(int flg) {
	key_t ipc_key = ftok(FTOK_PATH, FTOK_ID);
	if (ipc_key < 0) {
		ERROR_MSG("can not create ipc_key.\n");
		return -1;
	}

	int shmem_key = shmget(ipc_key, SHARED_MEM_SIZE, flg);
	if (shmem_key < 0) {
		ERROR_MSG("can not create shared memory.\n");
		return -1;
	}
	return shmem_key;
}

int createSharedMemory(void** shmem_addr, int flg) {
	int shmem_key = getSharedMemoryKey(flg);

	if (shmem_key < 0) {
		ERROR_MSG("can not create shared memory.\n");
		return -1;
	}

	*shmem_addr = shmat(shmem_key, NULL, 0);

	if (shmem_addr < 0) {
		ERROR_MSG("can not attach shaerd memory.\n");
		return -1;
	}
	return shmem_key;
}

int deleteSharedMemory(int shmem_key, void** shmem_addr) {
	int ret = shmdt(*shmem_addr);
	if (ret < 0) {
		ERROR_MSG("can not detach shared memory.\n");
	}

	ret = shmctl(shmem_key, IPC_RMID, NULL);
	if (ret < 0) {
		ERROR_MSG("can not delete shared memory.\n");
	}

	return ret;
}

void writeMessageToFirst(void** shmem_addr, char** str) {
	char* len_area = (char*)*shmem_addr;
	char* str_area = (char*)(*shmem_addr) + sizeof(char) * 1;
	
	*len_area = strlen(*str);
	strncpy(str_area, *str, MAX_MSG_SIZE);
//	printf("writeMessageToFirst:%s\n", *str);
	return;
}

void writeMessageToLatter(void** shmem_addr, char** str) {
	char* len_area = (char*)(*shmem_addr) + OFFSET;
	char* str_area = (char*)(*shmem_addr) + OFFSET + sizeof(char) * 1;
	
	*len_area = strlen(*str);
	strncpy(str_area, *str, MAX_MSG_SIZE);
//	printf("writeMessageToLatter:%s\n", *str);
	return;
}

void readMessageFromFirst(void** shmem_addr, char** str) {
	char* message = (char*)(*shmem_addr) + sizeof(char) * 1;
	strncpy(*str, message, MAX_MSG_SIZE);
//	printf("readMessageFromFirst:%s\n", *str);
	return;
}

void readMessageFromLatter(void** shmem_addr, char** str) {
	char* message = (char*)(*shmem_addr) + OFFSET + sizeof(char) * 1;
	strncpy(*str, message, MAX_MSG_SIZE);
//	printf("readMessageFromLatter:%s\n", *str);
	return;
}

void reverseMessage(char** str) {
	int count = 0;
	int str_len = strlen(*str);
	while (count < str_len) {
		char* c = *str + sizeof(char) * count;
		if ('a' <= *c && *c <= 'z') {
			*c = *c - 32;
		} else if ('A' <= *c && *c <= 'Z') {
			*c = *c + 32;
		}
		count++;
	}
//	printf("reverseMessage:%s\n", *str);
	return;
}

bool isEqualMessage(char* str, char* cmp) {
	if (strcmp(str, cmp) == 0) {
		return true;
	}
	return false;
}

