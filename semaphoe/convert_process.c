#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>

#include "print.h"
#include "protocol.h"

int createSharedMemory(void**, int);
int deleteSharedMemory(int, void**);

int createSemaphoe();
int deleteSemaphoe();

int unlockSemaphoe(int, int);
int lockSemaphoe(int, int);
int waitSemaphoe(int, int);

bool isEqualMessage(char*, char*);

void readMessageFromFirst(void**, char**);
void writeMessageToLatter(void**, char**);

void reverseMessage(char**);

int main(void) {
	int ret = 0;
	void* shmem_addr = NULL;
	int shmem_key = createSharedMemory(&shmem_addr, IPC_CREAT | 0666);
	int semaphoe = createSemaphoe();

	lockSemaphoe(semaphoe, 0);
	while (1) {
		char* input_message = (char*)malloc(MAX_MSG_SIZE);
		char* output_message = (char*)malloc(MAX_MSG_SIZE);
		
		readMessageFromFirst(&shmem_addr, &input_message);
		strncpy(output_message, input_message, MAX_MSG_SIZE);

		reverseMessage(&output_message);
		writeMessageToLatter(&shmem_addr, &output_message);

		unlockSemaphoe(semaphoe, 1);
		lockSemaphoe(semaphoe, 0);

		waitSemaphoe(semaphoe, 0);

		if (isEqualMessage(input_message, "END")) {
			USER_MSG("process has finished successfully.\n");
			deleteSemaphoe(semaphoe);
			deleteSharedMemory(shmem_key, &shmem_addr);
			return 0;
		}

		unlockSemaphoe(semaphoe, 1);
		lockSemaphoe(semaphoe, 0);
	}
	deleteSemaphoe(semaphoe);
	deleteSharedMemory(shmem_key, &shmem_addr);
  return -1;
}
