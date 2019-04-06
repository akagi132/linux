#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>

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

	char str[4][30] = {"ABCDEFG                   _1",
										 "       hijklmn            _2",
										 "              OPQRST      _3",
										 "                    uvwxyz_4"};

	for (int count = 0; count < 100; count++) {
		for (int i = 0; i < 4; i++) {
			char* input_str = (char*)malloc(MAX_MSG_SIZE);
			strncpy(input_str, str[i], MAX_MSG_SIZE);

			writeMessageToFirst(&shmem_addr, &input_str);

			unlockSemaphoe(semaphoe, 0);
			lockSemaphoe(semaphoe, 1);

			char* output_str = (char*)malloc(MAX_MSG_SIZE);

			readMessageFromLatter(&shmem_addr, &output_str);
			USER_MSG("%s\n", output_str);

			unlockSemaphoe(semaphoe, 0);
			lockSemaphoe(semaphoe, 1);
		}
	}
	char* input_str = (char*)malloc(MAX_MSG_SIZE);
	strncpy(input_str, "END", MAX_MSG_SIZE);

	writeMessageToFirst(&shmem_addr, &input_str);

	unlockSemaphoe(semaphoe, 0);
	lockSemaphoe(semaphoe, 1);

	char* output_str = (char*)malloc(MAX_MSG_SIZE);

	readMessageFromLatter(&shmem_addr, &output_str);
	USER_MSG("%s\n", output_str);

	unlockSemaphoe(semaphoe, 0);
  return 0;
}

