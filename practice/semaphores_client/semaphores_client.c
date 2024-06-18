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

// 0 0 - either client or server is processing data
// 0 1 - server waits request
// 1 0 - clients can start interaction session
// 1 1 - server answer is ready

int main()
{
	int error = 0;
	
	int sem_fd = semget(SEM_KEY, 2, 0666);
	if (sem_fd == -1)
	{
		printf("SEM OPEN ERROR\n");
		return -1;
	}
	
	int shm_fd = shmget(SHM_KEY, sizeof(shm_msg_t), 0666);
	if (sem_fd == -1)
	{
		printf("SEM OPEN ERROR\n");
		semctl(sem_fd, 2, IPC_RMID);
		return -1;
	}
	
	shm_msg_t *addr = (shm_msg_t *) shmat(shm_fd, NULL, 0666);
	
	struct sembuf sem_cmd[2] =
		{
			{ .sem_num = 0, .sem_op = -1, .sem_flg = 0 },
			{ .sem_num = 1, .sem_op = 0, .sem_flg = 0 },
		};
	
	while (1)
	{
		int res = 0;
		int cmd, data;
		
		printf("ENTER CMD: ");
		scanf("%d", &cmd);
		
		if (cmd > 1)
		{
			printf("INVALID\n");
			continue;
		}
		
		if (cmd == 1)
		{
			printf("ENTER VALUE: ");
			scanf("%d", &data);
		}
		
		// start interaction session
		
		sem_cmd[0].sem_op = -1;
		sem_cmd[1].sem_op = 0;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		// send request to server
		
		addr->cmd = cmd;
		addr->data = data;
		sem_cmd[0].sem_op = 0;
		sem_cmd[1].sem_op = 1;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		if (addr->cmd == 0)
		{
			break;
		}
		
		// wait for server answer
		
		sem_cmd[0].sem_op = -1;
		sem_cmd[1].sem_op = -1;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		// read and process server answer
		
		if (addr->cmd == 1)
		{
			printf("MULTIPLICATION RESULT: %d\n", addr->data);
			addr->data *= 2;
		}
		else
		{
			printf("INVALID\n");
			// INVALID ?
		}
		
		// end interaction session
		
		sem_cmd[0].sem_op = 1;
		sem_cmd[1].sem_op = 0;
		res = (res == -1) ? res : semop(sem_fd, sem_cmd, 2);
		
		if (res == -1)
		{
			error = 1;
			break;
		}
	}
	
	if (error)
	{
		printf("CRITICAL ERROR OCCURRED\n");
		return -1;
	}
	
}