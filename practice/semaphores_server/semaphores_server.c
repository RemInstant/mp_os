#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEM_KEY 100
#define SHM_KEY 100

typedef struct shm_msg_t
{
	int cmd;
	int data;
} shm_msg_t;

typedef union semun
{
	int				 val;		// значение семафора
	struct semid_ds	*buf;		// управляющие параметры набора
	unsigned short	 array[2];	// массив значений семафора
} semun;

// 0 0 - either client or server is processing data
// 0 1 - server waits request
// 1 0 - clients can start interaction session
// 1 1 - server answer is ready

int main()
{
	semctl(semget(SEM_KEY, 2, IPC_CREAT | 0666), 0, IPC_RMID);
	shmctl(shmget(SHM_KEY, sizeof(shm_msg_t), IPC_CREAT | 0666), IPC_RMID, NULL);
	
	int error = 0;
	
	int sem_fd = semget(SEM_KEY, 2, IPC_CREAT | 0666);
	if (sem_fd == -1)
	{
		printf("SEM OPEN ERROR\n");
		return -1;
	}
	
	int shm_fd = shmget(SHM_KEY, sizeof(shm_msg_t), IPC_CREAT | 0666);
	if (sem_fd == -1)
	{
		printf("SEM OPEN ERROR\n");
		semctl(sem_fd, 2, IPC_RMID);
		return -1;
	}
	
	shm_msg_t *addr = (shm_msg_t *) shmat(shm_fd, NULL, 0666);
	
	struct sembuf sem_cmd[2] =
		{
			{ .sem_num = 0, .sem_op = 1, .sem_flg = 0 },
			{ .sem_num = 1, .sem_op = 0, .sem_flg = 0 },
		};
	
	if (semop(sem_fd, sem_cmd, 2) == -1)
	{
		error = 1;
	}
	
	while (1)
	{
		int res = 0;
		
		// wait for client msg
		
		sem_cmd[0].sem_op = 0;
		sem_cmd[1].sem_op = -1;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		// read and process client msg
		
		if (addr->cmd == 0)
		{
			printf("SHUTDOWN\n");
			break;
		}
		else if (addr->cmd == 1)
		{
			printf("MULTIPLICATION\n");
			addr->data *= 2;
		}
		else
		{
			printf("INVALID\n");
			// INVALID ?
		}
		
		// send answer to client
		
		semun aboba;
		res = (res == -1) ? res : semctl(sem_fd, 2, GETALL, aboba.array);
		
		sem_cmd[0].sem_op = 1;
		sem_cmd[1].sem_op = 1;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		if (res == -1)
		{
			error = 1;
			break;
		}
	}
	
	shmdt(addr);
	semctl(sem_fd, 0, IPC_RMID);
	shmctl(shm_fd, IPC_RMID, NULL);
	
	if (error)
	{
		printf("CRITICAL ERROR OCCURRED\n");
		return -1;
	}
	
}