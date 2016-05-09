
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 20
#define BUF_MAX 1024
#define WR 0
#define RD 1

/*
  Splits the cmdline up by |
  Each existance of | informs program that a new child is going to be spawned
  With output of parent being piped as input into the new child.
  eg:
  sort < in.txt | cat
  populates args[] = {"sort < in.txt", "cat"}

  Allows me to determine how many child processes I need to spawn

*/
pid_t pid;

int get_pipeAmt(char* cmdline, char* args[]) 
{
  int i = 0;

  /* if no args */
  if((args[0] = strtok(cmdline, "|")) == NULL) 
    return 0; 

  while((args[++i] = strtok(NULL, "|")) != NULL) {
    if(i >= MAX_ARGS) {
      printf("Too many arguments!\n");
      exit(1);
    }
  }
  /* the last one is always NULL */
  return i;
}
/*
  Splits each process up into their specific arguments
  eg:
  sort < in.txt
  populates args[] = {"sort", "<", "in.txt"}

  Allows program to determin if there is additional arguments for each process
  (redirects, and files that the program needs)

*/
int get_processArg(char* cmdline, char* args[]){
  int i = 0;
  /* if no args */
  if((args[0] = strtok(cmdline, "\n\t ")) == NULL) 
    return 0; 

  while((args[++i] = strtok(NULL, "\n\t ")) != NULL) {
    printf("%i\n", i);
    if(i >= MAX_ARGS) {
      printf("too many arguments!\n");
      exit(1);
    }
  }
  return i;
}

void redirect(int old, int new){
  if(new == old)//no need for redirect
    return;
  if(dup2 (old, new) != -1)
    close(old);
  else{
    perror("redirection failed - redirect");
    exit(1);
  }

}
/*
  
  stdin/FILE --> child 1 --> out --> child 2 ---> stdout
*/
int execute(char* cmd, int start, int end, int in_fd){
    int pipes[2];
    int res;
    pipe(pipes);

    pid = fork();

    if(pid == 0){
        if(start){ //first child
          dup2(pipes[WR], STDOUT_FILENO);
          /*
          add logic to check for < here

          and and > and >> in case its the first and only child.
          */
        } else if (!start && !end){ //some middle
          dup2(in_fd, STDIN_FILENO);
          dup2(pipes[WR], STDOUT_FILENO);
        } else {//final process
          dup2(in_fd, STDIN_FILENO);
          /*
          add logic to check for existance of > and >> here
          */
        }

        res = execvp(cmd[0], cmd);
        if(res == -1){
          //come up with error prompt
        }

    }
    close(pipes[WR]);
    if(end)
      close(pipes[RD]);

    return pipes[RD];
}

void waitForChild(int cc){
  while(cc != 0){
    wait(NULL);
    cc--;
  }
}

int main (int argc, char* argv [])
{
  char cmdline[BUF_MAX];
  char* args[MAX_ARGS];
  char* args2[MAX_ARGS];
  const int PIPE_RD = 0;
  const int PIPE_WR = 1;
  int childCreated = 0;

  
  for(;;) {
    int c = 0;
    int fd_in = 0;
    int process;
    int numarg2;
    int first = 1;
    int last = 0;
    printf("COP4338$ ");
    if(fgets(cmdline, BUF_MAX, stdin) == NULL) {
      perror("fgets failed");
      exit(1);
    }
    //printf("%s",cmdline);
    process = get_pipeAmt(cmdline, args);
    
    //printf("%d : %s\n", process, *args2);
    while(args[c] != NULL){
      numarg2 = get_processArg(args[c], args2);

      fd_in = execute(args2, first, last, 0);
      //printf("%s", *args2)
      childCreated++;
      c++;
    }
    waitForChild(childCreated);
    childCreated = 0;
   //execvp(args2[0], args2);
    //execvp("ls", NULL);
    //execute(numArgs, args, PIPE_RD);

  }
  return 0;
}
