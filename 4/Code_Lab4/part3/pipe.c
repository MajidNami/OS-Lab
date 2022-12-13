#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void)
{
   char write_msg[BUFFER_SIZE] = "This is First Proccess";
   char read_msg[BUFFER_SIZE];
   int fd[2];
   pid_t pid;

   if(pipe(fd) == -1)
   {
      fprintf(stderr, "pipe failed\n");
      return 1;
   }
   pid = fork();

   if(pid < 0)
   {
      fprintf(stderr, "fork failed\n");
      return 1;
   }
   if(pid > 0)
   {
      close(fd[READ_END]);
      write(fd[WRITE_END], write_msg, strlen(write_msg));
      close(fd[WRITE_END]);
   }
   else
   {
      close(fd[WRITE_END]);
      read(fd[READ_END], read_msg, BUFFER_SIZE);
      for(int i = 0 ;i < strlen(read_msg); i++)
      {
         if(islower(read_msg[i]))
            read_msg[i] = toupper(read_msg[i]);

        else
            read_msg[i] = tolower(read_msg[i]);

      }
      printf("%s\n", read_msg);
      close(fd[READ_END]);
   }
   return 0;
}
