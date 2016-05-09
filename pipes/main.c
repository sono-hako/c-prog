#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	pid_t sort, ls;
	int fd[2];

	if(pipe(fd) < 0) {
		perror("pipe failed");
		return 1;
	}
		   
	sort = fork();
	if(sort < 0) {//error happened during fork: sort
		perror("fork failed:sort");
		exit(-1);
	}
	
	if(sort == 0) { /* child process: sort*/
		ls = fork();
		if(ls < 0){//error happened during fork: ls
			perror("fork failed: ls");
		}
		if(ls == 0){/* child process: ls */
			close(fd[0]);//close read end of pipe in child: ls, dont need it
			dup2(fd[1], 1);//set write end to stdout
			close(fd[1]);//doesn't dup2 close fd[1] automatically?
			execlp("ls","ls",NULL);//run ls
		}
		/* back in child process: sort */
		close(fd[1]);//close write end of pipe in child: sort, dont need it
		dup2(fd[0], 0);//set read end to stdin
		execlp("sort","sort",NULL);//run sort (using ls output)

	} else { /* back in parent process */
		/* 
		I feel like something should happen here 
		sort is waiting for another line to exit...
		*/
	}
	return 0;
}