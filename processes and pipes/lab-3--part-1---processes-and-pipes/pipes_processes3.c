#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid,pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", NULL, NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  if(pipe(pipefd) == -1){
      fprintf(stderr, "The Pipe1 Failed\n" );
      return 1; 
   }
  
  if(pipe(pipefd2) == -1){
      fprintf(stderr, "The Pipe2 Failed" );
      return 1;
   }

  pid = fork();
  
  if(argc<1){
    printf("Please enter an argument for grep\n");
  }
  grep_args[1] = argv[1];

  if (pid == 0){
    
    
    pid2 = fork();
    
    if(pid2 == 0){
      dup2(pipefd2[0], 0);
      
      
      close(pipefd[0]);
			close(pipefd[1]);
    
			close(pipefd2[0]);
			close(pipefd2[1]);
      
      execvp("sort", sort_args);
    }
    
    else if(pid2<0){
      perror("error");
    }
    
    else{
       // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd[0], 0);
      dup2(pipefd2[1], 1);

      // close unused hald of pipe
      
      close(pipefd[0]);
			close(pipefd[1]);
    
			close(pipefd2[0]);
			close(pipefd2[1]);

      // execute grep

      execvp("grep", grep_args);
    }
    
     
    }
  else{
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);
			close(pipefd[1]);
    
			close(pipefd2[0]);
			close(pipefd2[1]);

      // execute cat

      execvp("cat", cat_args);
    }
}