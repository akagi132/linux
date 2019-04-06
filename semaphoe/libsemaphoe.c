#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "protocol.h"
#include "print.h"

#define LOCK -1
#define UNLOCK 1
#define WAIT 0

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int getSemaphoeKey(int flg) {
	key_t ipc_key = ftok(FTOK_PATH, FTOK_ID);
	if (ipc_key < 0) {
		ERROR_MSG("can not create ipc_key.\n");
		return -1;
	}

	int semaphoe = semget(ipc_key, SEMAPHOE_NUM, flg);
	if (semaphoe < 0) {
		ERROR_MSG("can not create semaphoe.\n");
		return -1;
	}
	return semaphoe;
}

int createSemaphoe() {
	int semaphoe = getSemaphoeKey(IPC_CREAT | 0666);
	if (semaphoe < 0) {
		return -1;
	}

	short array[1];
	array[0] = 0;
	union semun argument;
	argument.array = array;
	int ret = semctl(semaphoe, 0, SETALL, argument);

	return semaphoe;
}

int deleteSemaphoe(int semaphoe) {
	int ret = semctl(semaphoe, 0, IPC_RMID);
	if (ret < 0) {
		ERROR_MSG("can not delete semaphoe.\n");
	}
	return ret;
}

int operateSemaphoe(int semaphoe_id, int semaphoe_num, int command) {
	struct sembuf arg[1];
	arg[0].sem_num = semaphoe_num;
	arg[0].sem_op = command;
	arg[0].sem_flg = 0;

	int ret = semop(semaphoe_id, arg, 1);
}

int lockSemaphoe(int semaphoe_id, int semaphoe_num) {
//	DEBUG_MSG("lock process %d\n", semaphoe_num);
	if (operateSemaphoe(semaphoe_id, semaphoe_num, LOCK) < 0) {
		ERROR_MSG("can not lock semaphoe.\n");
		return -1;
	}
	return 0;
}

int unlockSemaphoe(int semaphoe_id, int semaphoe_num) {
//	DEBUG_MSG("unlock process %d\n", semaphoe_num);
	if (operateSemaphoe(semaphoe_id, semaphoe_num, UNLOCK) < 0) {
		ERROR_MSG("can not unlock semaphoe.\n");
		return -1;
	}
	return 0;
}

int waitSemaphoe(int semaphoe_id, int semaphoe_num) {
//	DEBUG_MSG("wait semaphoe...\n");
	if (operateSemaphoe(semaphoe_id, semaphoe_num, WAIT) < 0) {
		ERROR_MSG("can not wait semaphoe.\n");
		return -1;
	}
	return 0;
}
