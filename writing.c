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

int main()
{
   struct sembuf sb;
   sb.sem_num = 0; 
   // set operation to down
   sb.sem_op = -1; 

   // access semaphore
   int semd = semget(SEM_KEY, 1, 0);
   semop(semd, &sb, 1);
   // open file
   int file = open("contents.txt", O_RDWR | O_APPEND);

   // access shared memory
   int shmd = shmget(SHM_KEY, 0, 0);
   int *last_size = shmat(shmd, 0, 0);
   lseek(file, -1 * (*last_size), SEEK_END);

   // print last line added to file
   char line[*last_size];
   read(file, line, (*last_size) - 1);
   printf("Last line: %s\n", line);

   // prompt the user for the next line
   printf("Enter the next line: ");
   char input[1000];
   fgets(input, 1000, stdin);

   *last_size = strlen(input);
   write(file, input, strlen(input));

   shmdt(last_size);

   // set the operation to up
   sb.sem_op = 1;       
   semop(semd, &sb, 1); 

   close(file);
   return 0;
}
