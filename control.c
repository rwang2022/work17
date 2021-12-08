#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#define SHM_KEY 24602
#define SEM_KEY 24603

union semun
{
   int val;
   struct semid_ds *buf;
   unsigned short *array;
   struct seminfo *__buf;
};

int main(int argc, char **argv)
{
   // first checks if there is a flag
   if (argc > 1)
   {
      // creating
      if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-create") == 0)
      {
         // create shared memory
         int shmd = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0640);

         // create semaphore
         int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);

         // set the value to 1
         union semun us;
         us.val = 1;
         int k = semctl(semd, 0, SETVAL, us);

         // create and open the file
         char *contents = argv[2];
         int file = open("contents.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
         close(file);

         return 0;
      }
      // removing
      else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-remove") == 0)
      {
         // remove shared memory
         int shmid = shmget(SHM_KEY, 0, 0);
         shmctl(shmid, IPC_RMID, 0);

         // remove semaphore
         int semid = semget(SEM_KEY, 1, 0);
         semctl(semid, IPC_RMID, 0);

         // display full contents of the story
         char contents[1000];
         int file = open("contents.txt", O_RDONLY);
         read(file, contents, 1000);
         printf("Full contents of the story: \n%s", contents);

         close(file);
         return 0;
      }

      // if not creating or removing
      else
      {
         printf("Invalid command. Please use the command flag -create or -remove to determine if it is creating/removing the resources.\n");
      }
   }

   //  if there is no flag
   else
   {
      printf("Invalid command. Please use the command flag -create or -remove to determine if it is creating/removing the resources.\n");
   }
   return 0;
}
