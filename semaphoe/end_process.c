#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

#include "print.h"
#include "protocol.h"

int createSharedMemory(void**, int);
void readMessageFromLatter(void**, char**);
void writeMessageToFirst(void**, char**);

int getSemaphoeKey(int);
int waitSemaphoe(int, int);
int unlockSemaphoe(int, int);
int lockSemaphoe(int, int);

int main() {
	int ret = 0;
	void* shmem_addr = NULL;
	int shmem_key = createSharedMemory(&shmem_addr, 0);
	
	int semaphoe = getSemaphoeKey(0666);

	char* input_str = (char*)malloc(MAX_MSG_SIZE);
	strncpy(input_str, "END", MAX_MSG_SIZE);

	waitSemaphoe(semaphoe, 1);

	writeMessageToFirst(&shmem_addr, &input_str);

	lockSemaphoe(semaphoe, 1);
	unlockSemaphoe(semaphoe, 0);

	waitSemaphoe(semaphoe, 1);

	char* output_message = (char*)malloc(MAX_MSG_SIZE);
	readMessageFromLatter(&shmem_addr, &output_message);
	USER_MSG("%s\n", output_message);

	lockSemaphoe(semaphoe, 1);
	unlockSemaphoe(semaphoe, 0);
  return 0;
}

