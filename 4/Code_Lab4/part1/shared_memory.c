#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    int seg_id;
    char* shared_memory;
    int size = 50;

    struct shmid_ds shm_buffer;
    int seg_size;

    seg_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);

    shared_memory = (char*) shmat (seg_id, NULL, 0);
    printf("shared memory address %p\n", shared_memory);

    shmctl(seg_id, IPC_STAT, &shm_buffer);
    seg_size = shm_buffer.shm_segsz;
    printf("segment size: %d\n", seg_size);

    sprintf(shared_memory, "%s", argv[1]);
    printf("Added to the shared memory: %s\n\n", argv[1]);

    shmdt(shared_memory);

    if(fork() == 0)
   	{
    	char *str = (char*) shmat(seg_id, NULL, 0);
  
    	printf("Data read from memory: %s\n",str);
      
    	shmdt(shared_memory);
    
    	shmctl(seg_id, IPC_RMID, NULL);
     
    	return 0;
   	}

    return 0;
}
