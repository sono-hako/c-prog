#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF 1024

void header(char* path){
	printf("==> %s <==\n", path);
}

int lastBytes(char* path, int bytes){// -c [num]
	int fd;

	char* buf[MAX_BUF];

	if(path){
		if((fd = open(path, O_RDONLY)) < 0){
			fprintf(stderr, "Cannot read file %s\n.", path);
			return -1;
		}
	}

	if(lseek(fd, (0 - bytes), SEEK_END) == -1){//jump to end, then move insertion point back 'bytes'
		fprintf(stderr, "Cannot seek to location: %d.\n", bytes);
		return -1;
	}

	while(read(fd, buf, MAX_BUF) != 0){
		write(STDOUT_FILENO, buf, 1024);
	}

	return 0;
}

int lastLines(char* path, int lines){// -n [num]
	//count lines, then parse through?
	int fd, pos, linesLeft = lines;
	char* buf[MAX_BUF];
	char seek[1];

	if(path){
		if((fd = open(path, O_RDONLY)) < 0){
			fprintf(stderr, "Cannot read file %s\n.", path);
			return -1;
		}
	}
	if((pos = lseek(fd, -1, SEEK_END)) < 0){
		fprintf(stderr, "Cannot seek to location: %d.\n", -1);
		return -1;
	}


	return 0;
}

int main(int argc, char** argv){
	int op;
	int endBytes = 0;
	int qFlag = 0, cFlag = 0, nFlag = 0;
	char* inPath = NULL;

	opterr = 0;
	while((op = getopt(argc, argv, "qc:n:")) != -1){
		switch(op){
			case 'c':
				endBytes = atoi(optarg);
				cFlag = 1;
				break;
			case 'q':
				qFlag = 1;
				break;
			case 'n':
				break;
			case '?':
				break;
			default:
				//dun goofed
				break; 
		}
	}
	while(optind < argc){
		inPath = *(argv+optind);
		if(!qFlag){
			header(inPath);
		}
		if(cFlag)
			lastBytes(inPath, endBytes);
		else if(nFlag){

		}
		printf("\n");
		optind++;

		//i really just want to do execvp
	}
}